// eetsmod.h - public API for writing native (C++) Eets mods.
//
// A mod is a shared object (.so) placed in the game's `mods/` directory. The
// loader (libeetsmod.so, LD_PRELOAD'd) dlopen's it and calls the optional
// entry points below. Mods call engine functions via eets_engine.h.
#pragma once
#include "eets_engine.h"

extern "C" {

// Called once, on the first rendered frame (engine fully initialised). Optional.
void EetsMod_Init();

// Called every frame, just before the back buffer is presented. Optional.
void EetsMod_Update();

// Called for each keyboard event. `keycode` is an SDL keycode (SDLK_*),
// `mods` is the SDL key-modifier bitmask (KMOD_*), `down` is 1 on press,
// 0 on release. Optional.
void EetsMod_OnKey(int keycode, int mods, int down);

// Called before the mod is unloaded (hot-reload or shutdown). Optional.
void EetsMod_Shutdown();

} // extern "C"

// ---- helpers provided by the loader (resolved at dlopen time) --------------
namespace Eets {
	// Log a line to stderr and to <game>/Log/native_mods.log.
	void Log(const char* fmt, ...);

	// Read a value from <game>/mods/<mod>.cfg (simple key=value lines).
	// Returns `def` if the file or key is absent. Pointer is valid until the
	// next ConfigGet call for the same mod.
	const char* ConfigGet(const char* mod, const char* key, const char* def);
	int   ConfigGetInt(const char* mod, const char* key, int def);
	float ConfigGetFloat(const char* mod, const char* key, float def);
}

// Minimal SDL keycode / modifier constants (so mods don't need SDL headers).
#ifndef EETS_SDL_KEYS
#define EETS_SDL_KEYS
enum {
	EKEY_a = 'a', EKEY_b = 'b', EKEY_g = 'g',           // ascii keycodes
	EKEY_SPACE = ' ',
	EKMOD_LCTRL = 0x0040, EKMOD_RCTRL = 0x0080,
	EKMOD_LSHIFT = 0x0001, EKMOD_RSHIFT = 0x0002,
	EKMOD_LALT = 0x0100, EKMOD_RALT = 0x0200,
};
#define EKMOD_CTRL  (EKMOD_LCTRL | EKMOD_RCTRL)
#define EKMOD_SHIFT (EKMOD_LSHIFT | EKMOD_RSHIFT)
#define EKMOD_ALT   (EKMOD_LALT | EKMOD_RALT)
#endif
