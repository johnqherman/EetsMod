// libeetsmod.so - native mod loader for Eets (LD_PRELOAD).
//
// Mods are plain .cpp files dropped in <game>/mods/. On launch the loader
// compiles each to a cached .so (recompiling only when the source changes),
// dlopen's it, and dispatches Init/Update/OnKey/Shutdown. Prebuilt .so files
// in mods/ are also loaded directly.
//
// Interposition: Eets links FNA3D/SDL2 dynamically, so their PLT calls can be
// hooked (the engine's internal, statically-linked Lua calls cannot):
//   FNA3D_SwapBuffers -> per-frame tick (load on first frame; Update each;
//                        poll sources for hot-recompile)
//   SDL_PollEvent     -> keyboard events
// Mods call engine functions directly at fixed addresses (eets_engine.h).
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
#include "eets_engine.h"

#define EETSMOD_VERSION "0.4.0"

namespace {

typedef void (*InitFn)();
typedef void (*UpdateFn)();
typedef void (*KeyFn)(int, int, int);
typedef void (*ShutdownFn)();

struct Mod {
	std::string name;       // mod name (file stem)
	std::string src;        // .cpp path ("" if prebuilt .so)
	std::string so;         // .so path (cache for compiled, or the file itself)
	void*       handle = nullptr;
	time_t      srcmtime = 0;
	InitFn      init = nullptr;
	UpdateFn    update = nullptr;
	KeyFn       onkey = nullptr;
	ShutdownFn  shutdown = nullptr;
};

std::vector<Mod> g_mods;
bool   g_loaded = false;
unsigned long g_frame = 0;
const unsigned RELOAD_POLL_FRAMES = 30;

FILE* logfile() { static FILE* f = fopen("Log/native_mods.log", "a"); return f; }
void logline(const char* fmt, ...) {
	char buf[2048];
	va_list ap; va_start(ap, fmt); vsnprintf(buf, sizeof(buf), fmt, ap); va_end(ap);
	fprintf(stderr, "[eetsmod] %s\n", buf);
	FILE* f = logfile(); if (f) { fprintf(f, "%s\n", buf); fflush(f); }
}

std::string modsdir() { const char* e = getenv("EETS_MODS"); return e ? e : "mods"; }
std::string cachedir() { return modsdir() + "/.cache"; }

time_t mtime_of(const std::string& p) { struct stat s; return stat(p.c_str(), &s) == 0 ? s.st_mtime : 0; }
bool   exists(const std::string& p)   { struct stat s; return stat(p.c_str(), &s) == 0; }

// directory containing libeetsmod.so (for locating bundled headers)
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
	return modsdir() + "/.include";   // fallback
}
const char* compiler() {
	const char* e = getenv("CXX"); if (e && *e) return e;
	return "g++";
}

// compile src -> so against the bundled headers. returns true on success.
bool compile(const std::string& src, const std::string& so) {
	std::string inc = includedir();
	std::string cmd = std::string(compiler()) +
		" -O2 -fPIC -std=c++17 -shared -w -I\"" + inc + "\" -o \"" + so + "\" \"" + src + "\""
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
	m.shutdown = (ShutdownFn)dlsym(m.handle, "EetsMod_Shutdown");
}

// build (if needed) and open. returns true on success.
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
	if (m.shutdown) m.shutdown();
	if (m.handle) dlclose(m.handle);
	m.handle = nullptr;
}

bool ends_with(const std::string& s, const char* suf) {
	size_t n = strlen(suf);
	return s.size() >= n && s.compare(s.size() - n, n, suf) == 0;
}
std::string stem(const std::string& f) {
	size_t dot = f.find_last_of('.');
	return dot == std::string::npos ? f : f.substr(0, dot);
}

void load_all() {
	std::string dir = modsdir();
	mkdir(cachedir().c_str(), 0755);
	logline("loader: include=%s compiler=%s", includedir().c_str(), compiler());
	logline("loader: scanning %s", dir.c_str());
	DIR* d = opendir(dir.c_str());
	if (!d) { logline("loader: no mods dir (%s)", dir.c_str()); return; }
	struct dirent* ent;
	while ((ent = readdir(d)) != nullptr) {
		std::string n = ent->d_name;
		Mod m; m.name = stem(n);
		if (ends_with(n, ".cpp")) { m.src = dir + "/" + n; m.so = cachedir() + "/" + m.name + ".so"; }
		else if (ends_with(n, ".so")) { m.so = dir + "/" + n; }
		else continue;
		if (!open_mod(m)) continue;
		logline("loader: loaded %s (init=%d update=%d onkey=%d shutdown=%d)%s",
		        m.name.c_str(), m.init?1:0, m.update?1:0, m.onkey?1:0, m.shutdown?1:0,
		        m.src.empty() ? " [prebuilt]" : " [compiled]");
		g_mods.push_back(m);
	}
	closedir(d);
	for (auto& m : g_mods) if (m.init) m.init();
	logline("loader: %zu mod(s) active", g_mods.size());
}

void poll_reload() {
	std::string dir = modsdir();
	// recompile + reload changed sources
	for (auto& m : g_mods) {
		if (m.src.empty()) continue;
		time_t t = mtime_of(m.src);
		if (t != 0 && t != m.srcmtime) {
			logline("reload: %s changed", m.name.c_str());
			close_mod(m);
			if (open_mod(m) && m.init) m.init();
		}
	}
	// pick up newly added .cpp/.so
	DIR* d = opendir(dir.c_str());
	if (!d) return;
	struct dirent* ent;
	while ((ent = readdir(d)) != nullptr) {
		std::string n = ent->d_name;
		std::string st = stem(n);
		if (!ends_with(n, ".cpp") && !ends_with(n, ".so")) continue;
		bool known = false;
		for (auto& m : g_mods) if (m.name == st) { known = true; break; }
		if (known) continue;
		Mod m; m.name = st;
		if (ends_with(n, ".cpp")) { m.src = dir + "/" + n; m.so = cachedir() + "/" + st + ".so"; }
		else { m.so = dir + "/" + n; }
		if (open_mod(m)) { logline("reload: new mod %s", st.c_str()); if (m.init) m.init(); g_mods.push_back(m); }
	}
	closedir(d);
}

// ---- config (mods/<mod>.cfg, key=value) -----------------------------------
std::map<std::string, std::map<std::string, std::string>> g_cfg;
std::string trim(const std::string& s) {
	size_t a = s.find_first_not_of(" \t\r\n"), b = s.find_last_not_of(" \t\r\n");
	return a == std::string::npos ? std::string() : s.substr(a, b - a + 1);
}
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

constexpr unsigned SDL_KEYDOWN = 0x300, SDL_KEYUP = 0x301;
struct KeyView { unsigned type, timestamp, windowID; unsigned char state, repeat, p0, p1;
                 int scancode, sym; unsigned short mod; };

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
}

extern "C" {

void FNA3D_SwapBuffers(void* device, void* src, void* dst, void* window) {
	static void (*real)(void*, void*, void*, void*) = nullptr;
	if (!real) real = (decltype(real))dlsym(RTLD_NEXT, "FNA3D_SwapBuffers");
	if (!g_loaded) { g_loaded = true; load_all(); }
	for (auto& m : g_mods) if (m.update) m.update();
	if (++g_frame % RELOAD_POLL_FRAMES == 0) poll_reload();

	// main-menu banner: "vX.Y.Z, N mods loaded" (bottom-left, small font)
	if (g_loaded && Eets::World_IsInMainMenu()) {
		char banner[128];
		snprintf(banner, sizeof(banner), "v%s, %zu mods loaded", EETSMOD_VERSION, g_mods.size());
		int h = Eets::ScreenHeight(); if (h <= 0) h = 720;
		// half of the original 20px font (= ~10px)
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
		KeyView* k = (KeyView*)event;
		if (k->type == SDL_KEYDOWN || k->type == SDL_KEYUP) {
			int down = (k->type == SDL_KEYDOWN) ? 1 : 0;
			for (auto& m : g_mods) if (m.onkey) m.onkey(k->sym, k->mod, down);
		}
	}
	return r;
}

} // extern "C"
