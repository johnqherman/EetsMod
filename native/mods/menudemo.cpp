// menudemo - custom menu showcase. Press M to toggle the menu.
#include "eetsmod.h"
#include "eets_ui.h"
using namespace Eets;

static bool g_open = false;
static bool g_lowgrav = false;
static float g_speed = 1.0f;

extern "C" void EetsMod_Init() {
	Eets::Log("menudemo: press M for a custom menu");
}

extern "C" void EetsMod_OnKey(int key, int, int down) {
	if (down && (key == 'm')) g_open = !g_open;
}

extern "C" void EetsMod_OnMouse(int x, int y, int button, int down) {
	UI::FeedMouse(x, y, button, down);
}

extern "C" void EetsMod_Update() {
	if (!g_open) return;
	UI::Begin(40, 80, 240, "menudemo");
	if (UI::Button("Spawn whale"))
		World_CreateObject("whale", Vector2{(float)(RenderWidth()/2), (float)(RenderHeight()/2)});
	if (UI::Button("Explosion (center)"))
		World_CreateExplosion(Vector2{(float)(RenderWidth()/2), (float)(RenderHeight()/2)}, 250.0f);
	if (UI::Toggle("Low gravity", g_lowgrav))
		World_SetGravity(Vector2{0, g_lowgrav ? 200.0f : 800.0f}, 0);
	UI::Slider("Game speed", g_speed, 0.0f, 2.0f);
	World_SetGameSpeed((unsigned long)(g_speed + 0.5f));
	UI::Label("M closes this menu");
	UI::End();
}
