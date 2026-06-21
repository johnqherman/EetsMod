// imagedemo - EXPERIMENTAL custom-image draw test. Press I to toggle.
// Draws a stock texture at a fixed spot to validate the DrawImage path.
#include "eetsmod.h"
using namespace Eets;

static bool g_on = true;

extern "C" void EetsMod_Init() {
	Eets::Log("imagedemo: press I to toggle a test image draw");
}
extern "C" void EetsMod_OnKey(int key, int, int down) {
	if (down && key == 'i') g_on = !g_on;
}
extern "C" void EetsMod_Update() {
	if (!g_on) return;
	bool ok = DrawImage("Eets.png", 200, 150);
	static bool logged = false;
	if (!logged) { logged = true; Eets::Log("imagedemo: DrawImage('Eets.png') returned %d", ok ? 1 : 0); }
}
