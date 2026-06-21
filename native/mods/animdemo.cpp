// animdemo - draw an animated sprite (.anim) at the cursor. Press J to toggle.
// fps defaults to the anim's native rate.
#include "eetsmod.h"
using namespace Eets;

static bool g_on = true;

extern "C" void EetsMod_Init() {
	Eets::Log("animdemo: press J to toggle an animated butterfly at the cursor");
}
extern "C" void EetsMod_OnKey(int key, int, int down) {
	if (down && key == 'j') g_on = !g_on;
}
extern "C" void EetsMod_Update() {
	if (!g_on) return;
	DrawAnim("DATA:Animations/butterfly/butterfly.anim",
	         MouseX() - 32, MouseY() - 32, (float)DeltaTime());
}
