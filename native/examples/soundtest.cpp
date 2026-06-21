// soundtest: K plays a stock sfx, M plays stock music.
#include "eetsmod.h"
using namespace Eets;

// add a custom sound with tools/add-sound.sh <name> <file>, then PlaySound("<name>")
extern "C" void EetsMod_Init() {
	Eets::Log("soundtest: K = sfx (Boing), M = music (Lounging Jazz)");
}
extern "C" void EetsMod_OnKey(int key, int, int down) {
	if (!down) return;
	if (key == 'k') PlaySound("Boing");
	if (key == 'm') Sound_PlayMusic("Lounging Jazz", true);
}
