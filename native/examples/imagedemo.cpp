// imagedemo - draw a custom image (Eets.png) at the cursor. Press I to toggle.
#include "eetsmod.h"
using namespace Eets;

static bool g_on = true;

extern "C" void EetsMod_Init() {
	void* s = LoadSprite("Eets.png");
	Eets::Log("imagedemo: Eets.png %dx%d loaded; press I to toggle, follows the mouse",
	          SpriteWidth(s), SpriteHeight(s));
}
extern "C" void EetsMod_OnKey(int key, int, int down) {
	if (down && key == 'i') g_on = !g_on;
}
extern "C" void EetsMod_OnMouse(int x, int y, int, int) { (void)x; (void)y; }
extern "C" void EetsMod_Update() {
	if (!g_on) return;
	DrawImage("Eets.png", MouseX() - 256, MouseY() - 256);
}
