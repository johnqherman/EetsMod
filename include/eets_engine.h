// abi note: Vector2 { float x, y } is one SSE eightbyte in xmm0 - matching the type matches the abi.
#pragma once
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>
#ifdef _WIN32
#include "eets_addr_win.h"   // PE32 i386 RVAs, resolved at runtime
#else
#include "eets_addr.h"       // Linux x86-64 absolute addresses
#endif

// On 32-bit MSVC (Win32) C++ member functions use __thiscall (this in ECX, not a
// stack arg). The wrappers below pass the object/extension/this as the first
// explicit arg, so member-targeting casts must carry __thiscall there. On Linux
// (SysV) the convention attribute is empty and these casts are unchanged.
#ifdef _WIN32
#  define ECALL __thiscall
#else
#  define ECALL
#endif

namespace Eets {

// valid for this exact build; loader warns if the running BuildID differs
constexpr const char* EXPECTED_BUILDID = "e81cc5504d3ef03324805df3e9fc508c1bf8c628";

struct Vector2 { float x, y; };
struct Color { unsigned char r, g, b, a;          // 4-byte packed (engine abi)
	Color(unsigned char R=255, unsigned char G=255, unsigned char B=255, unsigned char A=255)
		: r(R), g(G), b(B), a(A) {}
};
struct Object;        // opaque engine type
struct MotionModel;   // opaque
struct ObjectMgr;     // opaque

// extension types: object behavior components, fetched from an Object (see Get*Extension below)
struct PositionExtension; struct SuckableExtension; struct WalkingExtension;
struct ThwackerExtension; struct EmotionPlatformExtension; struct EdibleExtension;
struct EmotionExtension; struct FlyingExtension; struct LightingExtension;
struct HoldingExtension; struct RollingExtension; struct PhysicsExtension;

// WalkingExtension::GetState values
enum WalkState { WES_Walking = 0, WES_Rise = 1, WES_Falling = 2, WES_Jumping = 3,
                 WES_Standing = 4, WES_Rolling = 5, WES_Thwacking = 6, WES_Alert = 7, WES_Inactive = 8 };
// FlyingExtension state values
enum FlyState { FS_Up = 0, FS_Down = 1, FS_Left = 2, FS_Right = 3, FS_Disabled = 4, FS_Count = 5 };

// a single contact from PhysicsExtension's report deque (luabind layout)
struct CollisionReport {
	Vector2 collision_point;      // +0x00
	Vector2 relative_velocity;    // +0x08
	unsigned long collision_count;// +0x10
	unsigned long id_1;           // +0x18
	unsigned long id_2;           // +0x20
};

// Engine string ABI. The engine takes std::string by const-ref (reads only; caller owns).
// On Linux that's libstdc++ std::string. On Win32 the engine is MSVC: its std::string has a
// different memory layout, and mingw's std::string is NOT compatible - passing one crashes.
// EString is compile-time-polymorphic: same call site, the type models the right ABI per platform.
#ifdef _WIN32
// MSVC 32-bit std::string (release): union{char buf[16]; char* ptr} @0x00, size @0x10, capacity @0x14.
// SSO when capacity <= 0xf (data inline); else _Ptr. Passed by const-ref so sizeof is irrelevant -
// only the field layout the engine reads matters (all confirmed from the decompiled TextPrinter).
struct EString {
	union { char buf[16]; const char* ptr; };
	uint32_t size, cap;
	EString(const char* s) {
		size = s ? (uint32_t)std::strlen(s) : 0;
		if (size < 16) { if (size) std::memcpy(buf, s, size); buf[size] = '\0'; cap = 15; }
		else { ptr = s; cap = size; }   // SSO overflow: point at the caller's buffer (engine only reads it)
	}
};
#else
using EString = std::string;
#endif

// FontPrintSizes enum -> pixel height: 1=13 2=14 3=20 4=28 5=35
enum FontSize { FONT_TINY = 1, FONT_SMALL = 2, FONT_NORMAL = 3, FONT_BIG = 4, FONT_HUGE = 5 };
// FontPrintStyles -> typeface. STYLE_GEEK (geekabyte) is the in-game gui font.
enum FontStyle { STYLE_KOMIKA = 1, STYLE_GEEK = 2, STYLE_NORMAL = 3, STYLE_BRADY = 4 };

inline Vector2 World_GetGravity() {
#ifdef _WIN32
	// 32-bit MSVC returns an 8-byte struct via a hidden first pointer arg (sret);
	// the engine's real signature is void World_GetGravity(Vector2* out).
	Vector2 v; ((void(*)(Vector2*))addr::World_GetGravity)(&v); return v;
#else
	return ((Vector2(*)())addr::World_GetGravity)();
#endif
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
	// sret: returns SoundHandle by value
	char handle[16] = {0};
	((void(*)(void*, const char*, bool, float, const Vector2&))addr::Sound_CreateSound)(handle, name, loop, vol, pos);
}
inline void PlaySound(const char* name, float vol = 0.0f) {
	Vector2 z{0.0f, 0.0f};
	Sound_CreateSound(name, false, vol, z);
}

inline Vector2 Object_GetPosition(Object* o) {
#ifdef _WIN32
	return *((Vector2*(ECALL *)(Object*))addr::Object_GetPosition)(o);   // returns Vector2* (ptr in EAX), deref
#else
	return ((Vector2(*)(Object*))addr::Object_GetPosition)(o);
#endif
}
inline Vector2 Object_GetVelocity(Object* o) {
#ifdef _WIN32
	Vector2 r; ((void(ECALL *)(Object*, Vector2*))addr::Object_GetVelocity)(o, &r); return r;   // sret
#else
	return ((Vector2(*)(Object*))addr::Object_GetVelocity)(o);
#endif
}
inline unsigned long Object_GetID(Object* o) {
	return ((unsigned long(ECALL *)(Object*))addr::Object_GetID)(o);
}
inline MotionModel* Object_GetMotionModel(Object* o) {
	return ((MotionModel*(ECALL *)(Object*))addr::Object_GetMotionModel)(o);
}
inline void Object_SetPosition(Object* o, const Vector2& p)   { ((void(ECALL *)(Object*, const Vector2&))addr::Object_SetPosition)(o, p); }
inline void Object_ForcePosition(Object* o, const Vector2& p) { ((void(ECALL *)(Object*, const Vector2&))addr::Object_ForcePosition)(o, p); }
inline void Object_SetFacing(Object* o, const Vector2& f)     { ((void(ECALL *)(Object*, const Vector2&))addr::Object_SetFacing)(o, f); }
inline Vector2 Object_GetFacing(Object* o) {
#ifdef _WIN32
	Vector2 r; ((void(ECALL *)(Object*, Vector2*))addr::Object_GetFacing)(o, &r); return r;   // sret
#else
	return ((Vector2(*)(Object*))addr::Object_GetFacing)(o);
#endif
}
inline void Object_SetFlipped(Object* o, bool f)             { ((void(ECALL *)(Object*, bool))addr::Object_SetFlipped)(o, f); }
inline bool Object_GetFlipped(Object* o)                     { return ((bool(ECALL *)(Object*))addr::Object_GetFlipped)(o); }
inline void Object_KillMe(Object* o)                        { ((void(ECALL *)(Object*))addr::Object_KillMe)(o); }

inline void MotionModel_PushMotion(MotionModel* m, const char* name, bool a, bool b) {
	((void(ECALL *)(MotionModel*, const char*, bool, bool))addr::MotionModel_PushMotion)(m, name, a, b);
}
inline void MotionModel_PopMotion(MotionModel* m) {
	((void(ECALL *)(MotionModel*))addr::MotionModel_PopMotion)(m);
}
inline const char* MotionModel_GetCurrentMotionName(MotionModel* m) {
	return ((const char*(ECALL *)(MotionModel*))addr::MotionModel_GetCurrentMotionName)(m);
}

// ObjectMgr holds a std::vector<Object*> at offsets +0 (begin) / +8 (end).
inline ObjectMgr* ObjectMgr_i() {
	return ((ObjectMgr*(*)())addr::ObjectMgr_i)();
}
template <class Fn>
inline void ForEachObject(Fn fn) {
	char* mgr = (char*)ObjectMgr_i();
	if (!mgr) return;
	Object** begin = *(Object***)(mgr + 0);
	Object** end   = *(Object***)(mgr + sizeof(void*));   // std::vector end ptr: +8 (x64) / +4 (win32)
	for (Object** p = begin; p && p < end; ++p) fn(*p);
}

inline bool World_IsInMainMenu() {
	return ((bool(*)())addr::World_IsInMainMenu)();
}
// the GraphicsEngine singleton. Linux: an accessor function. Win32: a global variable
// holding the GE pointer (DAT_00ee3db0) - there is no accessor, so deref the global.
inline void* GE_instance() {
#ifdef _WIN32
	return addr::GraphicsEngine_i ? *(void**)addr::GraphicsEngine_i : nullptr;
#else
	return ((void*(*)())addr::GraphicsEngine_i)();
#endif
}
inline int ScreenWidth() {
	char* g = (char*)GE_instance();
	return g ? *(int*)(g + 0x40) : 0;
}
inline int ScreenHeight() {
	char* g = (char*)GE_instance();
	return g ? *(int*)(g + 0x44) : 0;
}
inline void DrawText(int x, int y, const char* text, Color c = Color()) {
	((void(*)(int, int, const char*, const Color&))addr::printText)(x, y, text, c);
}
// warning: dir is the baseline DIRECTION not a scale - keep horizontal {1,0} or text rotates; dirx<1 shrinks horizontally.
inline void DrawTextSized(int x, int y, const char* text, int size,
                          Color c = Color(), int style = STYLE_NORMAL, float dirx = 1.0f) {
	EString s(text ? text : "");   // std::string on Linux, MSVC-layout on Win32 (same call site)
	Vector2 pos{(float)x, (float)y}, dir{dirx, 0.0f};
	((void(*)(const EString&, int, int, Color, Vector2, bool, const Vector2&))
	 addr::TextPrinter_DrawString)(s, size, style, c, pos, false, dir);
}
inline void DrawTextOutlined(int x, int y, const char* text, int size,
                             Color c = Color(), Color shadow = Color(0, 0, 0, 200),
                             int style = STYLE_NORMAL) {
	DrawTextSized(x + 2, y + 2, text, size, shadow, style);
	DrawTextSized(x, y, text, size, c, style);
}

inline void* GraphicsEngine_i() { return GE_instance(); }
// GraphicsEngine geometry funcs swap R<->B internally; pre-swap so our RGBA shows correctly.
inline Color swab(Color c) { return Color(c.b, c.g, c.r, c.a); }
inline void DrawLine(Vector2 a, Vector2 b, Color c, float width = 1.0f) {
	void* g = GraphicsEngine_i(); if (!g) return;
	Color s = swab(c);
	// Win32 GraphicsEngine::DrawLine is __thiscall and RET 0x10 -> 4 stack args incl. width (callee-cleanup)
	((void(ECALL*)(void*, const Vector2&, const Vector2&, const Color&, float))
	 addr::GraphicsEngine_DrawLine)(g, a, b, s, width);
}
inline void FillRect(int x, int y, int w, int h, Color c) {
	void* g = GraphicsEngine_i(); if (!g) return;
	Color s = swab(c);
	Vector2 a{(float)x, (float)y}, b{(float)(x + w), (float)(y + h)};
	((void(ECALL*)(void*, const Vector2&, const Vector2&, const Color&))
	 addr::GraphicsEngine_DrawSquare)(g, a, b, s);
}
inline void FillCircle(int x, int y, float r, Color c, int segs = 24) {
	void* g = GraphicsEngine_i(); if (!g) return;
	if (!addr::GraphicsEngine_DrawCircleFilled) return;  // absent on Win32 (no procedural circle)
	Color s = swab(c);
	Vector2 p{(float)x, (float)y};
	((void(ECALL*)(void*, const Vector2&, float, const Color&, int))
	 addr::GraphicsEngine_DrawCircleFilled)(g, p, r, s, segs);
}
// built from filled bars - DrawLine ignores width
inline void DrawRect(int x, int y, int w, int h, Color c, float t = 2.0f) {
	int ti = (int)t; if (ti < 1) ti = 1;
	FillRect(x, y, w, ti, c);
	FillRect(x, y + h - ti, w, ti, c);
	FillRect(x, y, ti, h, c);
	FillRect(x + w - ti, y, ti, h, c);
}

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

inline void* LoadSprite(const char* path, int format = 0) {
	static std::unordered_map<std::string, void*> cache;
	auto it = cache.find(path);
	if (it != cache.end()) return it->second;
	void* sm = ((void*(*)())addr::SpriteManager_i)();
	void* sprite = nullptr;
	if (sm) {
		struct { void* sprite; void* ctrl; } holder = { nullptr, nullptr };
		std::string p = path;
		((void(*)(void*, void*, const std::string&, int))addr::SpriteManager_Load)(&holder, sm, p, format);
		sprite = holder.sprite;     // keep the ref (cache it; persistent draw)
	}
	cache[path] = sprite;
	return sprite;
}
inline int  SpriteWidth(void* s)  { return s ? (int)((unsigned(ECALL*)(void*))addr::Sprite_GetWidth)(s)  : 0; }
inline int  SpriteHeight(void* s) { return s ? (int)((unsigned(ECALL*)(void*))addr::Sprite_GetHeight)(s) : 0; }

inline void DrawSpriteAt(void* sprite, int x, int y, Color tint = Color()) {
	void* ge = GraphicsEngine_i();
	if (!ge || !sprite) return;
	Vector2 uv0{0.0f, 0.0f}, uv1{1.0f, 1.0f};
	((void(ECALL*)(void*, Vector2&, Vector2&))addr::Sprite_GetDiffuseUV)(sprite, uv0, uv1);
	Vector2 pos{(float)x, (float)y};
	((void(ECALL*)(void*, void*, const Vector2&, const Vector2&, const Vector2&, const Color&))
	 addr::GraphicsEngine_DrawSprite)(ge, sprite, pos, uv0, uv1, tint);
}

inline bool DrawImage(const char* path, int x, int y, Color tint = Color()) {
	void* sprite = LoadSprite(path);
	if (!sprite) return false;
	DrawSpriteAt(sprite, x, y, tint);
	return true;
}
inline bool DrawImageHUD(const char* path, int x, int y, Color tint = Color()) {
	if (((bool(*)())addr::World_IsSimulating)()) {
		Vector2 z{0.0f, 0.0f};
		((void(*)(const Vector2&, const Vector2&))addr::World_SetGFXViewOffset)(z, z);
	}
	return DrawImage(path, x, y, tint);
}

inline void* LoadAnim(const char* path) {
	static std::unordered_map<std::string, void*> cache;
	auto it = cache.find(path);
	if (it != cache.end()) return it->second;
	void* a = ((void*(*)(unsigned long))addr::Animation_operator_new)(0x78);
	if (a) ((void(*)(void*, const char*, bool))addr::Animation_ctor)(a, path, true);
	cache[path] = a;
	return a;
}
inline float AnimFrameDuration(void* a) { return a ? *(float*)((char*)a + 0x30) : 0.0f; }
inline int AnimFrameCount(void* a) { return a ? (int)((unsigned(*)(void*))addr::Animation_FrameCount)(a) : 0; }

inline bool DrawAnim(const char* path, int x, int y, float dt, float fps = 0.0f, Color tint = Color()) {
	void* a = LoadAnim(path);
	if (!a) return false;
	unsigned frames = (unsigned)AnimFrameCount(a);
	if (frames > 1) {
		if (fps <= 0.0f) {                      // native rate from the anim's frame duration
			float d = AnimFrameDuration(a);
			fps = (d > 0.0001f) ? 1.0f / d : 12.0f;
		}
		static std::unordered_map<std::string, double>   acc;
		static std::unordered_map<std::string, unsigned> idx;
		double step = 1.0 / fps;
		double& t = acc[path]; t += dt;
		while (t >= step) { t -= step; idx[path] = (idx[path] + 1) % frames; }
		((void(*)(void*, unsigned))addr::Animation_SetCurrentFrame)(a, idx[path]);
	}
	void* sprite = ((void*(*)(void*))addr::Animation_GetCurrentFrame)(a);
	if (!sprite) return false;
	DrawSpriteAt(sprite, x, y, tint);
	return true;
}

inline const char* Localize(const char* id) {
	void* sp = *(void**)addr::StringPool_instance;
	if (!sp) return id;
	return ((const char*(ECALL*)(void*, const char*))addr::StringPool_Resolve)(sp, id);   // __thiscall (this=StringPool*)
}

// ---- remaining binding statics (auto-wrapped; return types best-effort) ----
inline unsigned Anim_GetCurrentFrameIndex(void* a) { return ((unsigned(*)(void*))addr::Anim_GetCurrentFrameIndex)(a); }
inline void Anim_SetCurrentFrameIndex(void* a, unsigned int b) { ((void(*)(void*, unsigned int))addr::Anim_SetCurrentFrameIndex)(a, b); }
inline void Creator_Undo() { ((void(*)())addr::Creator_Undo)(); }
inline void Misc_BindKey(const char* a, const char* b) { ((void(*)(const char*, const char*))addr::Misc_BindKey)(a, b); }
inline void Misc_PauseProfile() { ((void(*)())addr::Misc_PauseProfile)(); }
inline void Misc_Profile() { ((void(*)())addr::Misc_Profile)(); }
inline const char* Object_GetBlueprintName(Object* a) { return ((const char*(*)(Object*))addr::Object_GetBlueprintName)(a); }
inline void Object_HoldObject(Object* a, Object* b) { ((void(*)(Object*, Object*))addr::Object_HoldObject)(a, b); }
inline bool Object_IsHolding(Object* a, Object* b) { return ((bool(*)(Object*, Object*))addr::Object_IsHolding)(a, b); }
inline bool Object_IsPhysicsEnabled(Object* a) { return ((bool(*)(Object*))addr::Object_IsPhysicsEnabled)(a); }
inline bool Object_IsRolling(Object* a) { return ((bool(*)(Object*))addr::Object_IsRolling)(a); }
inline bool Object_IsWalker(Object* a) { return ((bool(*)(Object*))addr::Object_IsWalker)(a); }
inline void Object_ReleaseAll(Object* a) { ((void(*)(Object*))addr::Object_ReleaseAll)(a); }
inline void Object_SetAltCollision(Object* a, const char* b) { ((void(*)(Object*, const char*))addr::Object_SetAltCollision)(a, b); }
inline void World_AddBackgroundImage(const char* a, const Vector2& b, const Vector2& c, float d, const Color& e, float f) { ((void(*)(const char*, const Vector2&, const Vector2&, float, const Color&, float))addr::World_AddBackgroundImage)(a, b, c, d, e, f); }
inline void World_AddForegroundImage(const char* a, const Vector2& b, const Vector2& c, float d, const Color& e, float f) { ((void(*)(const char*, const Vector2&, const Vector2&, float, const Color&, float))addr::World_AddForegroundImage)(a, b, c, d, e, f); }
inline void World_Alert(const Vector2& a, float b) { ((void(*)(const Vector2&, float))addr::World_Alert)(a, b); }
inline bool World_AnimationsEqual(void* a, void* b) { return ((bool(*)(void*, void*))addr::World_AnimationsEqual)(a, b); }
inline void World_CheckGoal(Object* a) { ((void(*)(Object*))addr::World_CheckGoal)(a); }
inline void World_CopyItem() { ((void(*)())addr::World_CopyItem)(); }
inline void World_CreateLight(const Vector2& a, const Color& b, float c, float d) { ((void(*)(const Vector2&, const Color&, float, float))addr::World_CreateLight)(a, b, c, d); }
inline double World_GetDeterministicDouble() { return ((double(*)())addr::World_GetDeterministicDouble)(); }
inline int World_GetDeterministicIntRange(int a, int b) { return ((int(*)(int, int))addr::World_GetDeterministicIntRange)(a, b); }
inline void World_IncrementStat(const char* a) { ((void(*)(const char*))addr::World_IncrementStat)(a); }
inline bool World_IsInLevelEditor() { return ((bool(*)())addr::World_IsInLevelEditor)(); }
inline bool World_LightExists(unsigned short a) { return ((bool(*)(unsigned short))addr::World_LightExists)(a); }
inline void World_MerchMerchPickUp() { ((void(*)())addr::World_MerchMerchPickUp)(); }
inline void World_MoveLight(unsigned short a, const Vector2& b) { ((void(*)(unsigned short, const Vector2&))addr::World_MoveLight)(a, b); }
inline void World_PasteItem() { ((void(*)())addr::World_PasteItem)(); }
inline void World_RemoveLight(unsigned short a) { ((void(*)(unsigned short))addr::World_RemoveLight)(a); }
inline void World_SaveLevel() { ((void(*)())addr::World_SaveLevel)(); }
inline Object* World_ScanForClosestEdible(Object* a, const Vector2& b, float c, float d, bool e) { return ((Object*(*)(Object*, const Vector2&, float, float, bool))addr::World_ScanForClosestEdible)(a, b, c, d, e); }
inline void World_SelectAll() { ((void(*)())addr::World_SelectAll)(); }
inline void World_SetFPS(int a) { ((void(*)(int))addr::World_SetFPS)(a); }
inline void World_SetGFXViewOffset(const Vector2& a, const Vector2& b) { ((void(*)(const Vector2&, const Vector2&))addr::World_SetGFXViewOffset)(a, b); }
inline void World_SetGlobalAmbient(const Color& a) { ((void(*)(const Color&))addr::World_SetGlobalAmbient)(a); }
inline void World_SetLevelModulate(const Color& a) { ((void(*)(const Color&))addr::World_SetLevelModulate)(a); }
inline void World_SetLightAttenuation(unsigned short a, float b, float c, float d) { ((void(*)(unsigned short, float, float, float))addr::World_SetLightAttenuation)(a, b, c, d); }
inline void World_SetTextHint(const char* a, const Vector2& b) { ((void(*)(const char*, const Vector2&))addr::World_SetTextHint)(a, b); }
inline void World_SetWinFlag() { ((void(*)())addr::World_SetWinFlag)(); }
inline void World_ShowClickables() { ((void(*)())addr::World_ShowClickables)(); }
inline void World_ShowGoalBox(const char* a, const Vector2& b, const char* c, const char* d, const char* e) { ((void(*)(const char*, const Vector2&, const char*, const char*, const char*))addr::World_ShowGoalBox)(a, b, c, d, e); }
inline void World_ShowGoalBoxFX(const char* a, const Vector2& b) { ((void(*)(const char*, const Vector2&))addr::World_ShowGoalBoxFX)(a, b); }
inline void World_ShowPuzzleAsNew() { ((void(*)())addr::World_ShowPuzzleAsNew)(); }
inline void World_ShowSolutionTime(float a) { ((void(*)(float))addr::World_ShowSolutionTime)(a); }
inline void World_ShowTutorialDialog(const char* a) { ((void(*)(const char*))addr::World_ShowTutorialDialog)(a); }

// ===== object extensions =====
// each accessor returns null if the object lacks that extension; always null-check.
inline PositionExtension* Object_GetPositionExtension(Object* o) { return ((PositionExtension*(ECALL *)(Object*))addr::Object_GetPositionExtension)(o); }
inline SuckableExtension* Object_GetSuckableExtension(Object* o) { return ((SuckableExtension*(ECALL *)(Object*))addr::Object_GetSuckableExtension)(o); }
inline WalkingExtension* Object_GetWalkingExtension(Object* o) { return ((WalkingExtension*(ECALL *)(Object*))addr::Object_GetWalkingExtension)(o); }
inline ThwackerExtension* Object_GetThwackerExtension(Object* o) { return ((ThwackerExtension*(ECALL *)(Object*))addr::Object_GetThwackerExtension)(o); }
inline EmotionPlatformExtension* Object_GetEmotionPlatformExtension(Object* o) { return ((EmotionPlatformExtension*(ECALL *)(Object*))addr::Object_GetEmotionPlatformExtension)(o); }
inline EdibleExtension* Object_GetEdibleExtension(Object* o) { return ((EdibleExtension*(ECALL *)(Object*))addr::Object_GetEdibleExtension)(o); }
inline EmotionExtension* Object_GetEmotionExtension(Object* o) { return ((EmotionExtension*(ECALL *)(Object*))addr::Object_GetEmotionExtension)(o); }
inline FlyingExtension* Object_GetFlyingExtension(Object* o) { return ((FlyingExtension*(ECALL *)(Object*))addr::Object_GetFlyingExtension)(o); }
inline LightingExtension* Object_GetLightingExtension(Object* o) { return ((LightingExtension*(ECALL *)(Object*))addr::Object_GetLightingExtension)(o); }
inline HoldingExtension* Object_GetHoldingExtension(Object* o) { return ((HoldingExtension*(ECALL *)(Object*))addr::Object_GetHoldingExtension)(o); }
inline RollingExtension* Object_GetRollingExtension(Object* o) { return ((RollingExtension*(ECALL *)(Object*))addr::Object_GetRollingExtension)(o); }
// physics has no templated getter; it sits at a fixed Object slot
inline PhysicsExtension* Object_GetPhysicsExtension(Object* o) {
#ifdef _WIN32
	(void)o; return nullptr;   // 0x90 is the x64 Object slot; the win32 (4-byte-ptr) offset + MSVC deque walk are unported
#else
	return o ? *(PhysicsExtension**)((char*)o + 0x90) : nullptr;
#endif
}

inline void WalkingExtension_SetWalkSpeed(WalkingExtension* w, float s) { ((void(ECALL *)(WalkingExtension*, float))addr::WalkingExtension_SetWalkSpeed)(w, s); }
inline void WalkingExtension_SetActive(WalkingExtension* w, bool a) { ((void(ECALL *)(WalkingExtension*, bool))addr::WalkingExtension_SetActive)(w, a); }
inline unsigned long WalkingExtension_GetState(WalkingExtension* w) { return ((unsigned long(ECALL *)(WalkingExtension*))addr::WalkingExtension_GetState)(w); }
inline void WalkingExtension_StartWalking(WalkingExtension* w) { ((void(ECALL *)(WalkingExtension*))addr::WalkingExtension_StartWalking)(w); }
inline void WalkingExtension_StopWalking(WalkingExtension* w) { ((void(ECALL *)(WalkingExtension*))addr::WalkingExtension_StopWalking)(w); }
inline void WalkingExtension_KnockDown(WalkingExtension* w, const Vector2& dir) { ((void(ECALL *)(WalkingExtension*, const Vector2&))addr::WalkingExtension_KnockDown)(w, dir); }
inline void WalkingExtension_SetNoWalkFrame(WalkingExtension* w, int f) { ((void(ECALL *)(WalkingExtension*, int))addr::WalkingExtension_SetNoWalkFrame)(w, f); }
inline void WalkingExtension_ForceReset(WalkingExtension* w) { ((void(ECALL *)(WalkingExtension*))addr::WalkingExtension_ForceReset)(w); }
inline void WalkingExtension_Reset(WalkingExtension* w) { ((void(ECALL *)(WalkingExtension*))addr::WalkingExtension_Reset)(w); }

inline void ThwackerExtension_SetThwackSpeed(ThwackerExtension* t, float s) { ((void(ECALL *)(ThwackerExtension*, float))addr::ThwackerExtension_SetThwackSpeed)(t, s); }
inline bool ThwackerExtension_IsThwacking(ThwackerExtension* t) { return ((bool(ECALL *)(ThwackerExtension*))addr::ThwackerExtension_IsThwacking)(t); }
inline Vector2 ThwackerExtension_GetCentre(ThwackerExtension* t) {
#ifdef _WIN32
	Vector2 r; ((void(ECALL *)(ThwackerExtension*, Vector2*))addr::ThwackerExtension_GetCentre)(t, &r); return r;
#else
	return ((Vector2(*)(ThwackerExtension*))addr::ThwackerExtension_GetCentre)(t);
#endif
}

inline bool EdibleExtension_GetEaten(EdibleExtension* e) { return ((bool(ECALL *)(EdibleExtension*))addr::EdibleExtension_GetEaten)(e); }
inline unsigned EdibleExtension_GetEater(EdibleExtension* e) { return ((unsigned(ECALL *)(EdibleExtension*))addr::EdibleExtension_GetEater)(e); }
inline bool EdibleExtension_IsEatenBy(EdibleExtension* e, unsigned id) { return ((bool(ECALL *)(EdibleExtension*, unsigned))addr::EdibleExtension_IsEatenBy)(e, id); }

inline bool LightingExtension_IsLit(LightingExtension* l) { return ((bool(ECALL *)(LightingExtension*))addr::LightingExtension_IsLit)(l); }

inline bool SuckableExtension_WasRecentlySucked(SuckableExtension* s) { return ((bool(ECALL *)(SuckableExtension*))addr::SuckableExtension_WasRecentlySucked)(s); }
inline void SuckableExtension_SetSucked(SuckableExtension* s) { ((void(ECALL *)(SuckableExtension*))addr::SuckableExtension_SetSucked)(s); }

inline bool PositionExtension_IsForeground(PositionExtension* p) { return ((bool(ECALL *)(PositionExtension*))addr::PositionExtension_IsForeground)(p); }
inline bool PositionExtension_IsBackground(PositionExtension* p) { return ((bool(ECALL *)(PositionExtension*))addr::PositionExtension_IsBackground)(p); }
inline void PositionExtension_SetIsForeground(PositionExtension* p, bool v) { ((void(ECALL *)(PositionExtension*, bool))addr::PositionExtension_SetIsForeground)(p, v); }
inline void PositionExtension_SetIsBackground(PositionExtension* p, bool v) { ((void(ECALL *)(PositionExtension*, bool))addr::PositionExtension_SetIsBackground)(p, v); }

inline bool RollingExtension_IsRolling(RollingExtension* r) { return ((bool(ECALL *)(RollingExtension*))addr::RollingExtension_IsRolling)(r); }

inline bool HoldingExtension_IsHolding(HoldingExtension* h, Object* o) { return ((bool(ECALL *)(HoldingExtension*, Object*))addr::HoldingExtension_IsHolding)(h, o); }
inline bool HoldingExtension_IsHoldingAny(HoldingExtension* h) { return ((bool(ECALL *)(HoldingExtension*))addr::HoldingExtension_IsHoldingAny)(h); }
inline void HoldingExtension_HoldObject(HoldingExtension* h, Object* o) { ((void(ECALL *)(HoldingExtension*, Object*))addr::HoldingExtension_HoldObject)(h, o); }
inline void HoldingExtension_ReleaseAll(HoldingExtension* h) { ((void(ECALL *)(HoldingExtension*))addr::HoldingExtension_ReleaseAll)(h); }
inline void HoldingExtension_ReleaseObject(HoldingExtension* h, Object* o) { ((void(ECALL *)(HoldingExtension*, Object*))addr::HoldingExtension_ReleaseObject)(h, o); }
// GetHolds returns a std::vector<Object*> const& (pointer to {begin,end,cap}); walk it
template <class Fn>
inline void ForEachHeld(HoldingExtension* h, Fn fn) {
	if (!h) return;
	char* vec = (char*)((void*(ECALL *)(HoldingExtension*))addr::HoldingExtension_GetHolds)(h);
	if (!vec) return;
	Object** begin = *(Object***)(vec + 0);
	Object** end   = *(Object***)(vec + sizeof(void*));   // std::vector end ptr: +8 (x64) / +4 (win32)
	for (Object** p = begin; p && p < end; ++p) fn(*p);
}

inline void FlyingExtension_SetState(FlyingExtension* f, unsigned long s) { ((void(ECALL *)(FlyingExtension*, unsigned long))addr::FlyingExtension_SetState)(f, s); }
inline unsigned long FlyingExtension_GetState(FlyingExtension* f) { return ((unsigned long(ECALL *)(FlyingExtension*))addr::FlyingExtension_GetState)(f); }

inline bool EmotionExtension_RecentlyChanged(EmotionExtension* e) { return ((bool(ECALL *)(EmotionExtension*))addr::EmotionExtension_RecentlyChanged)(e); }
inline const char* EmotionExtension_GetEmotionName(EmotionExtension* e) { return ((const char*(ECALL *)(EmotionExtension*))addr::EmotionExtension_GetEmotionName)(e); }
inline void EmotionExtension_SetEmotionName(EmotionExtension* e, const char* n) { ((void(ECALL *)(EmotionExtension*, const char*))addr::EmotionExtension_SetEmotionName)(e, n); }

inline unsigned EmotionPlatformExtension_GetCurrentEmotion(EmotionPlatformExtension* e) { return ((unsigned(ECALL *)(EmotionPlatformExtension*))addr::EmotionPlatformExtension_GetCurrentEmotion)(e); }
inline bool EmotionPlatformExtension_MatchesCurrentEmotion(EmotionPlatformExtension* e) { return ((bool(ECALL *)(EmotionPlatformExtension*))addr::EmotionPlatformExtension_MatchesCurrentEmotion)(e); }
inline void EmotionPlatformExtension_SetEmotion(EmotionPlatformExtension* e, unsigned id) { ((void(ECALL *)(EmotionPlatformExtension*, unsigned))addr::EmotionPlatformExtension_SetEmotion)(e, id); }

// iterate an object's accumulated collision contacts. matches the Lua GetCollisionReports path:
// reading enables accumulation, so the engine keeps refilling the deque each step.
template <class Fn>
inline void ForEachCollision(Object* o, Fn fn) {
#ifdef _WIN32
	(void)o; (void)fn; return;   // walker below is libstdc++ deque + 8-byte unsigned long; MSVC deque port pending
#else
	PhysicsExtension* phys = Object_GetPhysicsExtension(o);
	if (!phys) return;
	// std::deque<CollisionReport>; element is 0x28 bytes, libstdc++ chunk holds 512/0x28 = 12 per node
	char* dq = (char*)((void*(ECALL *)(PhysicsExtension*))addr::PhysicsExtension_GetAccumulate)(phys);
	if (!dq) return;
	const long bufsz = 12;
	char*  cur     = *(char**)(dq + 0x10);  // start._M_cur
	char*  last    = *(char**)(dq + 0x20);  // start._M_last
	char** node    = *(char***)(dq + 0x28); // start._M_node
	char*  fin_cur = *(char**)(dq + 0x30);  // finish._M_cur
	while (cur != fin_cur) {
		fn(*(const CollisionReport*)cur);
		cur += sizeof(CollisionReport);
		if (cur == last) { ++node; cur = *node; last = *node + bufsz * sizeof(CollisionReport); }
	}
#endif
}

// pure math on our Vector2 (no engine call needed)
inline float VecDot(const Vector2& a, const Vector2& b) { return a.x*b.x + a.y*b.y; }
inline float VecLenSqr(const Vector2& v) { return v.x*v.x + v.y*v.y; }
inline float VecLen(const Vector2& v) { float d = VecLenSqr(v); return d > 0 ? __builtin_sqrtf(d) : 0; }
inline Vector2 VecNormalize(const Vector2& v) { float l = VecLen(v); return l > 0 ? Vector2{v.x/l, v.y/l} : Vector2{0,0}; }

} // namespace Eets
