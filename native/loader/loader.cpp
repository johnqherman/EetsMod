// libeetsmod.so - native mod loader for Eets (LD_PRELOAD).
//
// Mods are plain .cpp files dropped in <game>/mods/. On launch the loader
// compiles each to a cached .so (recompiling only when the source changes),
// dlopen's it, and dispatches Init/Update/OnKey/OnMouse/OnWheel/OnEvent/Shutdown.
// Mod callbacks run inside a crash guard: a faulting mod is disabled, not fatal.
//
// Interposition: Eets links FNA3D/SDL2 dynamically, so their PLT calls can be
// hooked (the engine's internal, statically-linked calls cannot):
//   FNA3D_SwapBuffers -> per-frame tick (load on first frame; Update each;
//                        poll sources for hot-recompile; draw menu banner)
//   SDL_PollEvent     -> keyboard + mouse events
// Mods call engine functions directly at fixed addresses (eets_engine.h) and can
// detour arbitrary engine functions via Eets::Hook (hook.h).
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <csetjmp>
#include <csignal>
#include "eets_engine.h"
#include "hook.h"

#define EETSMOD_VERSION "0.5.0"

namespace {

typedef void (*InitFn)();
typedef void (*UpdateFn)();
typedef void (*KeyFn)(int, int, int);
typedef void (*MouseFn)(int, int, int, int);
typedef void (*WheelFn)(int, int);
typedef void (*EventFn)(const char*, void*, void*);
typedef void (*ShutdownFn)();

struct Mod {
	std::string name;
	std::string src;
	std::string so;
	void*       handle = nullptr;
	time_t      srcmtime = 0;
	bool        disabled = false;
	// manifest (optional, from <name>.cfg meta keys)
	std::string version, author;
	int         priority = 0;
	std::vector<std::string> requires_;
	// entry points
	InitFn      init = nullptr;
	UpdateFn    update = nullptr;
	KeyFn       onkey = nullptr;
	MouseFn     onmouse = nullptr;
	WheelFn     onwheel = nullptr;
	EventFn     onevent = nullptr;
	ShutdownFn  shutdown = nullptr;
};

std::vector<Mod> g_mods;
bool   g_loaded = false;
unsigned long g_frame = 0;
const unsigned RELOAD_POLL_FRAMES = 30;
int g_mouse_x = 0, g_mouse_y = 0;

FILE* logfile() { static FILE* f = fopen("Log/native_mods.log", "a"); return f; }
void logline(const char* fmt, ...) {
	char buf[2048];
	va_list ap; va_start(ap, fmt); vsnprintf(buf, sizeof(buf), fmt, ap); va_end(ap);
	fprintf(stderr, "[eetsmod] %s\n", buf);
	FILE* f = logfile(); if (f) { fprintf(f, "%s\n", buf); fflush(f); }
}

// ---- crash guard: a faulting mod is disabled, the game survives -------------
sigjmp_buf g_jmp;
volatile sig_atomic_t g_guarding = 0;
struct sigaction g_old_sa[8];
const int g_sigs[] = { SIGSEGV, SIGFPE, SIGILL, SIGBUS };

void crash_handler(int sig, siginfo_t* info, void* uctx) {
	if (g_guarding) { g_guarding = 0; siglongjmp(g_jmp, sig); }
	// not in a mod: chain to the engine's / default handler
	for (size_t i = 0; i < sizeof(g_sigs)/sizeof(g_sigs[0]); i++) {
		if (g_sigs[i] == sig) {
			struct sigaction& o = g_old_sa[i];
			if (o.sa_flags & SA_SIGINFO) { if (o.sa_sigaction) { o.sa_sigaction(sig, info, uctx); return; } }
			else if (o.sa_handler && o.sa_handler != SIG_DFL && o.sa_handler != SIG_IGN) { o.sa_handler(sig); return; }
			break;
		}
	}
	signal(sig, SIG_DFL); raise(sig);
}
void install_guards() {
	struct sigaction sa; memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = crash_handler; sa.sa_flags = SA_SIGINFO | SA_NODEFER; sigemptyset(&sa.sa_mask);
	for (size_t i = 0; i < sizeof(g_sigs)/sizeof(g_sigs[0]); i++)
		sigaction(g_sigs[i], &sa, &g_old_sa[i]);
}
// run fn() guarded; on fault mark the mod disabled. Returns false if it crashed.
template <class Fn>
bool guard(Mod* m, Fn&& fn) {
	if (m && m->disabled) return false;
	if (sigsetjmp(g_jmp, 1) == 0) {
		g_guarding = 1; fn(); g_guarding = 0; return true;
	}
	g_guarding = 0;
	if (m) { m->disabled = true; logline("CRASH in mod '%s' - disabled", m->name.c_str()); }
	return false;
}

std::string modsdir() { const char* e = getenv("EETS_MODS"); return e ? e : "mods"; }
std::string cachedir() { return modsdir() + "/.cache"; }
time_t mtime_of(const std::string& p) { struct stat s; return stat(p.c_str(), &s) == 0 ? s.st_mtime : 0; }
bool   exists(const std::string& p)   { struct stat s; return stat(p.c_str(), &s) == 0; }

std::string loaderdir() {
	Dl_info info;
	if (dladdr((void*)&logline, &info) && info.dli_fname) {
		std::string p = info.dli_fname;
		size_t slash = p.find_last_of('/');
		if (slash != std::string::npos) return p.substr(0, slash);
	}
	return ".";
}
std::string includedir() {
	const char* e = getenv("EETS_INCLUDE");
	if (e) return e;
	std::string cand = loaderdir() + "/eetsmod-include";
	if (exists(cand + "/eetsmod.h")) return cand;
	return modsdir() + "/.include";
}
const char* compiler() { const char* e = getenv("CXX"); return (e && *e) ? e : "g++"; }

bool compile(const std::string& src, const std::string& so) {
	std::string cmd = std::string(compiler()) +
		" -O2 -fPIC -std=c++17 -shared -w -I\"" + includedir() + "\" -o \"" + so + "\" \"" + src + "\""
		" >> Log/native_mods.log 2>&1";
	logline("compile: %s", src.c_str());
	int rc = system(cmd.c_str());
	if (rc != 0) { logline("compile FAILED (%d): %s", rc, src.c_str()); return false; }
	return true;
}

void resolve(Mod& m) {
	m.init     = (InitFn)    dlsym(m.handle, "EetsMod_Init");
	m.update   = (UpdateFn)  dlsym(m.handle, "EetsMod_Update");
	m.onkey    = (KeyFn)     dlsym(m.handle, "EetsMod_OnKey");
	m.onmouse  = (MouseFn)   dlsym(m.handle, "EetsMod_OnMouse");
	m.onwheel  = (WheelFn)   dlsym(m.handle, "EetsMod_OnWheel");
	m.onevent  = (EventFn)   dlsym(m.handle, "EetsMod_OnEvent");
	m.shutdown = (ShutdownFn)dlsym(m.handle, "EetsMod_Shutdown");
}

bool open_mod(Mod& m) {
	if (!m.src.empty()) {
		m.srcmtime = mtime_of(m.src);
		bool stale = !exists(m.so) || mtime_of(m.so) < m.srcmtime;
		if (stale && !compile(m.src, m.so)) return false;
	}
	m.handle = dlopen(m.so.c_str(), RTLD_NOW | RTLD_GLOBAL);
	if (!m.handle) { logline("dlopen failed %s: %s", m.so.c_str(), dlerror()); return false; }
	resolve(m);
	return true;
}
void close_mod(Mod& m) {
	if (m.shutdown && !m.disabled) guard(&m, [&]{ m.shutdown(); });
	if (m.handle) dlclose(m.handle);
	m.handle = nullptr;
}

bool ends_with(const std::string& s, const char* suf) {
	size_t n = strlen(suf); return s.size() >= n && s.compare(s.size() - n, n, suf) == 0;
}
std::string stem(const std::string& f) {
	size_t dot = f.find_last_of('.'); return dot == std::string::npos ? f : f.substr(0, dot);
}
std::string trim(const std::string& s) {
	size_t a = s.find_first_not_of(" \t\r\n"), b = s.find_last_not_of(" \t\r\n");
	return a == std::string::npos ? std::string() : s.substr(a, b - a + 1);
}

// ---- config + manifest (mods/<mod>.cfg, key=value) ------------------------
std::map<std::string, std::map<std::string, std::string>> g_cfg;
std::map<std::string, std::string>& cfg_for(const char* mod) {
	auto it = g_cfg.find(mod); if (it != g_cfg.end()) return it->second;
	auto& m = g_cfg[mod];
	std::string path = modsdir() + "/" + mod + ".cfg";
	FILE* f = fopen(path.c_str(), "r");
	if (f) {
		char line[512];
		while (fgets(line, sizeof(line), f)) {
			std::string s = trim(line);
			if (s.empty() || s[0] == '#') continue;
			size_t eq = s.find('='); if (eq == std::string::npos) continue;
			m[trim(s.substr(0, eq))] = trim(s.substr(eq + 1));
		}
		fclose(f); logline("config: loaded %s (%zu keys)", path.c_str(), m.size());
	}
	return m;
}
void read_manifest(Mod& m) {
	auto& c = cfg_for(m.name.c_str());
	auto get = [&](const char* k)->std::string{ auto i=c.find(k); return i!=c.end()?i->second:std::string(); };
	m.version = get("version"); m.author = get("author");
	std::string pr = get("priority"); if (!pr.empty()) m.priority = atoi(pr.c_str());
	std::string req = get("requires");
	size_t p = 0;
	while (p < req.size()) {
		size_t c2 = req.find(',', p);
		std::string one = trim(req.substr(p, c2 == std::string::npos ? std::string::npos : c2 - p));
		if (!one.empty()) m.requires_.push_back(one);
		if (c2 == std::string::npos) break; p = c2 + 1;
	}
}
bool have_mod(const std::string& name) {
	for (auto& m : g_mods) if (m.name == name && !m.disabled) return true;
	return false;
}

// ---- events ---------------------------------------------------------------
void fire_event(const char* name, void* a, void* b) {
	for (auto& m : g_mods) if (m.onevent && !m.disabled)
		guard(&m, [&]{ m.onevent(name, a, b); });
}

// detour: ObjectMgr::CreateObject -> "object_spawn" event
typedef void* (*CreateObjectFn)(void*, const char*, unsigned);
CreateObjectFn orig_CreateObject = nullptr;
void* det_CreateObject(void* self, const char* name, unsigned layer) {
	void* o = orig_CreateObject(self, name, layer);
	fire_event("object_spawn", o, (void*)name);
	return o;
}
void install_engine_event_hooks() {
	if (eets_hook::install((void*)0x576290, (void*)det_CreateObject, (void**)&orig_CreateObject))
		logline("hook: ObjectMgr::CreateObject -> object_spawn event");
	else
		logline("hook: ObjectMgr::CreateObject NOT hooked (prologue not relocatable)");
}

// ---- discovery + load -----------------------------------------------------
void load_all() {
	install_guards();
	std::string dir = modsdir();
	mkdir(cachedir().c_str(), 0755);
	logline("eetsmod v%s  include=%s compiler=%s", EETSMOD_VERSION, includedir().c_str(), compiler());
	logline("loader: scanning %s", dir.c_str());

	// 1. discover mod files + read manifests
	std::vector<Mod> found;
	DIR* d = opendir(dir.c_str());
	if (d) {
		struct dirent* ent;
		while ((ent = readdir(d)) != nullptr) {
			std::string n = ent->d_name;
			Mod m; m.name = stem(n);
			if (ends_with(n, ".cpp")) { m.src = dir + "/" + n; m.so = cachedir() + "/" + m.name + ".so"; }
			else if (ends_with(n, ".so")) { m.so = dir + "/" + n; }
			else continue;
			read_manifest(m);
			found.push_back(m);
		}
		closedir(d);
	} else logline("loader: no mods dir (%s)", dir.c_str());

	// 2. order by priority (high first)
	for (size_t i = 0; i < found.size(); i++)
		for (size_t j = i + 1; j < found.size(); j++)
			if (found[j].priority > found[i].priority) std::swap(found[i], found[j]);

	// 3. compile + open + dependency check
	for (auto& m : found) {
		if (!open_mod(m)) continue;
		g_mods.push_back(m);
		logline("loader: loaded %s%s%s (update=%d key=%d mouse=%d event=%d)%s",
		        m.name.c_str(),
		        m.version.empty() ? "" : (" v" + m.version).c_str(),
		        m.priority ? (" [prio " + std::to_string(m.priority) + "]").c_str() : "",
		        m.update?1:0, m.onkey?1:0, m.onmouse?1:0, m.onevent?1:0,
		        m.src.empty() ? " [prebuilt]" : " [compiled]");
	}
	for (auto& m : g_mods) {
		for (auto& dep : m.requires_) if (!have_mod(dep)) {
			m.disabled = true;
			logline("dep: '%s' requires missing '%s' - disabled", m.name.c_str(), dep.c_str());
		}
	}

	// 4. engine event hooks, then init
	install_engine_event_hooks();
	for (auto& m : g_mods) if (m.init && !m.disabled) guard(&m, [&]{ m.init(); });
	int active = 0; for (auto& m : g_mods) if (!m.disabled) active++;
	logline("loader: %d/%zu mod(s) active", active, g_mods.size());
}

void poll_reload() {
	std::string dir = modsdir();
	for (auto& m : g_mods) {
		if (m.src.empty()) continue;
		time_t t = mtime_of(m.src);
		if (t != 0 && t != m.srcmtime) {
			logline("reload: %s changed", m.name.c_str());
			close_mod(m);
			m.disabled = false;
			if (open_mod(m) && m.init) guard(&m, [&]{ m.init(); });
		}
	}
	DIR* d = opendir(dir.c_str());
	if (!d) return;
	struct dirent* ent;
	while ((ent = readdir(d)) != nullptr) {
		std::string n = ent->d_name;
		std::string st = stem(n);
		if (!ends_with(n, ".cpp") && !ends_with(n, ".so")) continue;
		bool known = false; for (auto& m : g_mods) if (m.name == st) { known = true; break; }
		if (known) continue;
		Mod m; m.name = st;
		if (ends_with(n, ".cpp")) { m.src = dir + "/" + n; m.so = cachedir() + "/" + st + ".so"; }
		else { m.so = dir + "/" + n; }
		read_manifest(m);
		if (open_mod(m)) { g_mods.push_back(m); logline("reload: new mod %s", st.c_str());
		                   if (g_mods.back().init) guard(&g_mods.back(), [&]{ g_mods.back().init(); }); }
	}
	closedir(d);
}

// ---- SDL event constants / views ------------------------------------------
constexpr unsigned SDL_KEYDOWN = 0x300, SDL_KEYUP = 0x301;
constexpr unsigned SDL_MOUSEMOTION = 0x400, SDL_MOUSEBUTTONDOWN = 0x401,
                   SDL_MOUSEBUTTONUP = 0x402, SDL_MOUSEWHEEL = 0x403;
struct KeyView { unsigned type, ts, win; unsigned char state, repeat, p0, p1; int scancode, sym; unsigned short mod; };
struct MotionView { unsigned type, ts, win, which, state; int x, y, xrel, yrel; };
struct ButtonView { unsigned type, ts, win, which; unsigned char button, state, clicks, pad; int x, y; };
struct WheelView { unsigned type, ts, win, which; int x, y; unsigned dir; };

} // namespace

namespace Eets {
	void Log(const char* fmt, ...) {
		char buf[2048];
		va_list ap; va_start(ap, fmt); vsnprintf(buf, sizeof(buf), fmt, ap); va_end(ap);
		logline("%s", buf);
	}
	const char* ConfigGet(const char* mod, const char* key, const char* def) {
		auto& m = cfg_for(mod); auto it = m.find(key);
		return it != m.end() ? it->second.c_str() : def;
	}
	int   ConfigGetInt(const char* mod, const char* key, int def)     { const char* v = ConfigGet(mod, key, nullptr); return v ? atoi(v) : def; }
	float ConfigGetFloat(const char* mod, const char* key, float def) { const char* v = ConfigGet(mod, key, nullptr); return v ? (float)atof(v) : def; }
	bool  Hook(void* target, void* detour, void** original) { return eets_hook::install(target, detour, original); }
	int   MouseX() { return g_mouse_x; }
	int   MouseY() { return g_mouse_y; }
}

extern "C" {

void FNA3D_SwapBuffers(void* device, void* src, void* dst, void* window) {
	static void (*real)(void*, void*, void*, void*) = nullptr;
	if (!real) real = (decltype(real))dlsym(RTLD_NEXT, "FNA3D_SwapBuffers");
	if (!g_loaded) { g_loaded = true; load_all(); }
	for (auto& m : g_mods) if (m.update && !m.disabled) guard(&m, [&]{ m.update(); });
	if (++g_frame % RELOAD_POLL_FRAMES == 0) poll_reload();

	if (g_loaded && Eets::World_IsInMainMenu()) {
		size_t active = 0; for (auto& m : g_mods) if (!m.disabled) active++;
		char banner[128];
		snprintf(banner, sizeof(banner), "v%s, %zu mods loaded", EETSMOD_VERSION, active);
		int h = Eets::ScreenHeight(); if (h <= 0) h = 720;
		Eets::DrawTextSized(10, h - 16, banner, Eets::FONT_NORMAL, Eets::Colour(255, 255, 255, 255), 0.5f);
		static bool once = false;
		if (!once) { once = true; logline("menu banner: \"%s\" (screen h=%d)", banner, h); }
	}
	if (real) real(device, src, dst, window);
}

int SDL_PollEvent(void* event) {
	static int (*real)(void*) = nullptr;
	if (!real) real = (decltype(real))dlsym(RTLD_NEXT, "SDL_PollEvent");
	int r = real ? real(event) : 0;
	if (r && event && g_loaded) {
		unsigned type = *(unsigned*)event;
		if (type == SDL_KEYDOWN || type == SDL_KEYUP) {
			KeyView* k = (KeyView*)event; int down = (type == SDL_KEYDOWN) ? 1 : 0;
			for (auto& m : g_mods) if (m.onkey && !m.disabled) guard(&m, [&]{ m.onkey(k->sym, k->mod, down); });
		} else if (type == SDL_MOUSEMOTION) {
			MotionView* mv = (MotionView*)event; g_mouse_x = mv->x; g_mouse_y = mv->y;
			for (auto& m : g_mods) if (m.onmouse && !m.disabled) guard(&m, [&]{ m.onmouse(mv->x, mv->y, -1, 0); });
		} else if (type == SDL_MOUSEBUTTONDOWN || type == SDL_MOUSEBUTTONUP) {
			ButtonView* b = (ButtonView*)event; g_mouse_x = b->x; g_mouse_y = b->y;
			int down = (type == SDL_MOUSEBUTTONDOWN) ? 1 : 0;
			for (auto& m : g_mods) if (m.onmouse && !m.disabled) guard(&m, [&]{ m.onmouse(b->x, b->y, b->button, down); });
		} else if (type == SDL_MOUSEWHEEL) {
			WheelView* w = (WheelView*)event;
			for (auto& m : g_mods) if (m.onwheel && !m.disabled) guard(&m, [&]{ m.onwheel(w->x, w->y); });
		}
	}
	return r;
}

} // extern "C"
