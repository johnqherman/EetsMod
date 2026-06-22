// soundtest: P plays a custom bundled sound, K a stock sfx, M stock music.
#include "eetsmod.h"
using namespace Eets;

// "pluh" is a custom sound shipped inside this mod: soundtest.assets/Sound/{Wav,Patch}.
// the bundle's assets/ tree installs into the game's Data/ when the .eetsmod is added,
// so the sound travels with the mod - no separate add-sound step on the player's machine.
extern "C" void EetsMod_Init() {
	Eets::Log("soundtest: P = custom sound (pluh), K = stock sfx, M = music");
}
extern "C" void EetsMod_OnKey(int key, int, int down) {
	if (!down) return;
	if (key == EKEY_p) PlaySound("pluh");
	if (key == EKEY_k) PlaySound("Boing");
	if (key == EKEY_m) Sound_PlayMusic("Lounging Jazz", true);
}
