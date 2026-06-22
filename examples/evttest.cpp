// evttest - validates the Win32 inline-hook path: logs every engine event that fires.
#include "eetsmod.h"
using namespace Eets;

extern "C" void EetsMod_Init() { Eets::Log("evttest: waiting for engine events (emotion_change / goal_check / ...)"); }

extern "C" void EetsMod_OnEvent(const char* name, void* a, void* b) {
	Eets::Log("evttest: EVENT '%s' a=%p b=%p", name, a, b);
}
