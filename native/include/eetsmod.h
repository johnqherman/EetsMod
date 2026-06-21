#pragma once
#include "eets_engine.h"

extern "C" {

void EetsMod_Init();

void EetsMod_Update();

void EetsMod_OnKey(int keycode, int mods, int down);

void EetsMod_OnMouse(int x, int y, int button, int down);

void EetsMod_OnWheel(int dx, int dy);

void EetsMod_OnEvent(const char* name, void* a, void* b);

void EetsMod_OnText(const char* utf8);

void EetsMod_Shutdown();

} // extern "C"

namespace Eets {
	void Log(const char* fmt, ...);

	const char* ConfigGet(const char* mod, const char* key, const char* def);
	int   ConfigGetInt(const char* mod, const char* key, int def);
	float ConfigGetFloat(const char* mod, const char* key, float def);

	bool Hook(void* target, void* detour, void** original);

	// last mouse position in render space (correct in fullscreen)
	int MouseX();
	int MouseY();

	// render backbuffer size; use for HUD instead of ScreenWidth/Height (configured res)
	int RenderWidth();
	int RenderHeight();

	const char* SaveGet(const char* mod, const char* key, const char* def);
	void  SaveSet(const char* mod, const char* key, const char* val);
	int   SaveGetInt(const char* mod, const char* key, int def);
	void  SaveSetInt(const char* mod, const char* key, int v);
	float SaveGetFloat(const char* mod, const char* key, float def);
	void  SaveSetFloat(const char* mod, const char* key, float v);

	double Time();
	double DeltaTime();

	void StartTextInput();
	void StopTextInput();
}

// minimal SDL keycode / modifier constants (so mods don't need SDL headers).
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
