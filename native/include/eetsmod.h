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

// Mouse motion/button. button: EMBTN_MOTION(-1), EMBTN_LEFT(1), EMBTN_MIDDLE(2),
// EMBTN_RIGHT(3); down 1/0 (0 for motion). (x, y) are window pixels. Optional.
void EetsMod_OnMouse(int x, int y, int button, int down);

// Mouse wheel scroll (dx, dy). Optional.
void EetsMod_OnWheel(int dx, int dy);

// Engine event. Currently fired: "object_spawn" (a = Object*, b = name string).
// More events as engine hooks are added. Optional.
void EetsMod_OnEvent(const char* name, void* a, void* b);

// Called before the mod is unloaded (hot-reload or shutdown). Optional.
void EetsMod_Shutdown();

} // extern "C"

// ---- helpers provided by the loader (resolved at dlopen time) --------------
namespace Eets {
	// Log a line to stderr and to <game>/Log/native_mods.log.
	void Log(const char* fmt, ...);

	// Read a value from <game>/mods/<mod>.cfg (simple key=value lines).
	const char* ConfigGet(const char* mod, const char* key, const char* def);
	int   ConfigGetInt(const char* mod, const char* key, int def);
	float ConfigGetFloat(const char* mod, const char* key, float def);

	// Detour an arbitrary engine function. On success *original receives a
	// trampoline to call the unhooked function. Returns false (no change) if the
	// target's prologue can't be safely relocated.
	//   static void(*orig)() = nullptr;
	//   Eets::Hook((void*)0xADDR, (void*)my_detour, (void**)&orig);
	bool Hook(void* target, void* detour, void** original);

	// Last known mouse position, in RENDER space (matches DrawText / world
	// coords; correct in fullscreen, unlike raw window pixels).
	int MouseX();
	int MouseY();

	// Actual render backbuffer size (from the live viewport). Use these for HUD
	// placement instead of eets_engine.h's ScreenWidth/Height, which return the
	// configured resolution and can differ from the render size in fullscreen.
	int RenderWidth();
	int RenderHeight();

	// Persistent per-mod save data (mods/<mod>.save). Survives restarts; unlike
	// config (user-editable settings) this is for mod-written state.
	const char* SaveGet(const char* mod, const char* key, const char* def);
	void  SaveSet(const char* mod, const char* key, const char* val);
	int   SaveGetInt(const char* mod, const char* key, int def);
	void  SaveSetInt(const char* mod, const char* key, int v);
	float SaveGetFloat(const char* mod, const char* key, float def);
	void  SaveSetFloat(const char* mod, const char* key, float v);
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
enum { EMBTN_MOTION = -1, EMBTN_LEFT = 1, EMBTN_MIDDLE = 2, EMBTN_RIGHT = 3 };
#endif
