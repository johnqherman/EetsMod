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
#include <string>
#include <unordered_map>
#include "eets_addr.h"   // complete addr:: table (all 76 statics + methods/UI)

namespace Eets {

// Addresses are valid for this exact game build. The loader warns if the running
// binary's BuildID differs (regenerate with gen_engine_header.sh).
constexpr const char* EXPECTED_BUILDID = "e81cc5504d3ef03324805df3e9fc508c1bf8c628";

struct Vector2 { float x, y; };
struct Colour { unsigned char r, g, b, a;          // 4-byte packed (engine ABI)
	Colour(unsigned char R=255, unsigned char G=255, unsigned char B=255, unsigned char A=255)
		: r(R), g(G), b(B), a(A) {}
};
struct Object;        // opaque engine type
struct MotionModel;   // opaque
struct ObjectMgr;     // opaque

// FontPrintSizes enum -> pixel height: 1=13 2=14 3=20 4=28 5=35
enum FontSize { FONT_TINY = 1, FONT_SMALL = 2, FONT_NORMAL = 3, FONT_BIG = 4, FONT_HUGE = 5 };
// FontPrintStyles -> typeface. STYLE_GEEK (geekabyte) is the in-game GUI font.
enum FontStyle { STYLE_KOMIKA = 1, STYLE_GEEK = 2, STYLE_BRADY = 4 };

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
inline Object* World_GetObjectByID(unsigned long id) {
	return ((Object*(*)(unsigned long))addr::World_GetObjectByID)(id);
}
inline void World_CreateEffect(const char* name, Vector2 pos) {
	((void(*)(const char*, Vector2))addr::World_CreateEffect)(name, pos);
}
inline void World_Scare(const Vector2& pos, float radius, int strength) {
	((void(*)(const Vector2&, float, int))addr::World_Scare)(pos, radius, strength);
}
inline void World_SetGameSpeed(unsigned long speed) {   // 0 = paused, 1 = normal, 2 = fast
	((void(*)(unsigned long))addr::World_SetGameSpeed)(speed);
}
inline void World_ChangeEmotion(unsigned long objHash, unsigned int emotion) {
	((void(*)(unsigned long, unsigned int))addr::World_ChangeEmotion)(objHash, emotion);
}
inline void Sound_CreateSound(const char* name, bool loop, float vol, const Vector2& pos) {
	((void(*)(const char*, bool, float, const Vector2&))addr::Sound_CreateSound)(name, loop, vol, pos);
}

// ---- Object methods --------------------------------------------------------
inline Vector2 Object_GetPosition(Object* o) {
	return ((Vector2(*)(Object*))addr::Object_GetPosition)(o);
}
inline Vector2 Object_GetVelocity(Object* o) {
	return ((Vector2(*)(Object*))addr::Object_GetVelocity)(o);
}
inline unsigned long Object_GetID(Object* o) {
	return ((unsigned long(*)(Object*))addr::Object_GetID)(o);
}
inline MotionModel* Object_GetMotionModel(Object* o) {
	return ((MotionModel*(*)(Object*))addr::Object_GetMotionModel)(o);
}
inline void Object_SetPosition(Object* o, const Vector2& p)   { ((void(*)(Object*, const Vector2&))addr::Object_SetPosition)(o, p); }
inline void Object_ForcePosition(Object* o, const Vector2& p) { ((void(*)(Object*, const Vector2&))addr::Object_ForcePosition)(o, p); }
inline void Object_SetFacing(Object* o, const Vector2& f)     { ((void(*)(Object*, const Vector2&))addr::Object_SetFacing)(o, f); }
inline Vector2 Object_GetFacing(Object* o)                    { return ((Vector2(*)(Object*))addr::Object_GetFacing)(o); }
inline void Object_SetFlipped(Object* o, bool f)             { ((void(*)(Object*, bool))addr::Object_SetFlipped)(o, f); }
inline bool Object_GetFlipped(Object* o)                     { return ((bool(*)(Object*))addr::Object_GetFlipped)(o); }
inline void Object_KillMe(Object* o)                        { ((void(*)(Object*))addr::Object_KillMe)(o); }

// ---- MotionModel methods ---------------------------------------------------
inline void MotionModel_PushMotion(MotionModel* m, const char* name, bool a, bool b) {
	((void(*)(MotionModel*, const char*, bool, bool))addr::MotionModel_PushMotion)(m, name, a, b);
}
inline void MotionModel_PopMotion(MotionModel* m) {
	((void(*)(MotionModel*))addr::MotionModel_PopMotion)(m);
}
inline const char* MotionModel_GetCurrentMotionName(MotionModel* m) {
	return ((const char*(*)(MotionModel*))addr::MotionModel_GetCurrentMotionName)(m);
}

// ---- live object enumeration ----------------------------------------------
// ObjectMgr holds a std::vector<Object*> at offsets +0 (begin) / +8 (end).
inline ObjectMgr* ObjectMgr_i() {
	return ((ObjectMgr*(*)())addr::ObjectMgr_i)();
}
// Call `fn(Object*)` for every live object. Safe no-op if no world is loaded.
template <class Fn>
inline void ForEachObject(Fn fn) {
	char* mgr = (char*)ObjectMgr_i();
	if (!mgr) return;
	Object** begin = *(Object***)(mgr + 0);
	Object** end   = *(Object***)(mgr + 8);
	for (Object** p = begin; p && p < end; ++p) fn(*p);
}

// ---- state / screen / text -------------------------------------------------
inline bool World_IsInMainMenu() {
	return ((bool(*)())addr::World_IsInMainMenu)();
}
inline int ScreenWidth() {
	char* g = (char*)((void*(*)())addr::GraphicsEngine_i)();
	return g ? *(int*)(g + 0x40) : 0;
}
inline int ScreenHeight() {
	char* g = (char*)((void*(*)())addr::GraphicsEngine_i)();
	return g ? *(int*)(g + 0x44) : 0;
}
// Draw text at screen pixel (x, y), origin top-left, using the engine font
// (normal 20px size).
inline void DrawText(int x, int y, const char* text, Colour c = Colour()) {
	((void(*)(int, int, const char*, const Colour&))addr::printText)(x, y, text, c);
}
// Draw text at a chosen font size (FONT_TINY..FONT_HUGE).
// NB: DrawString's last Vector2 is the text BASELINE DIRECTION, not a uniform
// scale - it must stay horizontal {1,0} or the text renders rotated. Size comes
// from the FontSize enum (13/14/20/28/35 px). The `dirx` arg defaults to 1.0
// (normal); values <1 shrink horizontally but keep the baseline horizontal.
inline void DrawTextSized(int x, int y, const char* text, int size,
                          Colour c = Colour(), int style = STYLE_GEEK, float dirx = 1.0f) {
	std::string s = text ? text : "";
	Vector2 pos{(float)x, (float)y}, dir{dirx, 0.0f};   // horizontal baseline
	((void(*)(const std::string&, int, int, Colour, Vector2, bool, const Vector2&))
	 addr::TextPrinter_DrawString)(s, size, style, c, pos, false, dir);
}
// Same, with a dropshadow (offset dark copy) like the game's GUI text.
inline void DrawTextOutlined(int x, int y, const char* text, int size,
                             Colour c = Colour(), Colour shadow = Colour(0, 0, 0, 200),
                             int style = STYLE_GEEK) {
	DrawTextSized(x + 2, y + 2, text, size, shadow, style);
	DrawTextSized(x, y, text, size, c, style);
}

// ---- 2D primitives (for custom menus / HUD) --------------------------------
inline void* GraphicsEngine_i() { return ((void*(*)())addr::GraphicsEngine_i)(); }
// GraphicsEngine geometry funcs swap R<->B internally; pre-swap so our RGBA shows correctly.
inline Colour swab(Colour c) { return Colour(c.b, c.g, c.r, c.a); }
inline void DrawLine(Vector2 a, Vector2 b, Colour c, float width = 1.0f) {
	void* g = GraphicsEngine_i(); if (!g) return;
	Colour s = swab(c);
	((void(*)(void*, const Vector2&, const Vector2&, const Colour&, float))
	 addr::GraphicsEngine_DrawLine)(g, a, b, s, width);
}
// filled rectangle (GraphicsEngine::DrawSquare = 2 triangles)
inline void FillRect(int x, int y, int w, int h, Colour c) {
	void* g = GraphicsEngine_i(); if (!g) return;
	Colour s = swab(c);
	Vector2 a{(float)x, (float)y}, b{(float)(x + w), (float)(y + h)};
	((void(*)(void*, const Vector2&, const Vector2&, const Colour&))
	 addr::GraphicsEngine_DrawSquare)(g, a, b, s);
}
// filled circle
inline void FillCircle(int x, int y, float r, Colour c, int segs = 24) {
	void* g = GraphicsEngine_i(); if (!g) return;
	Colour s = swab(c);
	Vector2 p{(float)x, (float)y};
	((void(*)(void*, const Vector2&, float, const Colour&, int))
	 addr::GraphicsEngine_DrawCircleFilled)(g, p, r, s, segs);
}
// rectangle outline of thickness t (built from filled bars - DrawLine ignores width)
inline void DrawRect(int x, int y, int w, int h, Colour c, float t = 2.0f) {
	int ti = (int)t; if (ti < 1) ti = 1;
	FillRect(x, y, w, ti, c);                 // top
	FillRect(x, y + h - ti, w, ti, c);        // bottom
	FillRect(x, y, ti, h, c);                 // left
	FillRect(x + w - ti, y, ti, h, c);        // right
}

// ---- more engine wrappers --------------------------------------------------
inline void World_Pause()            { ((void(*)())addr::World_Pause)(); }
inline bool World_IsPaused()         { return ((bool(*)())addr::World_IsPaused)(); }
inline bool World_IsSimulating()     { return ((bool(*)())addr::World_IsSimulating)(); }
inline void World_SetMaximumSpeed(float s) { ((void(*)(float))addr::World_SetMaximumSpeed)(s); }
inline void Sound_PlayMusic(const char* name, bool loop) { ((void(*)(const char*, bool))addr::Sound_PlayMusic)(name, loop); }
inline void Sound_SetMusicVolume(long v) { ((void(*)(long))addr::Sound_SetMusicVolume)(v); }
inline void Misc_Print(const char* s) { ((void(*)(const char*))addr::Misc_Print)(s); }
inline void Misc_SaveScreenshot()    { ((void(*)())addr::Misc_SaveScreenshot)(); }
inline void Object_EnableCollisions(Object* o, bool e) { ((void(*)(Object*, bool))addr::Object_EnableCollisions)(o, e); }
inline void Object_CallFunction(Object* o, const char* fn) { ((void(*)(Object*, const char*))addr::Object_CallFunction)(o, fn); }
inline void Object_CreateEffect(Object* o, const char* fx) { ((void(*)(Object*, const char*))addr::Object_CreateEffect)(o, fx); }
inline unsigned long Object_GetBlueprintHash(Object* o) { return ((unsigned long(*)(Object*))addr::Object_GetBlueprintHash)(o); }

// ---- custom images (EXPERIMENTAL; verify visually) -------------------------
// Loads an image via TextureManager (caches it), looks the texture up in the
// engine's cache (a libstdc++ unordered_map<string, KLEITEXTURE*> at the manager
// singleton - ABI-compatible because mods build with the same libstdc++), and
// draws it with IGraphicsEngine::DrawTexture at render-space (x, y).
// `path` is a DATA: path, e.g. "DATA:Images/foo.tga". Returns false if not drawn.
inline bool DrawImage(const char* path, int x, int y) {
	void* tm = *(void**)addr::TextureManager_instance;
	if (!tm) return false;
	auto* cache = (std::unordered_map<std::string, void*>*)tm;   // map at manager+0
	std::string key = path;
	((int(*)(void*, const std::string&, int, bool))addr::TextureManager_LoadTexture)(tm, key, 0, true);
	auto it = cache->find(key);
	if (it == cache->end() || !it->second) return false;
	void* ige = ((void*(*)())addr::IGraphicsEngine_i)();
	if (!ige) return false;
	Vector2 pos{(float)x, (float)y};
	((void(*)(void*, void*, const Vector2&))addr::IGraphicsEngine_DrawTexture)(ige, it->second, pos);
	return true;
}

// ---- localization ----------------------------------------------------------
// Resolve a localized string id ("$01701") to its text. Returns the id itself
// if unresolved. Add your own strings at boot with StringPool::LoadFile.
inline const char* Localize(const char* id) {
	void* sp = *(void**)addr::StringPool_instance;
	if (!sp) return id;
	return ((const char*(*)(void*, const char*))addr::StringPool_Resolve)(sp, id);
}

} // namespace Eets
