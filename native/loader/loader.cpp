// libeetsmod.so - native mod loader for Eets (LD_PRELOAD).
//
// Strategy: Eets links FNA3D and SDL2 dynamically, so calls to them go through
// the PLT and CAN be interposed (unlike the engine's internal, statically-linked
// Lua calls). We interpose:
//   FNA3D_SwapBuffers  -> per-frame tick (dlopen mods on first call; Update each)
//   SDL_PollEvent      -> keyboard events dispatched to mods
// Mods call engine functions directly via fixed addresses (eets_engine.h).
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>
#include <dirent.h>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace {

typedef void (*InitFn)();
typedef void (*UpdateFn)();
typedef void (*KeyFn)(int, int, int);

struct Mod {
	std::string name;
	void* handle;
	InitFn   init;
	UpdateFn update;
	KeyFn    onkey;
};

std::vector<Mod> g_mods;
bool g_loaded = false;

FILE* logfile() {
	static FILE* f = nullptr;
	if (!f) { f = fopen("Log/native_mods.log", "a"); }
	return f;
}

void logline(const char* fmt, ...) {
	char buf[1024];
	va_list ap; va_start(ap, fmt); vsnprintf(buf, sizeof(buf), fmt, ap); va_end(ap);
	fprintf(stderr, "[eetsmod] %s\n", buf);
	FILE* f = logfile();
	if (f) { fprintf(f, "%s\n", buf); fflush(f); }
}

std::string modsdir() {
	const char* e = getenv("EETS_MODS");
	return e ? std::string(e) : std::string("mods");
}

void load_mods() {
	std::string dir = modsdir();
	logline("loader: scanning %s for *.so", dir.c_str());
	DIR* d = opendir(dir.c_str());
	if (!d) { logline("loader: no mods dir (%s)", dir.c_str()); return; }
	struct dirent* ent;
	while ((ent = readdir(d)) != nullptr) {
		std::string n = ent->d_name;
		if (n.size() < 4 || n.substr(n.size() - 3) != ".so") continue;
		std::string path = dir + "/" + n;
		void* h = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL);
		if (!h) { logline("loader: dlopen failed %s: %s", path.c_str(), dlerror()); continue; }
		Mod m;
		m.name   = n;
		m.handle = h;
		m.init   = (InitFn)  dlsym(h, "EetsMod_Init");
		m.update = (UpdateFn)dlsym(h, "EetsMod_Update");
		m.onkey  = (KeyFn)   dlsym(h, "EetsMod_OnKey");
		g_mods.push_back(m);
		logline("loader: loaded %s (init=%d update=%d onkey=%d)", n.c_str(),
		        m.init ? 1 : 0, m.update ? 1 : 0, m.onkey ? 1 : 0);
	}
	closedir(d);
	for (auto& m : g_mods) if (m.init) m.init();
	logline("loader: %zu mod(s) active", g_mods.size());
}

// Minimal SDL key-event view (avoids needing SDL headers).
constexpr unsigned SDL_KEYDOWN = 0x300, SDL_KEYUP = 0x301;
struct KeyView {            // overlay onto the start of SDL_Event for key events
	unsigned type;          // +0
	unsigned timestamp;     // +4
	unsigned windowID;      // +8
	unsigned char state;    // +12
	unsigned char repeat;   // +13
	unsigned char pad0, pad1;
	int   scancode;         // +16
	int   sym;              // +20  (SDL_Keycode)
	unsigned short mod;     // +24  (KMOD_*)
};

} // namespace

namespace Eets {
	void Log(const char* fmt, ...) {
		char buf[1024];
		va_list ap; va_start(ap, fmt); vsnprintf(buf, sizeof(buf), fmt, ap); va_end(ap);
		logline("%s", buf);
	}
}

extern "C" {

// ---- per-frame tick (FNA3D present) --------------------------------------
void FNA3D_SwapBuffers(void* device, void* src, void* dst, void* window) {
	static void (*real)(void*, void*, void*, void*) = nullptr;
	if (!real) real = (decltype(real))dlsym(RTLD_NEXT, "FNA3D_SwapBuffers");

	if (!g_loaded) { g_loaded = true; load_mods(); }
	for (auto& m : g_mods) if (m.update) m.update();

	if (real) real(device, src, dst, window);
}

// ---- input (SDL event pump) ----------------------------------------------
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
