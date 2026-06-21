// eets_engine.h - typed bindings to Eets engine functions by absolute address.
//
// Eets is a non-PIE (fixed-base) x86-64 ELF, so these addresses are stable at
// runtime for a given build. Regenerate with native/gen_engine_header.sh if the
// game updates. Generated for BuildID e81cc5504d3ef03324805df3e9fc508c1bf8c628.
//
// ABI note: Vector2 is { float x, y } -> one SSE eightbyte, passed/returned in
// xmm0 exactly as the engine's own C++ does, so matching the type matches the ABI.
#pragma once
#include <cstdint>

namespace Eets {

struct Vector2 { float x, y; };
struct Object;   // opaque engine type

namespace addr {
	// World_* (Lua-binding statics; thin forwarders to the Simulator singleton)
	constexpr uintptr_t World_GetGravity            = 0x5bc710;
	constexpr uintptr_t World_SetGravity            = 0x5bc6d0;
	constexpr uintptr_t World_CreateObject          = 0x5bc660;
	constexpr uintptr_t World_GetEets               = 0x5bca40;
	constexpr uintptr_t World_CreateExplosion       = 0x5bc0d0;
	constexpr uintptr_t World_CreateExplosionSpecial= 0x5bc170;
	// Object_* (take an Object*)
	constexpr uintptr_t Object_ApplyImpulse         = 0x5c9d40;
	constexpr uintptr_t Object_EnablePhysics        = 0x5c9d10;
}

// typed function-pointer wrappers
inline Vector2 World_GetGravity() {
	return ((Vector2(*)())addr::World_GetGravity)();
}
inline void World_SetGravity(Vector2 g, int mode = 0) {
	((void(*)(Vector2, int))addr::World_SetGravity)(g, mode);
}
inline Object* World_CreateObject(const char* name, Vector2 pos) {
	return ((Object*(*)(const char*, Vector2))addr::World_CreateObject)(name, pos);
}
inline Object* World_GetEets() {
	return ((Object*(*)())addr::World_GetEets)();
}
inline void World_CreateExplosion(const Vector2& pos, float radius) {
	((void(*)(const Vector2&, float))addr::World_CreateExplosion)(pos, radius);
}
inline void World_CreateExplosionSpecial() {
	((void(*)())addr::World_CreateExplosionSpecial)();
}
inline void Object_ApplyImpulse(Object* o, const Vector2& impulse) {
	((void(*)(Object*, const Vector2&))addr::Object_ApplyImpulse)(o, impulse);
}
inline void Object_EnablePhysics(Object* o, bool enabled) {
	((void(*)(Object*, bool))addr::Object_EnablePhysics)(o, enabled);
}

} // namespace Eets
