// sandbox - sample native mod showing config, enumeration, and more bindings.
//   CTRL+1 / CTRL+2 / CTRL+3  -> game speed paused / normal / fast
//   CTRL+E                    -> explosion at Eets
//   CTRL+O                    -> log live object count + ids
// Config: mods/sandbox.cfg, keys: explosion_radius (float), enable_speed (0/1)
#include "eetsmod.h"
#include <cstdio>
using namespace Eets;

static float g_radius     = 200.0f;
static bool  g_enableSpeed = true;

extern "C" void EetsMod_Init() {
	g_radius      = Eets::ConfigGetFloat("sandbox", "explosion_radius", 200.0f);
	g_enableSpeed = Eets::ConfigGetInt("sandbox", "enable_speed", 1) != 0;
	Eets::Log("sandbox: ready (radius=%.0f speed=%d) - CTRL+1/2/3, CTRL+E, CTRL+O",
	          g_radius, g_enableSpeed ? 1 : 0);
}

extern "C" void EetsMod_OnKey(int key, int mods, int down) {
	if (!down || !(mods & EKMOD_CTRL)) return;

	if (g_enableSpeed && key == '1') { World_SetGameSpeed(0); Eets::Log("sandbox: paused"); }
	else if (g_enableSpeed && key == '2') { World_SetGameSpeed(1); Eets::Log("sandbox: normal"); }
	else if (g_enableSpeed && key == '3') { World_SetGameSpeed(2); Eets::Log("sandbox: fast"); }
	else if (key == 'e') {
		Object* eets = World_GetEets();
		if (eets) { Vector2 p = Object_GetPosition(eets); World_CreateExplosion(p, g_radius);
		            Eets::Log("sandbox: boom @ %.0f,%.0f", p.x, p.y); }
	}
	else if (key == 'o') {
		int count = 0;
		Object* eets = World_GetEets();
		unsigned long eid = eets ? Object_GetID(eets) : 0;
		ForEachObject([&](Object* o) {
			if (!o) return;
			count++;
			if (Object_GetID(o) == eid && eets)
				Eets::Log("sandbox:   eets id=%lu", eid);
		});
		Eets::Log("sandbox: %d live object(s)", count);
	}
}
