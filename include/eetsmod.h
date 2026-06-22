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
	// printable keys are ascii: use 'a'..'z', '0'..'9', etc. directly. named ones for convenience:
	EKEY_a='a', EKEY_b='b', EKEY_c='c', EKEY_d='d', EKEY_e='e', EKEY_f='f', EKEY_g='g',
	EKEY_h='h', EKEY_i='i', EKEY_j='j', EKEY_k='k', EKEY_l='l', EKEY_m='m', EKEY_n='n',
	EKEY_o='o', EKEY_p='p', EKEY_q='q', EKEY_r='r', EKEY_s='s', EKEY_t='t', EKEY_u='u',
	EKEY_v='v', EKEY_w='w', EKEY_x='x', EKEY_y='y', EKEY_z='z',
	EKEY_0='0', EKEY_1='1', EKEY_2='2', EKEY_3='3', EKEY_4='4',
	EKEY_5='5', EKEY_6='6', EKEY_7='7', EKEY_8='8', EKEY_9='9',
	EKEY_SPACE=' ', EKEY_RETURN='\r', EKEY_ESCAPE=0x1b, EKEY_TAB='\t',
	EKEY_BACKSPACE=0x08, EKEY_DELETE=0x7f,
	// non-ascii keys are SDL keycodes (scancode | 0x40000000)
	EKEY_F1=0x4000003a, EKEY_F2=0x4000003b, EKEY_F3=0x4000003c, EKEY_F4=0x4000003d,
	EKEY_F5=0x4000003e, EKEY_F6=0x4000003f, EKEY_F7=0x40000040, EKEY_F8=0x40000041,
	EKEY_F9=0x40000042, EKEY_F10=0x40000043, EKEY_F11=0x40000044, EKEY_F12=0x40000045,
	EKEY_RIGHT=0x4000004f, EKEY_LEFT=0x40000050, EKEY_DOWN=0x40000051, EKEY_UP=0x40000052,
	EKEY_HOME=0x4000004a, EKEY_END=0x4000004d, EKEY_PAGEUP=0x4000004b, EKEY_PAGEDOWN=0x4000004e,
	EKEY_INSERT=0x40000049,
	EKMOD_LCTRL = 0x0040, EKMOD_RCTRL = 0x0080,
	EKMOD_LSHIFT = 0x0001, EKMOD_RSHIFT = 0x0002,
	EKMOD_LALT = 0x0100, EKMOD_RALT = 0x0200,
};
#define EKMOD_CTRL  (EKMOD_LCTRL | EKMOD_RCTRL)
#define EKMOD_SHIFT (EKMOD_LSHIFT | EKMOD_RSHIFT)
#define EKMOD_ALT   (EKMOD_LALT | EKMOD_RALT)
enum { EMBTN_MOTION = -1, EMBTN_LEFT = 1, EMBTN_MIDDLE = 2, EMBTN_RIGHT = 3 };
#endif
