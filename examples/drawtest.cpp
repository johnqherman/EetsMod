// drawtest - full UI-primitive validation (mirrors loader MODS panel). CTRL+G toggles.
#include "eetsmod.h"
using namespace Eets;

static bool g_on = true;

extern "C" void EetsMod_Init() { Eets::Log("drawtest: CTRL+G toggles full UI canary"); }
extern "C" void EetsMod_OnKey(int key, int, int down) { if (down && (key=='g'||key=='G')) g_on = !g_on; }

extern "C" void EetsMod_Update() {
	if (!g_on) return;
	if (!GraphicsEngine_i()) return;
	// drop shadow + red panel + black border (FillRect x2 + DrawRect)
	FillRect(53, 53, 240, 90, Color(0, 0, 0, 110));
	FillRect(50, 50, 240, 90, Color(205, 40, 35, 255));
	DrawRect(50, 50, 240, 90, Color(0, 0, 0, 255), 3.0f);
	// title bar + label text (printText)
	FillRect(54, 54, 232, 22, Color(165, 22, 20, 255));
	DrawText(60, 56, "MODS  (drawtest)", Color(255, 232, 40, 255));
	DrawText(60, 90, "FillRect + DrawLine + Text OK", Color(255, 255, 255, 255));
	// diagonal accent line
	DrawLine(Vector2{50, 145}, Vector2{290, 145}, Color(255, 232, 40, 255));
}
