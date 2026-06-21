// lowgrav - sample native Eets mod. Toggle low gravity with CTRL+G.
#include "eetsmod.h"
using namespace Eets;

static bool    g_low = false;
static bool    g_have = false;
static Vector2 g_normal;

extern "C" void EetsMod_Init() {
	Eets::Log("lowgrav: ready - press CTRL+G in a level");
}

extern "C" void EetsMod_OnKey(int key, int mods, int down) {
	if (!down) return;
	if (key == 'g' && (mods & EKMOD_CTRL)) {
		if (!g_have) { g_normal = World_GetGravity(); g_have = true; }
		g_low = !g_low;
		Vector2 g = g_normal;
		if (g_low) g.y *= 0.25f;
		World_SetGravity(g, 0);
		Eets::Log("lowgrav: %s", g_low ? "ON" : "OFF");
	}
}
