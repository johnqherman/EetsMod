// extdemo - read Eets' behaviour extensions and live collisions. F3 toggles the readout.
#include "eetsmod.h"
#include <cstdio>
using namespace Eets;

static bool g_show = true;

static const char* walk_state_name(unsigned long s) {
	switch (s) {
		case WES_Walking: return "walking"; case WES_Rise: return "rise";
		case WES_Falling: return "falling"; case WES_Jumping: return "jumping";
		case WES_Standing: return "standing"; case WES_Rolling: return "rolling";
		case WES_Thwacking: return "thwacking"; case WES_Alert: return "alert";
		default: return "inactive";
	}
}

extern "C" void EetsMod_Init() {
	Eets::Log("extdemo: ready - F3 toggles the Eets extension/collision readout");
}

extern "C" void EetsMod_OnKey(int key, int, int down) {
	if (down && key == 0x4000003C) g_show = !g_show;   // F3
}

extern "C" void EetsMod_Update() {
	if (!g_show || World_IsInMainMenu()) return;
	Object* eets = World_GetEets();
	if (!eets) return;

	int y = 80;
	char buf[128];

	if (WalkingExtension* w = Object_GetWalkingExtension(eets)) {
		snprintf(buf, sizeof(buf), "walk: %s", walk_state_name(WalkingExtension_GetState(w)));
		DrawTextOutlined(20, y, buf, FONT_NORMAL, Colour(255, 255, 0, 255)); y += 26;
	}
	if (EmotionExtension* e = Object_GetEmotionExtension(eets)) {
		const char* name = EmotionExtension_GetEmotionName(e);
		snprintf(buf, sizeof(buf), "emotion: %s%s", name ? name : "?",
		         EmotionExtension_RecentlyChanged(e) ? " (changed)" : "");
		DrawTextOutlined(20, y, buf, FONT_NORMAL, Colour(255, 255, 0, 255)); y += 26;
	}
	if (HoldingExtension* h = Object_GetHoldingExtension(eets)) {
		int held = 0;
		ForEachHeld(h, [&](Object*) { held++; });
		snprintf(buf, sizeof(buf), "holding: %d", held);
		DrawTextOutlined(20, y, buf, FONT_NORMAL, Colour(255, 255, 0, 255)); y += 26;
	}

	int contacts = 0;
	ForEachCollision(eets, [&](const CollisionReport&) { contacts++; });
	if (contacts) {
		snprintf(buf, sizeof(buf), "collisions: %d", contacts);
		DrawTextOutlined(20, y, buf, FONT_NORMAL, Colour(255, 128, 128, 255));
	}
}
