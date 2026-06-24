// libeetsmod.so
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "compat.h"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <exception>
#include "eets_engine.h"
#include "eetsmod.h"   // EETS_API marks service decls dllexport in the loader (Windows)
#include "hook.h"

#define EETSMOD_VERSION "0.19.1"

// mod native binary ext: .dll on Windows, .so on Linux (.eetsmod bundles carry both)
#ifdef _WIN32
#define MOD_EXT ".dll"
#else
#define MOD_EXT ".so"
#endif

namespace {

typedef void (*InitFn)();
typedef void (*UpdateFn)();
typedef void (*KeyFn)(int, int, int);
typedef void (*MouseFn)(int, int, int, int);
typedef void (*WheelFn)(int, int);
typedef void (*EventFn)(const char*, void*, void*);
typedef void (*TextFn)(const char*);
typedef void (*ShutdownFn)();

struct Mod {
	std::string name;
	std::string src;
	std::string so;
	void*       handle = nullptr;
	time_t      srcmtime = 0;
	bool        disabled = false;
	std::string version, author, minFw;
	int         priority = 0;
	bool        sim = false;          // affects simulation (replays/leaderboard)
	std::vector<std::string> requires_;
	InitFn      init = nullptr;
	UpdateFn    update = nullptr;
	KeyFn       onkey = nullptr;
	MouseFn     onmouse = nullptr;
	WheelFn     onwheel = nullptr;
	EventFn     onevent = nullptr;
	TextFn      ontext = nullptr;
	ShutdownFn  shutdown = nullptr;
};

std::vector<Mod> g_mods;
bool   g_loaded = false;
unsigned long g_frame = 0;
const unsigned RELOAD_POLL_FRAMES = 30;
int g_mouse_x = 0, g_mouse_y = 0;        // in render (viewport) space
bool g_overlay = false;
// main-menu MODS button hit-rect, refreshed each frame drawn (w=0 when hidden)
int g_modbtn_x = 0, g_modbtn_y = 0, g_modbtn_w = 0, g_modbtn_h = 0;
// overlay "open mods folder" button row (y set during draw, -1 when hidden)
int g_folder_btn_y = -1;
// real backbuffer size from FNA3D_SetViewport; differs from window res in fullscreen
int g_vp_w = 0, g_vp_h = 0;              // committed (last full frame)
int g_vp_cur_w = 0, g_vp_cur_h = 0;     // max seen during the current frame
double g_time = 0.0, g_dt = 0.0;
const int OV_X = 8, OV_Y = 8, OV_W = 320, OV_TITLE = 40, OV_ROWH = 26;  // f1 manager layout
std::string g_selected;

// absolute game dir from this .so's own path; never use process cwd (LD_PRELOAD ctor runs before game chdir's)
std::string base() {
	static std::string b;
	if (b.empty()) {
#ifdef _WIN32
		b = eets_self_dir();
#else
		Dl_info info;
		if (dladdr((void*)&base, &info) && info.dli_fname) {
			std::string p = info.dli_fname; size_t s = p.find_last_of('/');
			if (s != std::string::npos) b = p.substr(0, s);
		}
#endif
		if (b.empty()) b = ".";
	}
	return b;
}

FILE* logfile() { static FILE* f = fopen((base() + "/Log/native_mods.log").c_str(), "a"); return f; }
void logline(const char* fmt, ...) {
	char buf[2048];
	va_list ap; va_start(ap, fmt); vsnprintf(buf, sizeof(buf), fmt, ap); va_end(ap);
	fprintf(stderr, "[eetsmod] %s\n", buf);
	FILE* f = logfile(); if (f) { fprintf(f, "%s\n", buf); fflush(f); }
}

// crash guard: a faulting mod is disabled, the game survives
#ifdef _WIN32
// Windows: vectored exception handler longjmps back to the guard on a mod fault
jmp_buf g_jmp;
volatile long g_guarding = 0;
#define EETS_SETJMP() setjmp(g_jmp)
LONG WINAPI crash_veh(EXCEPTION_POINTERS* ep) {
	DWORD c = ep->ExceptionRecord->ExceptionCode;
	bool fatal = c == EXCEPTION_ACCESS_VIOLATION || c == EXCEPTION_ILLEGAL_INSTRUCTION ||
	             c == EXCEPTION_INT_DIVIDE_BY_ZERO || c == EXCEPTION_FLT_DIVIDE_BY_ZERO ||
	             c == EXCEPTION_PRIV_INSTRUCTION || c == EXCEPTION_IN_PAGE_ERROR;
	if (g_guarding && fatal) { g_guarding = 0; longjmp(g_jmp, 1); }
	return EXCEPTION_CONTINUE_SEARCH;   // not in a mod: defer to engine/default handler
}
void install_guards() { AddVectoredExceptionHandler(1, crash_veh); }
#else
sigjmp_buf g_jmp;
volatile sig_atomic_t g_guarding = 0;
struct sigaction g_old_sa[8];
const int g_sigs[] = { SIGSEGV, SIGFPE, SIGILL, SIGBUS };
#define EETS_SETJMP() sigsetjmp(g_jmp, 1)

void crash_handler(int sig, siginfo_t* info, void* uctx) {
	if (g_guarding) { g_guarding = 0; siglongjmp(g_jmp, sig); }
	// not in a mod: chain to engine/default handler
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
#endif
template <class Fn>
bool guard(Mod* m, Fn&& fn) {
	if (m && m->disabled) return false;
	if (EETS_SETJMP() == 0) {
		g_guarding = 1;
		try {
			fn();
		} catch (const std::exception& e) {
			g_guarding = 0;
			if (m) { m->disabled = true; logline("EXCEPTION in mod '%s': %s - disabled", m->name.c_str(), e.what()); }
			return false;
		} catch (...) {
			g_guarding = 0;
			if (m) { m->disabled = true; logline("EXCEPTION in mod '%s' - disabled", m->name.c_str()); }
			return false;
		}
		g_guarding = 0;
		return true;
	}
	g_guarding = 0;
	if (m) { m->disabled = true; logline("CRASH in mod '%s' - disabled", m->name.c_str()); }
	return false;
}

#ifdef _WIN32
void check_buildid() { logline("buildid: Windows PE - skipping GNU BuildID check"); }
#else
struct BuildIdCtx { char hex[64]; bool found; };
int buildid_cb(struct dl_phdr_info* info, size_t, void* data) {
	if (info->dlpi_name && info->dlpi_name[0] != '\0') return 0;  // main exe: empty name
	BuildIdCtx* ctx = (BuildIdCtx*)data;
	for (int i = 0; i < info->dlpi_phnum; i++) {
		const ElfW(Phdr)& ph = info->dlpi_phdr[i];
		if (ph.p_type != PT_NOTE) continue;
		const unsigned char* p = (const unsigned char*)(info->dlpi_addr + ph.p_vaddr);
		const unsigned char* end = p + ph.p_memsz;
		while (p + 12 <= end) {
			uint32_t namesz = *(uint32_t*)p, descsz = *(uint32_t*)(p + 4), type = *(uint32_t*)(p + 8);
			const unsigned char* name = p + 12;
			const unsigned char* desc = name + ((namesz + 3) & ~3u);
			if (type == 3 && namesz == 4 && memcmp(name, "GNU", 3) == 0) {  // NT_GNU_BUILD_ID
				char* o = ctx->hex;
				for (uint32_t k = 0; k < descsz && k < 30; k++) { sprintf(o, "%02x", desc[k]); o += 2; }
				ctx->found = true; return 1;
			}
			p = desc + ((descsz + 3) & ~3u);
		}
	}
	return 0;
}
void check_buildid() {
	BuildIdCtx ctx; ctx.hex[0] = 0; ctx.found = false;
	dl_iterate_phdr(buildid_cb, &ctx);
	if (!ctx.found) { logline("buildid: could not read running BuildID"); return; }
	if (strcmp(ctx.hex, Eets::EXPECTED_BUILDID) == 0) {
		logline("buildid: %s (matches engine bindings)", ctx.hex);
	} else {
		logline("buildid: WARNING running %s != expected %s", ctx.hex, Eets::EXPECTED_BUILDID);
		logline("buildid: engine addresses may be wrong - regenerate gen_engine_header.sh");
	}
}
#endif

std::string modsdir() { const char* e = getenv("EETS_MODS"); return e ? e : base() + "/mods"; }
std::string datadir() { return base() + "/Data"; }
std::string cachedir() { return modsdir() + "/.cache"; }
time_t mtime_of(const std::string& p) { struct stat s; return stat(p.c_str(), &s) == 0 ? s.st_mtime : 0; }
bool   exists(const std::string& p)   { struct stat s; return stat(p.c_str(), &s) == 0; }

std::string loaderdir() {
#ifdef _WIN32
	return eets_self_dir();
#else
	Dl_info info;
	if (dladdr((void*)&logline, &info) && info.dli_fname) {
		std::string p = info.dli_fname;
		size_t slash = p.find_last_of('/');
		if (slash != std::string::npos) return p.substr(0, slash);
	}
	return ".";
#endif
}
std::string includedir() {
	const char* e = getenv("EETS_INCLUDE");
	if (e) return e;
	std::string cand = loaderdir() + "/eetsmod-include";
	if (exists(cand + "/eetsmod.h")) return cand;
	return modsdir() + "/.include";
}
const char* compiler() { const char* e = getenv("CXX"); return (e && *e) ? e : "g++"; }
std::string trim(const std::string& s);

// last compiler diagnostic (first error line), surfaced in-game on a failed reload
std::string g_last_compile_err;

bool compile(const std::string& src, const std::string& so) {
	const char* dbg = getenv("EETSMOD_DEBUG");
	const char* extra = getenv("EETSMOD_CXXFLAGS");
	std::string opt = (dbg && *dbg && strcmp(dbg, "0") != 0) ? "-g -O0" : "-O2";
	std::string cmd = std::string(compiler()) + " " + opt +
		" -fPIC -std=c++17 -Wall -shared -I\"" + includedir() + "\" " +
		(extra ? extra : "") + " -o \"" + so + "\" \"" + src + "\" 2>&1";
	logline("compile: %s%s", src.c_str(), (dbg && *dbg) ? " [debug]" : "");
	g_last_compile_err.clear();
	FILE* p = popen(cmd.c_str(), "r");
	if (!p) { logline("compile FAILED: cannot run %s", compiler()); g_last_compile_err = "compiler not found"; return false; }
	char line[2048];
	FILE* lf = logfile();
	while (fgets(line, sizeof(line), p)) {
		if (lf) fputs(line, lf);
		if (g_last_compile_err.empty() && strstr(line, "error:")) {
			g_last_compile_err = trim(line);
			if (g_last_compile_err.size() > 90) g_last_compile_err.resize(90);
		}
	}
	if (lf) fflush(lf);
	int rc = pclose(p);
	if (rc != 0) {
		if (g_last_compile_err.empty()) g_last_compile_err = "compile failed (see Log/native_mods.log)";
		logline("compile FAILED (%d): %s", rc, src.c_str());
		return false;
	}
	return true;
}

void resolve(Mod& m) {
	m.init     = (InitFn)    dlsym(m.handle, "EetsMod_Init");
	m.update   = (UpdateFn)  dlsym(m.handle, "EetsMod_Update");
	m.onkey    = (KeyFn)     dlsym(m.handle, "EetsMod_OnKey");
	m.onmouse  = (MouseFn)   dlsym(m.handle, "EetsMod_OnMouse");
	m.onwheel  = (WheelFn)   dlsym(m.handle, "EetsMod_OnWheel");
	m.onevent  = (EventFn)   dlsym(m.handle, "EetsMod_OnEvent");
	m.ontext   = (TextFn)    dlsym(m.handle, "EetsMod_OnText");
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

std::map<std::string, std::map<std::string, std::string>> g_cfg;
// maps a bundled mod's name to its staged .cfg path (loose mods fall back to mods/<name>.cfg)
std::map<std::string, std::string> g_cfgpath;
std::string cfg_path(const std::string& mod) {
	auto it = g_cfgpath.find(mod);
	return it != g_cfgpath.end() ? it->second : modsdir() + "/" + mod + ".cfg";
}
std::map<std::string, std::string>& cfg_for(const char* mod) {
	auto it = g_cfg.find(mod); if (it != g_cfg.end()) return it->second;
	auto& m = g_cfg[mod];
	std::string path = cfg_path(mod);
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
void cfg_flush(const std::string& mod) {
	auto it = g_cfg.find(mod); if (it == g_cfg.end()) return;
	FILE* f = fopen(cfg_path(mod).c_str(), "w");
	if (!f) return;
	for (auto& kv : it->second) fprintf(f, "%s = %s\n", kv.first.c_str(), kv.second.c_str());
	fclose(f);
}
// manifest/identity keys are mod metadata read by read_manifest (not user-tunable settings); the config
// editor must never expose them - e.g. flipping `sim` would change the mod's simulation flag on next load.
bool is_reserved_key(const std::string& k) {
	return k == "version" || k == "author" || k == "min_framework" ||
	       k == "sim" || k == "priority" || k == "requires";
}
std::string cfg_adjust(const std::string& v, int dir) {
	if (v == "0" || v == "1") return v == "0" ? "1" : "0";
	char* end = nullptr; double d = strtod(v.c_str(), &end);
	if (end && *end == '\0') {
		bool isf = v.find('.') != std::string::npos;
		d += dir * (isf ? 0.5 : 1.0);
		char b[48];
		if (isf) snprintf(b, sizeof(b), "%g", d);
		else     snprintf(b, sizeof(b), "%ld", (long)d);
		return b;
	}
	return v;
}

void read_manifest(Mod& m) {
	auto& c = cfg_for(m.name.c_str());
	auto get = [&](const char* k)->std::string{ auto i=c.find(k); return i!=c.end()?i->second:std::string(); };
	m.version = get("version"); m.author = get("author"); m.minFw = get("min_framework");
	std::string pr = get("priority"); if (!pr.empty()) m.priority = atoi(pr.c_str());
	std::string sm = get("sim"); m.sim = (sm == "1" || sm == "true");
	std::string req = get("requires");
	size_t p = 0;
	while (p < req.size()) {
		size_t c2 = req.find(',', p);
		std::string one = trim(req.substr(p, c2 == std::string::npos ? std::string::npos : c2 - p));
		if (!one.empty()) m.requires_.push_back(one);
		if (c2 == std::string::npos) break;
		p = c2 + 1;
	}
}
bool have_mod(const std::string& name) {
	for (auto& m : g_mods) if (m.name == name && !m.disabled) return true;
	return false;
}
int vercmp(const std::string& a, const std::string& b) {
	int ai = 0, bi = 0;
	while (ai < (int)a.size() || bi < (int)b.size()) {
		int x = 0, y = 0;
		while (ai < (int)a.size() && a[ai] != '.') x = x*10 + (a[ai]-'0'), ai++;
		while (bi < (int)b.size() && b[bi] != '.') y = y*10 + (b[bi]-'0'), bi++;
		if (x != y) return x - y;
		if (ai < (int)a.size()) ai++;
		if (bi < (int)b.size()) bi++;
	}
	return 0;
}

std::vector<std::string> g_userDisabled;
bool is_user_disabled(const std::string& n) {
	for (auto& d : g_userDisabled) if (d == n) return true;
	return false;
}
void load_disabled() {
	g_userDisabled.clear();
	FILE* f = fopen((modsdir() + "/.disabled").c_str(), "r");
	if (!f) return;
	char line[256];
	while (fgets(line, sizeof(line), f)) { std::string s = trim(line); if (!s.empty()) g_userDisabled.push_back(s); }
	fclose(f);
}
void save_disabled() {
	FILE* f = fopen((modsdir() + "/.disabled").c_str(), "w");
	if (!f) return;
	for (auto& d : g_userDisabled) fprintf(f, "%s\n", d.c_str());
	fclose(f);
}
void set_user_disabled(const std::string& n, bool dis) {
	bool has = is_user_disabled(n);
	if (dis && !has) g_userDisabled.push_back(n);
	else if (!dis && has) { std::vector<std::string> v; for (auto& d : g_userDisabled) if (d != n) v.push_back(d); g_userDisabled = v; }
	save_disabled();
}

void fire_event(const char* name, void* a, void* b) {
	for (auto& m : g_mods) if (m.onevent && !m.disabled)
		guard(&m, [&]{ m.onevent(name, a, b); });
}

// __thiscall member methods (this in ECX on Win32): detours/trampolines must carry ECALL or `this`
// reads off the stack and caller/callee cleanup disagree, corrupting the stack. emotion/goal are
// cdecl luabind free functions (plain). ECALL is empty on Linux (SysV)
typedef void* (ECALL *CreateObjectFn)(void*, const char*, unsigned);
CreateObjectFn orig_CreateObject = nullptr;
void* ECALL det_CreateObject(void* self, const char* name, unsigned layer) {
	void* o = orig_CreateObject(self, name, layer);
	fire_event("object_spawn", o, (void*)name);
	return o;
}
typedef void (ECALL *ThisFn)(void*);
ThisFn orig_LoadWin = nullptr, orig_Reset = nullptr, orig_KillMe = nullptr, orig_EetsDead = nullptr;
typedef void (ECALL *ThisArgFn)(void*, void*);
ThisArgFn orig_Complete = nullptr;
void ECALL det_LoadWin(void* s)  { orig_LoadWin(s);  fire_event("level_load", s, nullptr); }
void ECALL det_Reset(void* s)    { orig_Reset(s);    fire_event("level_reset", s, nullptr); }
void ECALL det_KillMe(void* s)   { fire_event("object_killed", s, nullptr); orig_KillMe(s); }
typedef void (ECALL *ThisIntFn)(void*, int);
ThisIntFn orig_EetsDead2 = nullptr;
void ECALL det_EetsDead(void* s, int arg) { orig_EetsDead2(s, arg); fire_event("eets_death", s, (void*)(intptr_t)arg); }
void ECALL det_Complete(void* s, void* p) { orig_Complete(s, p); fire_event("level_complete", s, p); }
typedef void (*EmotionFn)(unsigned long, unsigned int);
EmotionFn orig_Emotion = nullptr;
void det_Emotion(unsigned long h, unsigned int e) { orig_Emotion(h, e); fire_event("emotion_change", (void*)h, (void*)(unsigned long)e); }
// detour MUST return World_CheckGoal's int, else EAX leaks event-dispatch junk and engine reads "goal reached"
typedef int (*GoalFn)(void*);
GoalFn orig_Goal = nullptr;
int det_Goal(void* o) { int r = orig_Goal(o); fire_event("goal_check", o, nullptr); return r; }
// level WON: Builder::CompleteLevel(Builder*), the win-moment handler (sets the win flag -> WinDialog, tags
// stats). Fires on EVERY win, unlike LevelManager::CompleteLevel which is skipped on already-completed levels.
ThisFn orig_Win = nullptr;
void ECALL det_Win(void* s) { orig_Win(s); fire_event("level_won", s, nullptr); }
// Eets DYING: Creator::StartEetsDeadDialog(Creator*), fired the instant the Eets dies (it pops the death
// dialog + pauses the sim). Fire BEFORE orig so a mod can react before the dialog shows (e.g. a match that
// resets+counts the death instead of ending the round). 'eets_death' (the close handler) only fires after
// the player dismisses that dialog.
ThisFn orig_Dying = nullptr;
void ECALL det_Dying(void* s) { fire_event("eets_dying", s, nullptr); orig_Dying(s); }
// SIM GUARD: Object::UpdatePlaying(Object*, float) runs each object's per-frame Lua. Some levels have buggy
// scripts (e.g. The Merch's reaper calls obj:GetEdibleExtension() on a just-killed -> nil object) that throw
// a Lua error -> uncaught C++ exception -> std::terminate -> the whole game dies. Wrap it so a single bad
// object's update is skipped this frame and the sim/game survive.
typedef void (ECALL *ThisFloatFn)(void*, float);
ThisFloatFn orig_UpdatePlaying = nullptr;
static long g_simGuardCount = 0;
void ECALL det_UpdatePlaying(void* o, float dt) {
	try { orig_UpdatePlaying(o, dt); }
	catch (...) {
		if ((g_simGuardCount++ % 300) == 0)
			logline("sim-guard: caught exception in Object::UpdatePlaying (count=%ld) - skipped object frame", g_simGuardCount);
	}
}
// eets_death prologue isn't relocatable; detect via object_killed of World_GetEets()

void try_hook(const char* name, void* target, void* detour, void** orig) {
	if (eets_hook::install(target, detour, orig)) logline("hook: %s", name);
	else logline("hook: %s NOT installed (prologue not relocatable)", name);
}
void install_engine_event_hooks() {
	try_hook("object_spawn (ObjectMgr::CreateObject)",  (void*)Eets::addr::hook_ObjectMgr_CreateObject,    (void*)det_CreateObject, (void**)&orig_CreateObject);
	try_hook("level_load (Simulator::LoadWinCondition)",(void*)Eets::addr::hook_Simulator_LoadWinCondition, (void*)det_LoadWin,      (void**)&orig_LoadWin);
	try_hook("level_reset (Simulator::ResetSimulation)",(void*)Eets::addr::hook_Simulator_ResetSimulation,  (void*)det_Reset,        (void**)&orig_Reset);
	try_hook("level_complete (LevelManager::Complete)", (void*)Eets::addr::hook_LevelManager_CompleteLevel,  (void*)det_Complete,     (void**)&orig_Complete);
	try_hook("object_killed (Object::KillMe)",          (void*)Eets::addr::hook_Object_KillMe,              (void*)det_KillMe,       (void**)&orig_KillMe);
	try_hook("emotion_change (World_ChangeEmotion)",    (void*)Eets::addr::hook_World_ChangeEmotion,        (void*)det_Emotion,      (void**)&orig_Emotion);
	try_hook("goal_check (World_CheckGoal)",            (void*)Eets::addr::hook_World_CheckGoal,            (void*)det_Goal,         (void**)&orig_Goal);
	try_hook("level_won (Builder::CompleteLevel)",      (void*)Eets::addr::hook_Builder_CompleteLevel,      (void*)det_Win,          (void**)&orig_Win);
	try_hook("eets_dying (Creator::StartEetsDeadDialog)",(void*)Eets::addr::hook_Creator_StartEetsDeadDialog,(void*)det_Dying,       (void**)&orig_Dying);
	try_hook("sim-guard (Object::UpdatePlaying)",       (void*)Eets::addr::hook_Object_UpdatePlaying,       (void*)det_UpdatePlaying,(void**)&orig_UpdatePlaying);
	// eets_death: hook the dialog-close handler OnEndEetsDeadDialog (__thiscall(Creator*, int)), not the opener; fires once per death
	try_hook("eets_death (Creator::OnEndEetsDeadDialog)", (void*)Eets::addr::hook_Creator_OnEndEetsDeadDialog,(void*)det_EetsDead, (void**)&orig_EetsDead2);
	(void)orig_EetsDead;
}

// .eetsmod is a STORED (uncompressed) ustar tar: .so + .dll + .cpp + .cfg + assets/
// (re)extract into a per-bundle staging dir only when newer than its stamp. in-process
// (no tar/rm/cp shell-out) for Linux/compat-layer parity; uncompressed avoids an inflate dep in the 32-bit loader
std::string bundle_dir(const std::string& name) { return cachedir() + "/" + name + ".d"; }

void mkdir_p(const std::string& path) {
	std::string cur;
	for (size_t i = 0; i < path.size(); ++i) {
		cur += path[i];
		if (path[i] == '/' && cur.size() > 1) mkdir(cur.c_str(), 0755);
	}
	mkdir(path.c_str(), 0755);
}
void rm_tree(const std::string& path) {
	DIR* d = opendir(path.c_str());
	if (d) {
		struct dirent* e;
		while ((e = readdir(d)) != nullptr) {
			std::string n = e->d_name;
			if (n == "." || n == "..") continue;
			std::string p = path + "/" + n;
			struct stat st;
			if (stat(p.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) rm_tree(p);
			else remove(p.c_str());
		}
		closedir(d);
	}
	rmdir(path.c_str());
}
void copy_tree(const std::string& src, const std::string& dst) {
	mkdir_p(dst);
	DIR* d = opendir(src.c_str()); if (!d) return;
	struct dirent* e;
	while ((e = readdir(d)) != nullptr) {
		std::string n = e->d_name;
		if (n == "." || n == "..") continue;
		std::string s = src + "/" + n, t = dst + "/" + n;
		struct stat st;
		if (stat(s.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) { copy_tree(s, t); continue; }
		FILE* in = fopen(s.c_str(), "rb"); if (!in) continue;
		FILE* out = fopen(t.c_str(), "wb");
		if (out) { char b[8192]; size_t r; while ((r = fread(b, 1, sizeof(b), in)) > 0) fwrite(b, 1, r, out); fclose(out); }
		fclose(in);
	}
	closedir(d);
}
// extract a STORED (uncompressed) ustar archive into dest. returns false on a bad/compressed archive
bool untar(const std::string& archive, const std::string& dest) {
	FILE* f = fopen(archive.c_str(), "rb"); if (!f) return false;
	unsigned char hdr[512];
	if (fread(hdr, 1, 512, f) != 512) { fclose(f); return false; }
	if (hdr[0] == 0x1f && hdr[1] == 0x8b) { fclose(f); return false; }   // gzipped: needs re-pack with current eetsmod
	bool ok = true;
	do {
		if (hdr[0] == '\0') break;                                       // end-of-archive zero block
		std::string name((char*)hdr, strnlen((char*)hdr, 100));
		if (hdr[345] != '\0') name = std::string((char*)hdr + 345, strnlen((char*)hdr + 345, 155)) + "/" + name;
		unsigned long size = strtoul(std::string((char*)hdr + 124, 12).c_str(), nullptr, 8);
		char type = hdr[156];
		std::string out = dest + "/" + name;
		if (type == '5') {                                              // directory
			mkdir_p(out);
		} else if (type == '0' || type == '\0') {                       // regular file
			size_t slash = out.find_last_of('/');
			if (slash != std::string::npos) mkdir_p(out.substr(0, slash));
			FILE* of = fopen(out.c_str(), "wb");
			unsigned long rem = size; char buf[4096];
			while (rem > 0) {
				size_t chunk = rem < sizeof(buf) ? (size_t)rem : sizeof(buf);
				size_t r = fread(buf, 1, chunk, f);
				if (r == 0) { ok = false; break; }
				if (of) fwrite(buf, 1, r, of);
				rem -= r;
			}
			if (of) fclose(of);
			unsigned long pad = (512 - (size % 512)) % 512;
			if (pad) fseek(f, pad, SEEK_CUR);
		}
	} while (fread(hdr, 1, 512, f) == 512);
	fclose(f);
	return ok;
}

void extract_bundles() {
	std::string dir = modsdir();
	DIR* d = opendir(dir.c_str()); if (!d) return;
	struct dirent* ent;
	while ((ent = readdir(d)) != nullptr) {
		std::string n = ent->d_name;
		if (!ends_with(n, ".eetsmod")) continue;
		std::string name = stem(n), bundle = dir + "/" + n, sdir = bundle_dir(name);
		std::string stamp = sdir + "/.stamp";
		if (exists(stamp) && mtime_of(stamp) >= mtime_of(bundle)) continue;
		rm_tree(sdir);
		mkdir_p(sdir);
		if (!untar(bundle, sdir)) { logline("bundle: extract FAILED %s (bad/compressed archive - re-pack with eetsmod)", n.c_str()); continue; }
		// a bundle's assets/ tree mirrors Data/ and is installed on extract
		std::string adir = sdir + "/assets";
		if (exists(adir)) copy_tree(adir, datadir());
		FILE* f = fopen(stamp.c_str(), "w"); if (f) fclose(f);
		logline("bundle: installed %s", n.c_str());
	}
	closedir(d);
}

// build a Mod from an extracted staging dir; false if it has no .so/.cpp
bool make_bundle_mod(const std::string& name, Mod& m) {
	std::string sdir = bundle_dir(name);
	std::string so = sdir + "/" + name + MOD_EXT, cpp = sdir + "/" + name + ".cpp", cfg = sdir + "/" + name + ".cfg";
	m.name = name;
	if (exists(so)) m.so = so;
	else if (exists(cpp)) { m.src = cpp; m.so = cachedir() + "/" + name + ".so"; }
	else return false;
	if (exists(cfg)) g_cfgpath[name] = cfg;
	return true;
}

// overwrites matching Data/ files (intentional override); in-process copy for Linux/compat-layer parity
void install_assets() {
	std::string adir = modsdir() + "/assets";
	if (!exists(adir)) return;
	copy_tree(adir, datadir());
	logline("assets: installed mods/assets/ into Data/");
}

// must run before the engine's boot scan of Data/ so bundle assets are present on first scan
// Linux: ctor attribute at LD_PRELOAD time. Windows: DllMain calls it (NOT a ctor, would double-init)
#ifndef _WIN32
__attribute__((constructor))
#endif
void eetsmod_preboot() {
	// drop inherited LD_PRELOAD: our system()/popen() children would re-load this .so and re-run the ctor (fork bomb). already mapped, so safe to clear
	unsetenv("LD_PRELOAD");
	mkdir((base() + "/Log").c_str(), 0755);
	mkdir(modsdir().c_str(), 0755);
	mkdir(cachedir().c_str(), 0755);
	extract_bundles();
	install_assets();
}

void load_all() {
	install_guards();
	check_buildid();
	std::string dir = modsdir();
	mkdir(cachedir().c_str(), 0755);
	extract_bundles();          // unpack any .eetsmod before assets/scan
	install_assets();
	load_disabled();
	logline("eetsmod v%s  include=%s compiler=%s", EETSMOD_VERSION, includedir().c_str(), compiler());
	logline("loader: scanning %s", dir.c_str());

	// process .eetsmod first so a bundle wins over a loose file of the same name (one mod per name)
	std::vector<Mod> found;
	std::vector<std::string> names;
	DIR* d = opendir(dir.c_str());
	if (d) {
		struct dirent* ent;
		std::vector<std::string> files;
		while ((ent = readdir(d)) != nullptr) {
			std::string n = ent->d_name;
			if (ends_with(n, ".eetsmod") || ends_with(n, ".cpp") || ends_with(n, MOD_EXT)) files.push_back(n);
		}
		closedir(d);
		std::sort(files.begin(), files.end(), [](const std::string& a, const std::string& b) {
			return ends_with(a, ".eetsmod") && !ends_with(b, ".eetsmod");   // .eetsmod first
		});
		for (auto& n : files) {
			std::string name = stem(n);
			bool dup = false; for (auto& s : names) if (s == name) { dup = true; break; }
			if (dup) { logline("loader: ignoring %s (mod '%s' already provided)", n.c_str(), name.c_str()); continue; }
			Mod m; m.name = name;
			if (ends_with(n, ".eetsmod")) { if (!make_bundle_mod(name, m)) continue; }
			else if (ends_with(n, ".cpp")) { m.src = dir + "/" + n; m.so = cachedir() + "/" + name + MOD_EXT; }
			else { m.so = dir + "/" + n; }
			read_manifest(m);
			names.push_back(name);
			found.push_back(m);
		}
	} else logline("loader: no mods dir (%s)", dir.c_str());

	for (size_t i = 0; i < found.size(); i++)
		for (size_t j = i + 1; j < found.size(); j++)
			if (found[j].priority > found[i].priority) std::swap(found[i], found[j]);

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
		if (is_user_disabled(m.name)) { m.disabled = true; logline("'%s' disabled by user", m.name.c_str()); }
		if (!m.minFw.empty() && vercmp(EETSMOD_VERSION, m.minFw) < 0) {
			m.disabled = true;
			logline("'%s' needs framework >= %s (have %s) - disabled", m.name.c_str(), m.minFw.c_str(), EETSMOD_VERSION);
		}
	}
	for (auto& m : g_mods) {
		for (auto& dep : m.requires_) if (!have_mod(dep)) {
			m.disabled = true;
			logline("dep: '%s' requires missing '%s' - disabled", m.name.c_str(), dep.c_str());
		}
	}

	install_engine_event_hooks();   // 32-bit E9 hook path (hook.h) on Win; unresolved hook RVAs null-guard in install()
	for (auto& m : g_mods) if (m.init && !m.disabled) guard(&m, [&]{ m.init(); });
	int active = 0; for (auto& m : g_mods) if (!m.disabled) active++;
	logline("loader: %d/%zu mod(s) active", active, g_mods.size());
	for (auto& m : g_mods) if (m.sim && !m.disabled)
		logline("integrity: '%s' affects simulation - replays/leaderboards may be invalid", m.name.c_str());
}

// transient on-screen message (reload result, compile errors). g_time set in the swap hook
std::string g_toast; double g_toast_until = 0; bool g_toast_fail = false;
void toast(const std::string& s, bool fail = false) { g_toast = s; g_toast_fail = fail; g_toast_until = g_time + 6.0; }

void poll_reload() {
	std::string dir = modsdir();
	extract_bundles();   // a .eetsmod dropped while running installs on the next poll
	for (auto& m : g_mods) {
		if (m.src.empty()) continue;
		time_t t = mtime_of(m.src);
		if (t != 0 && t != m.srcmtime) {
			logline("reload: %s changed", m.name.c_str());
			close_mod(m);
			m.disabled = false;
			if (open_mod(m)) { toast("reloaded " + m.name); if (m.init) guard(&m, [&]{ m.init(); }); }
			else { m.disabled = true; toast(m.name + ": " + g_last_compile_err, true); }
		}
	}
	DIR* d = opendir(dir.c_str());
	if (!d) return;
	struct dirent* ent;
	while ((ent = readdir(d)) != nullptr) {
		std::string n = ent->d_name;
		std::string st = stem(n);
		if (!ends_with(n, ".cpp") && !ends_with(n, MOD_EXT) && !ends_with(n, ".eetsmod")) continue;
		bool known = false; for (auto& m : g_mods) if (m.name == st) { known = true; break; }
		if (known) continue;
		Mod m; m.name = st;
		if (ends_with(n, ".eetsmod")) { if (!make_bundle_mod(st, m)) continue; }
		else if (ends_with(n, ".cpp")) { m.src = dir + "/" + n; m.so = cachedir() + "/" + st + MOD_EXT; }
		else { m.so = dir + "/" + n; }
		read_manifest(m);
		if (open_mod(m)) { g_mods.push_back(m); logline("reload: new mod %s", st.c_str());
		                   if (g_mods.back().init) guard(&g_mods.back(), [&]{ g_mods.back().init(); }); }
	}
	closedir(d);
}

std::map<std::string, std::map<std::string, std::string>> g_save;
std::map<std::string, std::string>& save_for(const char* mod) {
	auto it = g_save.find(mod); if (it != g_save.end()) return it->second;
	auto& m = g_save[mod];
	FILE* f = fopen((cachedir() + "/" + mod + ".save").c_str(), "r");
	if (f) {
		char line[1024];
		while (fgets(line, sizeof(line), f)) {
			std::string s = trim(line);
			if (s.empty() || s[0] == '#') continue;
			size_t eq = s.find('='); if (eq == std::string::npos) continue;
			m[trim(s.substr(0, eq))] = trim(s.substr(eq + 1));
		}
		fclose(f);
	}
	return m;
}
void save_flush(const char* mod) {
	FILE* f = fopen((cachedir() + "/" + mod + ".save").c_str(), "w");
	if (!f) return;
	fprintf(f, "# eetsmod save data for '%s'\n", mod);
	for (auto& kv : save_for(mod)) fprintf(f, "%s = %s\n", kv.first.c_str(), kv.second.c_str());
	fclose(f);
}

constexpr unsigned SDL_KEYDOWN = 0x300, SDL_KEYUP = 0x301, SDL_TEXTINPUT = 0x303;
constexpr unsigned SDL_MOUSEMOTION = 0x400, SDL_MOUSEBUTTONDOWN = 0x401,
                   SDL_MOUSEBUTTONUP = 0x402, SDL_MOUSEWHEEL = 0x403;
struct KeyView { unsigned type, ts, win; unsigned char state, repeat, p0, p1; int scancode, sym; unsigned short mod; };
struct MotionView { unsigned type, ts, win, which, state; int x, y, xrel, yrel; };
struct ButtonView { unsigned type, ts, win, which; unsigned char button, state, clicks, pad; int x, y; };
struct WheelView { unsigned type, ts, win, which; int x, y; unsigned dir; };

void open_mods_folder() {
	std::string md = modsdir(); std::string abs = md;
	if (!md.empty() && md[0] != '/') { char cwd[4096]; if (getcwd(cwd, sizeof(cwd))) abs = std::string(cwd) + "/" + md; }
	system(("xdg-open \"" + abs + "\" >/dev/null 2>&1 &").c_str());
	logline("manager: opened mods folder %s", abs.c_str());
}

bool manage_click(int mx, int my) {
	// close button (x) in the title bar
	if (mx >= OV_X + OV_W - 32 && mx < OV_X + OV_W - 6 && my >= OV_Y + 7 && my < OV_Y + 33) {
		g_overlay = false; return true;
	}
	// "open mods folder" button
	if (g_folder_btn_y >= 0 && mx >= OV_X + 6 && mx < OV_X + OV_W - 6 &&
	    my >= g_folder_btn_y && my < g_folder_btn_y + OV_ROWH) { open_mods_folder(); return true; }
	if (mx < OV_X || mx >= OV_X + OV_W) return false;
	int nMods = (int)g_mods.size();
	int rel = my - (OV_Y + OV_TITLE);
	if (rel < 0) return my >= OV_Y;                 // title bar: consume
	int i = rel / OV_ROWH;
	if (i >= 0 && i < nMods) {
		Mod& m = g_mods[i];
		if (mx < OV_X + 58) {                       // ON/OFF zone -> enable toggle
			bool dis = !m.disabled; m.disabled = dis; set_user_disabled(m.name, dis);
			logline("manager: %s %s", m.name.c_str(), dis ? "disabled" : "enabled");
			if (!dis && m.init) guard(&m, [&]{ m.init(); });
		} else {                                    // name -> expand config
			g_selected = (g_selected == m.name) ? std::string() : m.name;
			if (!g_selected.empty()) cfg_for(g_selected.c_str());
		}
		return true;
	}
	if (g_selected.empty()) return true;
	auto& cfg = g_cfg[g_selected];
	int cy0 = OV_Y + OV_TITLE + nMods * OV_ROWH;
	int row = (my - cy0) / OV_ROWH - 1;             // -1: first row is the "config:" header
	int k = 0;
	for (auto& kv : cfg) {
		if (is_reserved_key(kv.first)) continue;   // manifest keys are hidden, so they take no click rows
		if (k == row) {
			int minusX = OV_X + OV_W - 70, plusX = OV_X + OV_W - 34;
			if (mx >= plusX) kv.second = cfg_adjust(kv.second, +1);
			else if (mx >= minusX) kv.second = cfg_adjust(kv.second, -1);
			else return true;
			cfg_flush(g_selected);
			logline("config: %s.%s = %s", g_selected.c_str(), kv.first.c_str(), kv.second.c_str());
			return true;
		}
		k++;
	}
	return true;
}

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
	int   RenderWidth()  { return g_vp_w > 0 ? g_vp_w : ScreenWidth(); }
	int   RenderHeight() { return g_vp_h > 0 ? g_vp_h : ScreenHeight(); }
	const char* SaveGet(const char* mod, const char* key, const char* def) {
		auto& m = save_for(mod); auto it = m.find(key);
		return it != m.end() ? it->second.c_str() : def;
	}
	void SaveSet(const char* mod, const char* key, const char* val) {
		save_for(mod)[key] = val ? val : ""; save_flush(mod);
	}
	int   SaveGetInt(const char* mod, const char* key, int def)   { const char* v = SaveGet(mod, key, nullptr); return v ? atoi(v) : def; }
	void  SaveSetInt(const char* mod, const char* key, int v)     { char b[32]; snprintf(b, sizeof(b), "%d", v); SaveSet(mod, key, b); }
	float SaveGetFloat(const char* mod, const char* key, float d) { const char* v = SaveGet(mod, key, nullptr); return v ? (float)atof(v) : d; }
	void  SaveSetFloat(const char* mod, const char* key, float v) { char b[48]; snprintf(b, sizeof(b), "%g", v); SaveSet(mod, key, b); }
	double Time()      { return g_time; }
	double DeltaTime() { return g_dt; }
	void StartTextInput() { auto f = (void(*)())dlsym(RTLD_NEXT, "SDL_StartTextInput"); if (f) f(); }
	void StopTextInput()  { auto f = (void(*)())dlsym(RTLD_NEXT, "SDL_StopTextInput");  if (f) f(); }
}

extern "C" {

// capture the real backbuffer size (viewport); correct in fullscreen
void FNA3D_SetViewport(void* device, void* viewport) {
	static void (*real)(void*, void*) = nullptr;
	if (!real) real = (decltype(real))dlsym(RTLD_NEXT, "FNA3D_SetViewport");
	int w = ((int*)viewport)[2], h = ((int*)viewport)[3];   // {x,y,w,h,...}
	if ((long)w * h > (long)g_vp_cur_w * g_vp_cur_h) { g_vp_cur_w = w; g_vp_cur_h = h; }
	if (real) real(device, viewport);
}

void FNA3D_SwapBuffers(void* device, void* src, void* dst, void* window) {
	static void (*real)(void*, void*, void*, void*) = nullptr;
	if (!real) real = (decltype(real))dlsym(RTLD_NEXT, "FNA3D_SwapBuffers");
	if (!g_loaded) { g_loaded = true; load_all(); }
	{
		struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
		double now = ts.tv_sec + ts.tv_nsec * 1e-9;
		static double start = now, last = now;
		g_dt = now - last; last = now; g_time = now - start;
	}
	if (g_vp_cur_w > 0) { g_vp_w = g_vp_cur_w; g_vp_h = g_vp_cur_h; }
	{ static bool vlog = false; if (!vlog && g_vp_w > 0) { vlog = true; logline("render dims: %dx%d (configured %dx%d)", g_vp_w, g_vp_h, Eets::ScreenWidth(), Eets::ScreenHeight()); } }
	for (auto& m : g_mods) if (m.update && !m.disabled) guard(&m, [&]{ m.update(); });
	if (++g_frame % RELOAD_POLL_FRAMES == 0) poll_reload();

	if (g_loaded && Eets::World_IsInMainMenu()) {
		using namespace Eets;
		size_t active = 0; for (auto& m : g_mods) if (!m.disabled) active++;
		int h = RenderHeight(); if (h <= 0) h = 720;
		// clickable bottom-left MODS button: opens the manager overlay (no F1 needed)
		g_modbtn_x = 10; g_modbtn_w = 150; g_modbtn_h = 38; g_modbtn_y = h - g_modbtn_h - 10;
		bool hot = g_mouse_x >= g_modbtn_x && g_mouse_x < g_modbtn_x + g_modbtn_w &&
		           g_mouse_y >= g_modbtn_y && g_mouse_y < g_modbtn_y + g_modbtn_h;
		FillRect(g_modbtn_x + 3, g_modbtn_y + 3, g_modbtn_w, g_modbtn_h, Color(0, 0, 0, 110));
		FillRect(g_modbtn_x, g_modbtn_y, g_modbtn_w, g_modbtn_h, hot ? Color(235, 70, 60, 255) : Color(205, 40, 35, 255));
		DrawRect(g_modbtn_x, g_modbtn_y, g_modbtn_w, g_modbtn_h, Color(0, 0, 0, 255), 3.0f);
		char btn[64]; snprintf(btn, sizeof(btn), "MODS (%zu)", active);
		DrawTextOutlined(g_modbtn_x + 12, g_modbtn_y + 8, btn, FONT_BIG, Color(255, 232, 40, 255));
		static bool once = false;
		if (!once) { once = true; logline("menu MODS button at %d,%d (screen h=%d)", g_modbtn_x, g_modbtn_y, h); }
	} else { g_modbtn_w = 0; }

	if (g_loaded && !g_toast.empty() && g_time < g_toast_until) {
		using namespace Eets;
		DrawTextOutlined(10, 10, g_toast.c_str(), FONT_NORMAL,
		                 g_toast_fail ? Color(255, 90, 80, 255) : Color(120, 255, 120, 255));
	}

	if (g_loaded && g_overlay) {
		using namespace Eets;
		int nMods = (int)g_mods.size();
		int cfgRows = 0;
		if (!g_selected.empty()) { cfgRows = 1; for (auto& kv : g_cfg[g_selected]) if (!is_reserved_key(kv.first)) cfgRows++; }   // +1 header, manifest keys hidden
		int H = OV_TITLE + nMods * OV_ROWH + cfgRows * OV_ROWH + OV_ROWH + 18;  // +1 row for the folder button
		FillRect(OV_X + 5, OV_Y + 6, OV_W, H, Color(0, 0, 0, 110));
		FillRect(OV_X, OV_Y, OV_W, H, Color(205, 40, 35, 255));
		DrawRect(OV_X, OV_Y, OV_W, H, Color(0, 0, 0, 255), 4.0f);
		FillRect(OV_X + 4, OV_Y + 4, OV_W - 8, OV_TITLE - 6, Color(165, 22, 20, 255));
		char line[160];
		snprintf(line, sizeof(line), "MODS v%s", EETSMOD_VERSION);
		DrawTextOutlined(OV_X + 10, OV_Y + 8, line, FONT_BIG, Color(255, 232, 40, 255));
		FillRect(OV_X + OV_W - 32, OV_Y + 7, 26, 26, Color(165, 22, 20, 255));
		DrawRect(OV_X + OV_W - 32, OV_Y + 7, 26, 26, Color(0, 0, 0, 255), 2.0f);
		DrawTextOutlined(OV_X + OV_W - 26, OV_Y + 8, "x", FONT_BIG, Color(255, 232, 40, 255));
		int y = OV_Y + OV_TITLE;
		for (auto& m : g_mods) {
			bool sel = (g_selected == m.name);
			if (sel) FillRect(OV_X + 4, y, OV_W - 8, OV_ROWH, Color(255, 120, 55, 160));
			DrawTextOutlined(OV_X + 10, y + 4, m.disabled ? "OFF" : "ON",
			                 FONT_NORMAL, m.disabled ? Color(255, 90, 80, 255) : Color(255, 210, 40, 255));
			snprintf(line, sizeof(line), "%s%s", m.name.c_str(), m.version.empty() ? "" : (" v" + m.version).c_str());
			DrawTextOutlined(OV_X + 64, y + 4, line, FONT_NORMAL, Color(255, 255, 255, 255));
			y += OV_ROWH;
		}
		if (!g_selected.empty()) {
			DrawTextOutlined(OV_X + 10, y + 4, ("config: " + g_selected).c_str(), FONT_SMALL, Color(255, 232, 40, 255));
			y += OV_ROWH;
			for (auto& kv : g_cfg[g_selected]) {
				if (is_reserved_key(kv.first)) continue;   // manifest/identity keys are not user-tunable
				snprintf(line, sizeof(line), "%s = %s", kv.first.c_str(), kv.second.c_str());
				DrawTextOutlined(OV_X + 14, y + 4, line, FONT_SMALL, Color(255, 255, 255, 255));
				FillRect(OV_X + OV_W - 70, y + 2, 28, OV_ROWH - 6, Color(70, 50, 60, 255));
				DrawRect(OV_X + OV_W - 70, y + 2, 28, OV_ROWH - 6, Color(0, 0, 0, 255), 2.0f);
				DrawTextOutlined(OV_X + OV_W - 63, y + 4, "-", FONT_NORMAL, Color(255, 255, 255, 255));
				FillRect(OV_X + OV_W - 34, y + 2, 28, OV_ROWH - 6, Color(70, 50, 60, 255));
				DrawRect(OV_X + OV_W - 34, y + 2, 28, OV_ROWH - 6, Color(0, 0, 0, 255), 2.0f);
				DrawTextOutlined(OV_X + OV_W - 28, y + 4, "+", FONT_NORMAL, Color(255, 255, 255, 255));
				y += OV_ROWH;
			}
		}
		// open-mods-folder button: where players drop a .eetsmod to install one
		y += 6; g_folder_btn_y = y;
		bool fhot = g_mouse_x >= OV_X + 6 && g_mouse_x < OV_X + OV_W - 6 &&
		            g_mouse_y >= y && g_mouse_y < y + OV_ROWH;
		FillRect(OV_X + 6, y, OV_W - 12, OV_ROWH, fhot ? Color(235, 70, 60, 255) : Color(165, 22, 20, 255));
		DrawRect(OV_X + 6, y, OV_W - 12, OV_ROWH, Color(0, 0, 0, 255), 2.0f);
		DrawTextOutlined(OV_X + 14, y + 4, "+ Add a mod (open mods folder)", FONT_SMALL, Color(255, 232, 40, 255));
	}
	g_vp_cur_w = g_vp_cur_h = 0;
	if (real) real(device, src, dst, window);
}

void map_mouse(int rx, int ry, unsigned winId) {
	static void* (*getWin)(unsigned) = nullptr;
	static void  (*getSize)(void*, int*, int*) = nullptr;
	if (!getWin)  getWin  = (decltype(getWin)) dlsym(RTLD_NEXT, "SDL_GetWindowFromID");
	if (!getSize) getSize = (decltype(getSize))dlsym(RTLD_NEXT, "SDL_GetWindowSize");
	int ww = 0, wh = 0;
	if (getWin && getSize) { void* w = getWin(winId); if (w) getSize(w, &ww, &wh); }
	if (ww > 0 && wh > 0 && g_vp_w > 0 && g_vp_h > 0) {
		g_mouse_x = (int)((long)rx * g_vp_w / ww);
		g_mouse_y = (int)((long)ry * g_vp_h / wh);
	} else { g_mouse_x = rx; g_mouse_y = ry; }
}

int SDL_PollEvent(void* event) {
	static int (*real)(void*) = nullptr;
	if (!real) real = (decltype(real))dlsym(RTLD_NEXT, "SDL_PollEvent");
	int r = real ? real(event) : 0;
	if (r && event && g_loaded) {
		unsigned type = *(unsigned*)event;
		if (type == SDL_KEYDOWN || type == SDL_KEYUP) {
			KeyView* k = (KeyView*)event; int down = (type == SDL_KEYDOWN) ? 1 : 0;
			if (down && k->sym == 0x4000003A) g_overlay = !g_overlay;   // f1 toggles overlay
			for (auto& m : g_mods) if (m.onkey && !m.disabled) guard(&m, [&]{ m.onkey(k->sym, k->mod, down); });
		} else if (type == SDL_MOUSEMOTION) {
			MotionView* mv = (MotionView*)event; map_mouse(mv->x, mv->y, mv->win);
			int mx = g_mouse_x, my = g_mouse_y;
			for (auto& m : g_mods) if (m.onmouse && !m.disabled) guard(&m, [&]{ m.onmouse(mx, my, -1, 0); });
		} else if (type == SDL_MOUSEBUTTONDOWN || type == SDL_MOUSEBUTTONUP) {
			ButtonView* b = (ButtonView*)event; map_mouse(b->x, b->y, b->win);
			int mx = g_mouse_x, my = g_mouse_y, down = (type == SDL_MOUSEBUTTONDOWN) ? 1 : 0, btn = b->button;
			// MODS button in the main menu opens the manager (consume the click)
			if (down && btn == 1 && g_modbtn_w > 0 &&
			    mx >= g_modbtn_x && mx < g_modbtn_x + g_modbtn_w &&
			    my >= g_modbtn_y && my < g_modbtn_y + g_modbtn_h) { g_overlay = !g_overlay; return r; }
			// manager click (consume on press so it doesn't pass to mods/game)
			if (g_overlay && down && btn == 1 && manage_click(mx, my)) { return r; }
			for (auto& m : g_mods) if (m.onmouse && !m.disabled) guard(&m, [&]{ m.onmouse(mx, my, btn, down); });
		} else if (type == SDL_TEXTINPUT) {
			const char* txt = (const char*)event + 12;   // SDL_TextInputEvent.text[32]
			for (auto& m : g_mods) if (m.ontext && !m.disabled) guard(&m, [&]{ m.ontext(txt); });
		} else if (type == SDL_MOUSEWHEEL) {
			WheelView* w = (WheelView*)event;
			for (auto& m : g_mods) if (m.onwheel && !m.disabled) guard(&m, [&]{ m.onwheel(w->x, w->y); });
		}
	}
	return r;
}

} // extern "C"

#ifdef _WIN32
// ============================================================================
// Windows entry path: version.dll proxy exports + IAT hooks + DllMain
//
// Linux interposes FNA3D_SwapBuffers/SDL_PollEvent/FNA3D_SetViewport via LD_PRELOAD
// Windows ships as version.dll: naming our DLL "version" replaces the system one
// process-wide, so we forward EVERY real-version.dll export (not just SDL2's three),
// and redirect the game's FNA3D/SDL calls by rewriting Eets.exe's IAT slots. originals
// stay reachable via dlsym(RTLD_NEXT) (compat.h shims it to GetModuleHandle+GetProcAddress)
// ============================================================================

// ---- 1) version.dll export proxy -------------------------------------------
// load+cache the real System32\version.dll HMODULE once; each export resolved+cached on first call
namespace {
HMODULE real_version_dll() {
	static HMODULE h = []() -> HMODULE {
		char dir[MAX_PATH] = {0};
		UINT n = GetSystemDirectoryA(dir, MAX_PATH);            // e.g. C:\Windows\System32
		std::string path = (n && n < MAX_PATH) ? std::string(dir) : std::string("C:\\Windows\\System32");
		path += "\\version.dll";
		HMODULE m = LoadLibraryA(path.c_str());
		// a Windows compat-layer "version=n,b" override can resolve back to THIS proxy; refuse to forward to self (infinite recursion)
		HMODULE self = nullptr;
		GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		                   (LPCSTR)&real_version_dll, &self);
		if (m == self) { logline("version-proxy: real version.dll resolved to self - refusing (would recurse)"); m = nullptr; }
		else logline("version-proxy: real version.dll %s (%s)", m ? "loaded" : "FAILED to load", path.c_str());
		return m;
	}();
	return h;
}
FARPROC real_version_proc(const char* name) {
	HMODULE h = real_version_dll();
	return h ? GetProcAddress(h, name) : nullptr;
}
} // namespace

// "version" replaces the system DLL process-wide, so EVERY real export must exist here too or an
// unforwarded call (W/Ex variants too) hits a missing function and the compat layer aborts; signatures verbatim
// from <winver.h> so the IAT and forwarded call share one ABI
//
// VERSION_PROXY(ret, name, params, ptypes, args, fail): undecorated (-Wl,--kill-at) WINAPI export that
// resolves+caches "name" in the real version.dll and calls through, returning `fail` if unresolved
//   params : parameter list, verbatim from winver.h (names included)
//   ptypes : the same parameters' TYPES only, for the function-pointer cast
//   args   : parameter names, forwarded in the call
#define VERSION_PROXY(ret, name, params, ptypes, args, fail)                 \
	__declspec(dllexport) ret WINAPI name params {                           \
		typedef ret (WINAPI *Fn) ptypes;                                     \
		static Fn fn = (Fn)real_version_proc(#name);                         \
		return fn ? fn args : (fail);                                        \
	}

extern "C" {

VERSION_PROXY(DWORD, GetFileVersionInfoSizeA,
	(LPCSTR lptstrFilename, LPDWORD lpdwHandle),
	(LPCSTR, LPDWORD),
	(lptstrFilename, lpdwHandle), 0)
VERSION_PROXY(DWORD, GetFileVersionInfoSizeW,
	(LPCWSTR lptstrFilename, LPDWORD lpdwHandle),
	(LPCWSTR, LPDWORD),
	(lptstrFilename, lpdwHandle), 0)
VERSION_PROXY(DWORD, GetFileVersionInfoSizeExA,
	(DWORD dwFlags, LPCSTR lpwstrFilename, LPDWORD lpdwHandle),
	(DWORD, LPCSTR, LPDWORD),
	(dwFlags, lpwstrFilename, lpdwHandle), 0)
VERSION_PROXY(DWORD, GetFileVersionInfoSizeExW,
	(DWORD dwFlags, LPCWSTR lpwstrFilename, LPDWORD lpdwHandle),
	(DWORD, LPCWSTR, LPDWORD),
	(dwFlags, lpwstrFilename, lpdwHandle), 0)

VERSION_PROXY(BOOL, GetFileVersionInfoA,
	(LPCSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData),
	(LPCSTR, DWORD, DWORD, LPVOID),
	(lptstrFilename, dwHandle, dwLen, lpData), FALSE)
VERSION_PROXY(BOOL, GetFileVersionInfoW,
	(LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData),
	(LPCWSTR, DWORD, DWORD, LPVOID),
	(lptstrFilename, dwHandle, dwLen, lpData), FALSE)
VERSION_PROXY(BOOL, GetFileVersionInfoExA,
	(DWORD dwFlags, LPCSTR lpwstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData),
	(DWORD, LPCSTR, DWORD, DWORD, LPVOID),
	(dwFlags, lpwstrFilename, dwHandle, dwLen, lpData), FALSE)
VERSION_PROXY(BOOL, GetFileVersionInfoExW,
	(DWORD dwFlags, LPCWSTR lpwstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData),
	(DWORD, LPCWSTR, DWORD, DWORD, LPVOID),
	(dwFlags, lpwstrFilename, dwHandle, dwLen, lpData), FALSE)

VERSION_PROXY(BOOL, VerQueryValueA,
	(LPCVOID pBlock, LPCSTR lpSubBlock, LPVOID* lplpBuffer, PUINT puLen),
	(LPCVOID, LPCSTR, LPVOID*, PUINT),
	(pBlock, lpSubBlock, lplpBuffer, puLen), FALSE)
VERSION_PROXY(BOOL, VerQueryValueW,
	(LPCVOID pBlock, LPCWSTR lpSubBlock, LPVOID* lplpBuffer, PUINT puLen),
	(LPCVOID, LPCWSTR, LPVOID*, PUINT),
	(pBlock, lpSubBlock, lplpBuffer, puLen), FALSE)

VERSION_PROXY(DWORD, VerLanguageNameA,
	(DWORD wLang, LPSTR szLang, DWORD nSize),
	(DWORD, LPSTR, DWORD),
	(wLang, szLang, nSize), 0)
VERSION_PROXY(DWORD, VerLanguageNameW,
	(DWORD wLang, LPWSTR szLang, DWORD nSize),
	(DWORD, LPWSTR, DWORD),
	(wLang, szLang, nSize), 0)

VERSION_PROXY(DWORD, VerInstallFileA,
	(DWORD uFlags, LPSTR szSrcFileName, LPSTR szDestFileName, LPSTR szSrcDir, LPSTR szDestDir, LPSTR szCurDir, LPSTR szTmpFile, PUINT lpuTmpFileLen),
	(DWORD, LPSTR, LPSTR, LPSTR, LPSTR, LPSTR, LPSTR, PUINT),
	(uFlags, szSrcFileName, szDestFileName, szSrcDir, szDestDir, szCurDir, szTmpFile, lpuTmpFileLen), 0)
VERSION_PROXY(DWORD, VerInstallFileW,
	(DWORD uFlags, LPWSTR szSrcFileName, LPWSTR szDestFileName, LPWSTR szSrcDir, LPWSTR szDestDir, LPWSTR szCurDir, LPWSTR szTmpFile, PUINT lpuTmpFileLen),
	(DWORD, LPWSTR, LPWSTR, LPWSTR, LPWSTR, LPWSTR, LPWSTR, PUINT),
	(uFlags, szSrcFileName, szDestFileName, szSrcDir, szDestDir, szCurDir, szTmpFile, lpuTmpFileLen), 0)

VERSION_PROXY(DWORD, VerFindFileA,
	(DWORD uFlags, LPSTR szFileName, LPSTR szWinDir, LPSTR szAppDir, LPSTR szCurDir, PUINT lpuCurDirLen, LPSTR szDestDir, PUINT lpuDestDirLen),
	(DWORD, LPSTR, LPSTR, LPSTR, LPSTR, PUINT, LPSTR, PUINT),
	(uFlags, szFileName, szWinDir, szAppDir, szCurDir, lpuCurDirLen, szDestDir, lpuDestDirLen), 0)
VERSION_PROXY(DWORD, VerFindFileW,
	(DWORD uFlags, LPWSTR szFileName, LPWSTR szWinDir, LPWSTR szAppDir, LPWSTR szCurDir, PUINT lpuCurDirLen, LPWSTR szDestDir, PUINT lpuDestDirLen),
	(DWORD, LPWSTR, LPWSTR, LPWSTR, LPWSTR, PUINT, LPWSTR, PUINT),
	(uFlags, szFileName, szWinDir, szAppDir, szCurDir, lpuCurDirLen, szDestDir, lpuDestDirLen), 0)

} // extern "C"
#undef VERSION_PROXY

// ---- 2) IAT hook installer -------------------------------------------------
namespace {

// overwrite one IAT slot, flipping the page writable around the store; false only if VirtualProtect refuses
bool patch_iat_slot(void** slot, void* target) {
	DWORD old = 0;
	if (!VirtualProtect(slot, sizeof(void*), PAGE_READWRITE, &old)) return false;
	*slot = target;
	DWORD tmp = 0;
	VirtualProtect(slot, sizeof(void*), old, &tmp);
	return true;
}

// walk the host EXE's import directory; match on the INT (OriginalFirstThunk) name array,
// repoint the parallel IAT (FirstThunk) slot at our detour. ordinal imports skipped; a name
// not found is fine (e.g. FNA3D_SetViewport may not be imported by every build)
void install_iat_hooks() {
	struct Target { const char* name; void* detour; bool found; };
	Target targets[] = {
		{ "FNA3D_SwapBuffers", (void*)&FNA3D_SwapBuffers, false },
		{ "SDL_PollEvent",     (void*)&SDL_PollEvent,     false },
		{ "FNA3D_SetViewport", (void*)&FNA3D_SetViewport, false },
	};

	BYTE* base = (BYTE*)GetModuleHandleA(NULL);
	if (!base) { logline("iat: GetModuleHandleA(NULL) failed - no hooks installed"); return; }

	IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)base;
	if (dos->e_magic != IMAGE_DOS_SIGNATURE) { logline("iat: bad DOS header - no hooks"); return; }
	IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);
	if (nt->Signature != IMAGE_NT_SIGNATURE) { logline("iat: bad NT header - no hooks"); return; }

	IMAGE_DATA_DIRECTORY& impDir = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	if (!impDir.VirtualAddress || !impDir.Size) { logline("iat: no import directory - no hooks"); return; }

	// import-descriptor array (one per imported DLL) is NUL-terminated
	for (IMAGE_IMPORT_DESCRIPTOR* imp = (IMAGE_IMPORT_DESCRIPTOR*)(base + impDir.VirtualAddress);
	     imp->Name; ++imp) {
		// INT (names) and IAT (live pointers) advance in lockstep; OriginalFirstThunk can be 0 (fall back to FirstThunk)
		IMAGE_THUNK_DATA* nameThunk = (IMAGE_THUNK_DATA*)(base + (imp->OriginalFirstThunk ? imp->OriginalFirstThunk : imp->FirstThunk));
		IMAGE_THUNK_DATA* iatThunk  = (IMAGE_THUNK_DATA*)(base + imp->FirstThunk);
		for (; nameThunk->u1.AddressOfData; ++nameThunk, ++iatThunk) {
			if (nameThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG) continue;   // imported by ordinal: no name
			IMAGE_IMPORT_BY_NAME* byName = (IMAGE_IMPORT_BY_NAME*)(base + nameThunk->u1.AddressOfData);
			for (Target& t : targets) {
				if (t.found || strcmp((const char*)byName->Name, t.name) != 0) continue;
				if (patch_iat_slot((void**)&iatThunk->u1.Function, t.detour)) {
					t.found = true;
					logline("iat: hooked %s", t.name);
				} else {
					logline("iat: VirtualProtect failed for %s - left unhooked", t.name);
				}
			}
		}
	}

	for (Target& t : targets)
		if (!t.found) logline("iat: %s not imported by host - skipped", t.name);
}

} // namespace

// ---- 3) DllMain ------------------------------------------------------------
// DllMain must do almost nothing: eetsmod_preboot() shells out (system()/popen()), and spawning a
// process under the held loader lock deadlocks (esp. under a compat layer), so it only starts a worker thread that
// runs after the lock releases, waits for the render/input DLLs to be mapped (IAT slots bound),
// then stages assets and installs hooks
DWORD WINAPI eets_init_thread(LPVOID) {
	for (int i = 0; i < 400; i++) {            // up to ~20s for FNA3D + SDL2 to load
		if (GetModuleHandleA("FNA3D.dll") && GetModuleHandleA("SDL2.dll")) break;
		Sleep(50);
	}
	eetsmod_preboot();
	install_iat_hooks();
	return 0;
}
BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hinst);
		CreateThread(nullptr, 0, eets_init_thread, nullptr, 0, nullptr);
	}
	return TRUE;
}
#endif // _WIN32
