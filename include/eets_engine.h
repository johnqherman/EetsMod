// ABI: Vector2 { float x, y } is one SSE eightbyte in xmm0 - matching the type matches the ABI
#pragma once
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>
#include <string>
#include <unordered_map>
#ifdef _WIN32
#include "eets_addr_win.h"   // PE32 i386 RVAs, resolved at runtime
#else
#include "eets_addr.h"       // Linux x86-64 absolute addresses
#endif

// Win32 member fns use __thiscall (this in ECX); casts must carry it. empty on Linux SysV
#ifdef _WIN32
#  define ECALL __thiscall
#else
#  define ECALL
#endif

#include <type_traits>

namespace Eets {

// null-safe ABI-correct call path. EC = member (__thiscall on Win32, plain on Linux SysV),
// FC = cdecl free fn. sig is a compile-time type so the ABI is preserved with no decay
// unresolved addr (0) -> call skipped, default R returned (no-op, not a jump to 0)
template <class F, bool Member> struct EngineCall;
template <class R, class... A> struct EngineCall<R(A...), true> {
	uintptr_t fn;
	R operator()(A... a) const {
		if constexpr (std::is_void_v<R>) { if (fn) ((R(ECALL*)(A...))fn)(a...); }
		else return fn ? ((R(ECALL*)(A...))fn)(a...) : R{};
	}
};
template <class R, class... A> struct EngineCall<R(A...), false> {
	uintptr_t fn;
	R operator()(A... a) const {
		if constexpr (std::is_void_v<R>) { if (fn) ((R(*)(A...))fn)(a...); }
		else return fn ? ((R(*)(A...))fn)(a...) : R{};
	}
};
template <class F> inline EngineCall<F, true>  EC(uintptr_t fn) { return {fn}; }   // member (__thiscall on Win32)
template <class F> inline EngineCall<F, false> FC(uintptr_t fn) { return {fn}; }   // free function (cdecl)

// valid for this exact build; loader warns if running BuildID differs
constexpr const char* EXPECTED_BUILDID = "e81cc5504d3ef03324805df3e9fc508c1bf8c628";

struct Vector2 { float x, y; };
struct Color { unsigned char r, g, b, a;          // 4-byte packed (engine abi)
	Color(unsigned char R=255, unsigned char G=255, unsigned char B=255, unsigned char A=255)
		: r(R), g(G), b(B), a(A) {}
};
struct Object;        // opaque engine type
struct MotionModel;   // opaque
struct ObjectMgr;     // opaque

// extension types: object behavior components, fetched via Get*Extension below
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

// engine string ABI. engine takes std::string by const-ref (reads only; caller owns)
// Linux: libstdc++ std::string. Win32: MSVC layout - mingw's is NOT compatible, passing one crashes
// EString models the right per-platform ABI at the same call site
#ifdef _WIN32
// MSVC 32-bit std::string (release): union{char buf[16]; char* ptr}@0x00, size@0x10, cap@0x14
// SSO when cap <= 0xf (inline); else _Ptr. by const-ref so only field layout matters, not sizeof
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
// FontPrintStyles -> typeface. STYLE_GEEK (geekabyte) is the in-game gui font
enum FontStyle { STYLE_KOMIKA = 1, STYLE_GEEK = 2, STYLE_NORMAL = 3, STYLE_BRADY = 4 };

inline Vector2 World_GetGravity() {
#ifdef _WIN32
	// 32-bit MSVC returns the 8-byte struct via hidden sret ptr: void World_GetGravity(Vector2* out)
	Vector2 v; FC<void(Vector2*)>(addr::World_GetGravity)(&v); return v;
#else
	return FC<Vector2()>(addr::World_GetGravity)();
#endif
}
inline void World_SetGravity(Vector2 g, int mode = 0) {
	FC<void(Vector2, int)>(addr::World_SetGravity)(g, mode);
}
inline Object* World_CreateObject(const char* name, Vector2 pos) {
	return FC<Object*(const char*, Vector2)>(addr::World_CreateObject)(name, pos);
}
inline Object* World_GetEets() {
	return FC<Object*()>(addr::World_GetEets)();
}
inline void World_CreateExplosion(const Vector2& pos, float radius) {
	FC<void(const Vector2&, float)>(addr::World_CreateExplosion)(pos, radius);
}
inline void World_CreateExplosionSpecial() {
	FC<void()>(addr::World_CreateExplosionSpecial)();
}
inline void Object_ApplyImpulse(Object* o, const Vector2& impulse) {
	FC<void(Object*, const Vector2&)>(addr::Object_ApplyImpulse)(o, impulse);
}
inline void Object_EnablePhysics(Object* o, bool enabled) {
	FC<void(Object*, bool)>(addr::Object_EnablePhysics)(o, enabled);
}
inline Object* World_GetObjectByID(unsigned long id) {
	return FC<Object*(unsigned long)>(addr::World_GetObjectByID)(id);
}
inline void World_CreateEffect(const char* name, Vector2 pos) {
	FC<void(const char*, Vector2)>(addr::World_CreateEffect)(name, pos);
}
inline void World_Scare(const Vector2& pos, float radius, int strength) {
	FC<void(const Vector2&, float, int)>(addr::World_Scare)(pos, radius, strength);
}
inline void World_SetGameSpeed(unsigned long speed) {   // 0 = paused, 1 = normal, 2 = fast
	FC<void(unsigned long)>(addr::World_SetGameSpeed)(speed);
}
inline void World_ChangeEmotion(unsigned long objHash, unsigned int emotion) {
	FC<void(unsigned long, unsigned int)>(addr::World_ChangeEmotion)(objHash, emotion);
}
inline void Sound_CreateSound(const char* name, bool loop, float vol, const Vector2& pos) {
	// sret: returns SoundHandle by value
	char handle[16] = {0};
	FC<void(void*, const char*, bool, float, const Vector2&)>(addr::Sound_CreateSound)(handle, name, loop, vol, pos);
}
inline void PlaySound(const char* name, float vol = 0.0f) {
	Vector2 z{0.0f, 0.0f};
	Sound_CreateSound(name, false, vol, z);
}

inline Vector2 Object_GetPosition(Object* o) {
	// Object::GetPosition reads through the object's Model (Object+0x38, same offset on Win32 + Linux64)
	// and the engine returns a Vector2& - i.e. a POINTER in (E/R)AX, not a Vector2 by value - so it must
	// be dereferenced on BOTH platforms. The old Linux path read the return as a value, reinterpreting
	// the pointer's 64 bits as two floats (x came out ~1e11, y ~0 from the heap address's high half).
	// A null Model (non-visual object, or before the Model attaches) makes the engine deref garbage.
	if (!o || !*(void**)((char*)o + 0x38)) return Vector2{0.0f, 0.0f};
#ifdef _WIN32
	Vector2* p = EC<Vector2*(Object*)>(addr::Object_GetPosition)(o);   // __thiscall (this in ECX)
#else
	Vector2* p = FC<Vector2*(Object*)>(addr::Object_GetPosition)(o);   // SysV (this is the first arg)
#endif
	return p ? *p : Vector2{0.0f, 0.0f};
}
inline Vector2 Object_GetVelocity(Object* o) {
	// velocity is computed (pos - prevpos) and returned by value; guard the Model it reads through
	if (!o || !*(void**)((char*)o + 0x38)) return Vector2{0.0f, 0.0f};
#ifdef _WIN32
	Vector2 r{0.0f, 0.0f}; EC<void(Object*, Vector2*)>(addr::Object_GetVelocity)(o, &r); return r;   // sret
#else
	return FC<Vector2(Object*)>(addr::Object_GetVelocity)(o);
#endif
}
inline unsigned long Object_GetID(Object* o) {
	return EC<unsigned long(Object*)>(addr::Object_GetID)(o);
}
inline MotionModel* Object_GetMotionModel(Object* o) {
	return EC<MotionModel*(Object*)>(addr::Object_GetMotionModel)(o);
}
inline void Object_SetPosition(Object* o, const Vector2& p)   { EC<void(Object*, const Vector2&)>(addr::Object_SetPosition)(o, p); }
inline void Object_ForcePosition(Object* o, const Vector2& p) { EC<void(Object*, const Vector2&)>(addr::Object_ForcePosition)(o, p); }
inline void Object_SetFacing(Object* o, const Vector2& f)     { EC<void(Object*, const Vector2&)>(addr::Object_SetFacing)(o, f); }
inline Vector2 Object_GetFacing(Object* o) {
	// engine returns &(Object+0x14) - a Vector2& (POINTER in (E/R)AX), not by value and not sret - so
	// dereference it on both platforms (was Win-sret / Linux-by-value, both wrong -> garbage facing).
	if (!o) return Vector2{0.0f, 0.0f};
#ifdef _WIN32
	Vector2* p = EC<Vector2*(Object*)>(addr::Object_GetFacing)(o);
#else
	Vector2* p = FC<Vector2*(Object*)>(addr::Object_GetFacing)(o);
#endif
	return p ? *p : Vector2{0.0f, 0.0f};
}
inline void Object_SetFlipped(Object* o, bool f)             { EC<void(Object*, bool)>(addr::Object_SetFlipped)(o, f); }
inline bool Object_GetFlipped(Object* o)                     { return EC<bool(Object*)>(addr::Object_GetFlipped)(o); }
inline void Object_KillMe(Object* o)                        { EC<void(Object*)>(addr::Object_KillMe)(o); }

inline void MotionModel_PushMotion(MotionModel* m, const char* name, bool a, bool b) {
	EC<void(MotionModel*, const char*, bool, bool)>(addr::MotionModel_PushMotion)(m, name, a, b);
}
inline void MotionModel_PopMotion(MotionModel* m) {
	EC<void(MotionModel*)>(addr::MotionModel_PopMotion)(m);
}
inline const char* MotionModel_GetCurrentMotionName(MotionModel* m) {
	return EC<const char*(MotionModel*)>(addr::MotionModel_GetCurrentMotionName)(m);
}

// ObjectMgr holds a std::vector<Object*> at offsets +0 (begin) / +8 (end)
inline ObjectMgr* ObjectMgr_i() {
	return FC<ObjectMgr*()>(addr::ObjectMgr_i)();
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
	return FC<bool()>(addr::World_IsInMainMenu)();
}
// GraphicsEngine singleton. Linux: accessor fn. Win32: no accessor - deref the global (DAT_00ee3db0)
inline void* GE_instance() {
#ifdef _WIN32
	return addr::GraphicsEngine_i ? *(void**)addr::GraphicsEngine_i : nullptr;
#else
	return FC<void*()>(addr::GraphicsEngine_i)();
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
	FC<void(int, int, const char*, const Color&)>(addr::printText)(x, y, text, c);
}
// dir is baseline DIRECTION not scale - keep {1,0} or text rotates; dirx<1 shrinks horizontally
inline void DrawTextSized(int x, int y, const char* text, int size,
                          Color c = Color(), int style = STYLE_NORMAL, float dirx = 1.0f) {
	EString s(text ? text : "");   // std::string on Linux, MSVC-layout on Win32 (same call site)
	Vector2 pos{(float)x, (float)y}, dir{dirx, 0.0f};
	FC<void(const EString&, int, int, Color, Vector2, bool, const Vector2&)>(addr::TextPrinter_DrawString)(s, size, style, c, pos, false, dir);
}
inline void DrawTextOutlined(int x, int y, const char* text, int size,
                             Color c = Color(), Color shadow = Color(0, 0, 0, 200),
                             int style = STYLE_NORMAL) {
	DrawTextSized(x + 2, y + 2, text, size, shadow, style);
	DrawTextSized(x, y, text, size, c, style);
}

inline void* GraphicsEngine_i() { return GE_instance(); }
// GraphicsEngine geometry funcs swap R<->B internally; pre-swap so our RGBA shows correctly
inline Color swab(Color c) { return Color(c.b, c.g, c.r, c.a); }
inline void DrawLine(Vector2 a, Vector2 b, Color c, float width = 1.0f) {
	void* g = GraphicsEngine_i(); if (!g) return;
	Color s = swab(c);
	// Win32 GraphicsEngine::DrawLine __thiscall, RET 0x10 -> 4 callee-cleanup stack args incl. width
	EC<void(void*, const Vector2&, const Vector2&, const Color&, float)>(addr::GraphicsEngine_DrawLine)(g, a, b, s, width);
}
inline void FillRect(int x, int y, int w, int h, Color c) {
	void* g = GraphicsEngine_i(); if (!g) return;
	Color s = swab(c);
	Vector2 a{(float)x, (float)y}, b{(float)(x + w), (float)(y + h)};
	EC<void(void*, const Vector2&, const Vector2&, const Color&)>(addr::GraphicsEngine_DrawSquare)(g, a, b, s);
}
inline void FillCircle(int x, int y, float r, Color c, int segs = 24) {
	void* g = GraphicsEngine_i(); if (!g) return;
#ifdef _WIN32
	// Win build has no filled-circle primitive; reproduce from FillRect scanlines to match Linux
	(void)segs;
	int ri = (int)(r + 0.5f); if (ri < 1) return;
	for (int dy = -ri; dy <= ri; ++dy) {
		int half = (int)(__builtin_sqrtf((float)(ri * ri - dy * dy)) + 0.5f);
		if (half > 0) FillRect(x - half, y + dy, half * 2, 1, c);
	}
#else
	Color s = swab(c);
	Vector2 p{(float)x, (float)y};
	EC<void(void*, const Vector2&, float, const Color&, int)>(addr::GraphicsEngine_DrawCircleFilled)(g, p, r, s, segs);
#endif
}
// built from filled bars - DrawLine ignores width
inline void DrawRect(int x, int y, int w, int h, Color c, float t = 2.0f) {
	int ti = (int)t; if (ti < 1) ti = 1;
	FillRect(x, y, w, ti, c);
	FillRect(x, y + h - ti, w, ti, c);
	FillRect(x, y, ti, h, c);
	FillRect(x + w - ti, y, ti, h, c);
}

inline void World_Pause()            { FC<void()>(addr::World_Pause)(); }
inline bool World_IsPaused()         { return FC<bool()>(addr::World_IsPaused)(); }
inline bool World_IsSimulating()     { return FC<bool()>(addr::World_IsSimulating)(); }
inline void World_SetMaximumSpeed(float s) { FC<void(float)>(addr::World_SetMaximumSpeed)(s); }
inline void Sound_PlayMusic(const char* name, bool loop) { FC<void(const char*, bool)>(addr::Sound_PlayMusic)(name, loop); }
inline void Sound_SetMusicVolume(long v) { FC<void(long)>(addr::Sound_SetMusicVolume)(v); }
inline void Misc_Print(const char* s) { FC<void(const char*)>(addr::Misc_Print)(s); }
inline void Misc_SaveScreenshot()    { FC<void()>(addr::Misc_SaveScreenshot)(); }
inline void Object_EnableCollisions(Object* o, bool e) { FC<void(Object*, bool)>(addr::Object_EnableCollisions)(o, e); }
inline void Object_CallFunction(Object* o, const char* fn) { FC<void(Object*, const char*)>(addr::Object_CallFunction)(o, fn); }
inline void Object_CreateEffect(Object* o, const char* fx) { FC<void(Object*, const char*)>(addr::Object_CreateEffect)(o, fx); }
inline unsigned long Object_GetBlueprintHash(Object* o) { return FC<unsigned long(Object*)>(addr::Object_GetBlueprintHash)(o); }

inline void* LoadSprite(const char* path, int format = 0) {
	static std::unordered_map<std::string, void*> cache;
	auto it = cache.find(path);
	if (it != cache.end()) return it->second;
	void* sm = FC<void*()>(addr::SpriteManager_i)();
	void* sprite = nullptr;
	if (sm) {
		struct { void* sprite; void* ctrl; } holder = { nullptr, nullptr };
#ifdef _WIN32
		// __thiscall + struct-return: `this`(sm) in ECX, hidden sret ptr is the first stack arg,
		// then the name (MSVC std::string via EString) and format
		EString p(path);
		EC<void(void*, void*, const EString&, int)>(addr::SpriteManager_Load)(sm, &holder, p, format);
#else
		// SysV sret method: hidden sret ptr first, then `this`, then args
		std::string p = path;
		FC<void(void*, void*, const std::string&, int)>(addr::SpriteManager_Load)(&holder, sm, p, format);
#endif
		sprite = holder.sprite;     // keep the ref (cache it; persistent draw)
	}
	cache[path] = sprite;
	return sprite;
}
inline int  SpriteWidth(void* s)  { return s ? (int)EC<unsigned(void*)>(addr::Sprite_GetWidth)(s)  : 0; }
inline int  SpriteHeight(void* s) { return s ? (int)EC<unsigned(void*)>(addr::Sprite_GetHeight)(s) : 0; }

inline void DrawSpriteAt(void* sprite, int x, int y, Color tint = Color()) {
	void* ge = GraphicsEngine_i();
	if (!ge || !sprite) return;
	Vector2 uv0{0.0f, 0.0f}, uv1{1.0f, 1.0f};
	EC<void(void*, Vector2&, Vector2&)>(addr::Sprite_GetDiffuseUV)(sprite, uv0, uv1);
	Vector2 pos{(float)x, (float)y};
	EC<void(void*, void*, const Vector2&, const Vector2&, const Vector2&, const Color&)>(addr::GraphicsEngine_DrawSprite)(ge, sprite, pos, uv0, uv1, tint);
}

inline bool DrawImage(const char* path, int x, int y, Color tint = Color()) {
	void* sprite = LoadSprite(path);
	if (!sprite) return false;
	DrawSpriteAt(sprite, x, y, tint);
	return true;
}
inline bool DrawImageHUD(const char* path, int x, int y, Color tint = Color()) {
	if (FC<bool()>(addr::World_IsSimulating)()) {
		Vector2 z{0.0f, 0.0f};
		FC<void(const Vector2&, const Vector2&)>(addr::World_SetGFXViewOffset)(z, z);
	}
	return DrawImage(path, x, y, tint);
}

inline void* LoadAnim(const char* path) {
	static std::unordered_map<std::string, void*> cache;
	auto it = cache.find(path);
	if (it != cache.end()) return it->second;
#ifdef _WIN32
	void* a = FC<void*(unsigned long)>(addr::Animation_operator_new)(0x54);     // Animation is 0x54 bytes on Win (0x78 on Linux)
	if (a) EC<void(void*, const char*, bool)>(addr::Animation_ctor)(a, path, true);  // ctor is __thiscall (this in ECX)
#else
	void* a = FC<void*(unsigned long)>(addr::Animation_operator_new)(0x78);
	if (a) FC<void(void*, const char*, bool)>(addr::Animation_ctor)(a, path, true);
#endif
	cache[path] = a;
	return a;
}
#ifdef _WIN32
inline float AnimFrameDuration(void* a) { return a ? *(float*)((char*)a + 0x18) : 0.0f; }   // duration @+0x18 on Win
#else
inline float AnimFrameDuration(void* a) { return a ? *(float*)((char*)a + 0x30) : 0.0f; }
#endif
inline int AnimFrameCount(void* a) { return a ? (int)FC<unsigned(void*)>(addr::Animation_FrameCount)(a) : 0; }

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
		FC<void(void*, unsigned)>(addr::Animation_SetCurrentFrame)(a, idx[path]);
	}
	void* sprite = FC<void*(void*)>(addr::Animation_GetCurrentFrame)(a);
	if (!sprite) return false;
	DrawSpriteAt(sprite, x, y, tint);
	return true;
}

inline const char* Localize(const char* id) {
	void* sp = *(void**)addr::StringPool_instance;
	if (!sp) return id;
	return EC<const char*(void*, const char*)>(addr::StringPool_Resolve)(sp, id);   // __thiscall (this=StringPool*)
}

// ---- remaining binding statics (auto-wrapped; return types best-effort) ----
inline unsigned Anim_GetCurrentFrameIndex(void* a) { return FC<unsigned(void*)>(addr::Anim_GetCurrentFrameIndex)(a); }
inline void Anim_SetCurrentFrameIndex(void* a, unsigned int b) { FC<void(void*, unsigned int)>(addr::Anim_SetCurrentFrameIndex)(a, b); }
inline void Creator_Undo() { FC<void()>(addr::Creator_Undo)(); }
inline void Misc_BindKey(const char* a, const char* b) { FC<void(const char*, const char*)>(addr::Misc_BindKey)(a, b); }
inline void Misc_PauseProfile() { FC<void()>(addr::Misc_PauseProfile)(); }
inline void Misc_Profile() { FC<void()>(addr::Misc_Profile)(); }
inline const char* Object_GetBlueprintName(Object* a) { return FC<const char*(Object*)>(addr::Object_GetBlueprintName)(a); }
inline void Object_HoldObject(Object* a, Object* b) { FC<void(Object*, Object*)>(addr::Object_HoldObject)(a, b); }
inline bool Object_IsHolding(Object* a, Object* b) { return FC<bool(Object*, Object*)>(addr::Object_IsHolding)(a, b); }
inline bool Object_IsPhysicsEnabled(Object* a) { return FC<bool(Object*)>(addr::Object_IsPhysicsEnabled)(a); }
inline bool Object_IsRolling(Object* a) { return FC<bool(Object*)>(addr::Object_IsRolling)(a); }
inline bool Object_IsWalker(Object* a) { return FC<bool(Object*)>(addr::Object_IsWalker)(a); }
inline void Object_ReleaseAll(Object* a) { FC<void(Object*)>(addr::Object_ReleaseAll)(a); }
inline void Object_SetAltCollision(Object* a, const char* b) { FC<void(Object*, const char*)>(addr::Object_SetAltCollision)(a, b); }
inline void World_AddBackgroundImage(const char* a, const Vector2& b, const Vector2& c, float d, const Color& e, float f) { FC<void(const char*, const Vector2&, const Vector2&, float, const Color&, float)>(addr::World_AddBackgroundImage)(a, b, c, d, e, f); }
inline void World_AddForegroundImage(const char* a, const Vector2& b, const Vector2& c, float d, const Color& e, float f) { FC<void(const char*, const Vector2&, const Vector2&, float, const Color&, float)>(addr::World_AddForegroundImage)(a, b, c, d, e, f); }
inline void World_Alert(const Vector2& a, float b) { FC<void(const Vector2&, float)>(addr::World_Alert)(a, b); }
inline bool World_AnimationsEqual(void* a, void* b) { return FC<bool(void*, void*)>(addr::World_AnimationsEqual)(a, b); }
inline void World_CheckGoal(Object* a) { FC<void(Object*)>(addr::World_CheckGoal)(a); }
inline void World_CopyItem() { FC<void()>(addr::World_CopyItem)(); }
inline void World_CreateLight(const Vector2& a, const Color& b, float c, float d) { FC<void(const Vector2&, const Color&, float, float)>(addr::World_CreateLight)(a, b, c, d); }
inline double World_GetDeterministicDouble() { return FC<double()>(addr::World_GetDeterministicDouble)(); }
inline int World_GetDeterministicIntRange(int a, int b) { return FC<int(int, int)>(addr::World_GetDeterministicIntRange)(a, b); }
inline void World_IncrementStat(const char* a) { FC<void(const char*)>(addr::World_IncrementStat)(a); }
inline bool World_IsInLevelEditor() { return FC<bool()>(addr::World_IsInLevelEditor)(); }
inline bool World_LightExists(unsigned short a) { return FC<bool(unsigned short)>(addr::World_LightExists)(a); }
inline void World_MerchMerchPickUp() { FC<void()>(addr::World_MerchMerchPickUp)(); }
inline void World_MoveLight(unsigned short a, const Vector2& b) { FC<void(unsigned short, const Vector2&)>(addr::World_MoveLight)(a, b); }
inline void World_PasteItem() { FC<void()>(addr::World_PasteItem)(); }
inline void World_RemoveLight(unsigned short a) { FC<void(unsigned short)>(addr::World_RemoveLight)(a); }
inline void World_SaveLevel() { FC<void()>(addr::World_SaveLevel)(); }
inline Object* World_ScanForClosestEdible(Object* a, const Vector2& b, float c, float d, bool e) { return FC<Object*(Object*, const Vector2&, float, float, bool)>(addr::World_ScanForClosestEdible)(a, b, c, d, e); }
inline void World_SelectAll() { FC<void()>(addr::World_SelectAll)(); }
inline void World_SetFPS(int a) { FC<void(int)>(addr::World_SetFPS)(a); }
inline void World_SetGFXViewOffset(const Vector2& a, const Vector2& b) { FC<void(const Vector2&, const Vector2&)>(addr::World_SetGFXViewOffset)(a, b); }
inline void World_SetGlobalAmbient(const Color& a) { FC<void(const Color&)>(addr::World_SetGlobalAmbient)(a); }
inline void World_SetLevelModulate(const Color& a) { FC<void(const Color&)>(addr::World_SetLevelModulate)(a); }
inline void World_SetLightAttenuation(unsigned short a, float b, float c, float d) { FC<void(unsigned short, float, float, float)>(addr::World_SetLightAttenuation)(a, b, c, d); }
inline void World_SetTextHint(const char* a, const Vector2& b) { FC<void(const char*, const Vector2&)>(addr::World_SetTextHint)(a, b); }
inline void World_SetWinFlag() { FC<void()>(addr::World_SetWinFlag)(); }
inline void World_ShowClickables() { FC<void()>(addr::World_ShowClickables)(); }
inline void World_ShowGoalBox(const char* a, const Vector2& b, const char* c, const char* d, const char* e) { FC<void(const char*, const Vector2&, const char*, const char*, const char*)>(addr::World_ShowGoalBox)(a, b, c, d, e); }
inline void World_ShowGoalBoxFX(const char* a, const Vector2& b) { FC<void(const char*, const Vector2&)>(addr::World_ShowGoalBoxFX)(a, b); }
inline void World_ShowPuzzleAsNew() { FC<void()>(addr::World_ShowPuzzleAsNew)(); }
inline void World_ShowSolutionTime(float a) { FC<void(float)>(addr::World_ShowSolutionTime)(a); }
inline void World_ShowTutorialDialog(const char* a) { FC<void(const char*)>(addr::World_ShowTutorialDialog)(a); }

// ===== object extensions =====
// each accessor returns null if the object lacks that extension; always null-check
inline PositionExtension* Object_GetPositionExtension(Object* o) { return FC<PositionExtension*(Object*)>(addr::Object_GetPositionExtension)(o); }
inline SuckableExtension* Object_GetSuckableExtension(Object* o) { return FC<SuckableExtension*(Object*)>(addr::Object_GetSuckableExtension)(o); }
inline WalkingExtension* Object_GetWalkingExtension(Object* o) { return FC<WalkingExtension*(Object*)>(addr::Object_GetWalkingExtension)(o); }
inline ThwackerExtension* Object_GetThwackerExtension(Object* o) { return FC<ThwackerExtension*(Object*)>(addr::Object_GetThwackerExtension)(o); }
inline EmotionPlatformExtension* Object_GetEmotionPlatformExtension(Object* o) { return FC<EmotionPlatformExtension*(Object*)>(addr::Object_GetEmotionPlatformExtension)(o); }
inline EdibleExtension* Object_GetEdibleExtension(Object* o) { return FC<EdibleExtension*(Object*)>(addr::Object_GetEdibleExtension)(o); }
inline EmotionExtension* Object_GetEmotionExtension(Object* o) { return FC<EmotionExtension*(Object*)>(addr::Object_GetEmotionExtension)(o); }
inline FlyingExtension* Object_GetFlyingExtension(Object* o) { return FC<FlyingExtension*(Object*)>(addr::Object_GetFlyingExtension)(o); }
inline LightingExtension* Object_GetLightingExtension(Object* o) { return FC<LightingExtension*(Object*)>(addr::Object_GetLightingExtension)(o); }
inline HoldingExtension* Object_GetHoldingExtension(Object* o) { return FC<HoldingExtension*(Object*)>(addr::Object_GetHoldingExtension)(o); }
inline RollingExtension* Object_GetRollingExtension(Object* o) { return FC<RollingExtension*(Object*)>(addr::Object_GetRollingExtension)(o); }
// physics has no templated getter; it sits at a fixed Object slot
inline PhysicsExtension* Object_GetPhysicsExtension(Object* o) {
#ifdef _WIN32
	return o ? *(PhysicsExtension**)((char*)o + 0x60) : nullptr;   // win32 Object->physics slot (x64 is +0x90)
#else
	return o ? *(PhysicsExtension**)((char*)o + 0x90) : nullptr;
#endif
}

inline void WalkingExtension_SetWalkSpeed(WalkingExtension* w, float s) { EC<void(WalkingExtension*, float)>(addr::WalkingExtension_SetWalkSpeed)(w, s); }
inline void WalkingExtension_SetActive(WalkingExtension* w, bool a) { EC<void(WalkingExtension*, bool)>(addr::WalkingExtension_SetActive)(w, a); }
inline unsigned long WalkingExtension_GetState(WalkingExtension* w) { return EC<unsigned long(WalkingExtension*)>(addr::WalkingExtension_GetState)(w); }
inline void WalkingExtension_StartWalking(WalkingExtension* w) { EC<void(WalkingExtension*)>(addr::WalkingExtension_StartWalking)(w); }
inline void WalkingExtension_StopWalking(WalkingExtension* w) { EC<void(WalkingExtension*)>(addr::WalkingExtension_StopWalking)(w); }
inline void WalkingExtension_KnockDown(WalkingExtension* w, const Vector2& dir) { EC<void(WalkingExtension*, const Vector2&)>(addr::WalkingExtension_KnockDown)(w, dir); }
inline void WalkingExtension_SetNoWalkFrame(WalkingExtension* w, int f) { EC<void(WalkingExtension*, int)>(addr::WalkingExtension_SetNoWalkFrame)(w, f); }
inline void WalkingExtension_ForceReset(WalkingExtension* w) { EC<void(WalkingExtension*)>(addr::WalkingExtension_ForceReset)(w); }
inline void WalkingExtension_Reset(WalkingExtension* w) { EC<void(WalkingExtension*)>(addr::WalkingExtension_Reset)(w); }

inline void ThwackerExtension_SetThwackSpeed(ThwackerExtension* t, float s) { EC<void(ThwackerExtension*, float)>(addr::ThwackerExtension_SetThwackSpeed)(t, s); }
inline bool ThwackerExtension_IsThwacking(ThwackerExtension* t) {
#ifdef _WIN32
	return t ? *(int*)((char*)t + 0x30) != 0 : false;   // standalone getter MSVC-inlined; thwack-state @+0x30 (0 = idle)
#else
	return EC<bool(ThwackerExtension*)>(addr::ThwackerExtension_IsThwacking)(t);
#endif
}
inline Vector2 ThwackerExtension_GetCentre(ThwackerExtension* t) {
	Vector2 r{0.0f, 0.0f};
#ifdef _WIN32
	// MSVC-inlined; centre = owner.pos + normalize(owner.facing)*reach. owner=*(Object**)(this+0x4), reach=*(float*)(this+0x3c)
	if (!t) return r;
	Object* owner = *(Object**)((char*)t + 0x4);
	if (!owner) return r;
	Vector2 pos = Object_GetPosition(owner), fac = Object_GetFacing(owner);
	float reach = *(float*)((char*)t + 0x3c);
	float len = __builtin_sqrtf(fac.x * fac.x + fac.y * fac.y);
	if (len > 0.0f) { r.x = pos.x + fac.x / len * reach; r.y = pos.y + fac.y / len * reach; }
	else r = pos;
#else
	r = FC<Vector2(ThwackerExtension*)>(addr::ThwackerExtension_GetCentre)(t);
#endif
	return r;
}

inline bool EdibleExtension_GetEaten(EdibleExtension* e) { return EC<bool(EdibleExtension*)>(addr::EdibleExtension_GetEaten)(e); }
inline unsigned EdibleExtension_GetEater(EdibleExtension* e) {
#ifdef _WIN32
	// MSVC-inlined; eaters = std::set<unsigned>@this+0x8. return lowest id via _Tree walk:
	// set{_Myhead@0,_Mysize@4}; begin=_Myhead->_Left; node{_Left@0,_Parent@4,_Right@8,_Color@0xc,_Isnil@0xd,_Myval@0x10}
	if (!e) return 0;
	char* set = (char*)e + 0x8;
	if (*(unsigned*)(set + 4) == 0) return 0;          // empty set -> no eater
	char* head = *(char**)set;
	if (!head) return 0;
	char* begin = *(char**)head;                       // _Myhead->_Left = leftmost
	return begin ? *(unsigned*)(begin + 0x10) : 0;
#else
	return EC<unsigned(EdibleExtension*)>(addr::EdibleExtension_GetEater)(e);
#endif
}
inline bool EdibleExtension_IsEatenBy(EdibleExtension* e, unsigned id) { return EC<bool(EdibleExtension*, unsigned)>(addr::EdibleExtension_IsEatenBy)(e, id); }

inline bool LightingExtension_IsLit(LightingExtension* l) { return EC<bool(LightingExtension*)>(addr::LightingExtension_IsLit)(l); }

inline bool SuckableExtension_WasRecentlySucked(SuckableExtension* s) { return EC<bool(SuckableExtension*)>(addr::SuckableExtension_WasRecentlySucked)(s); }
inline void SuckableExtension_SetSucked(SuckableExtension* s) { EC<void(SuckableExtension*)>(addr::SuckableExtension_SetSucked)(s); }

inline bool PositionExtension_IsForeground(PositionExtension* p) { return EC<bool(PositionExtension*)>(addr::PositionExtension_IsForeground)(p); }
inline bool PositionExtension_IsBackground(PositionExtension* p) { return EC<bool(PositionExtension*)>(addr::PositionExtension_IsBackground)(p); }
inline void PositionExtension_SetIsForeground(PositionExtension* p, bool v) { EC<void(PositionExtension*, bool)>(addr::PositionExtension_SetIsForeground)(p, v); }
inline void PositionExtension_SetIsBackground(PositionExtension* p, bool v) { EC<void(PositionExtension*, bool)>(addr::PositionExtension_SetIsBackground)(p, v); }

inline bool RollingExtension_IsRolling(RollingExtension* r) { return EC<bool(RollingExtension*)>(addr::RollingExtension_IsRolling)(r); }

inline bool HoldingExtension_IsHolding(HoldingExtension* h, Object* o) { return EC<bool(HoldingExtension*, Object*)>(addr::HoldingExtension_IsHolding)(h, o); }
inline bool HoldingExtension_IsHoldingAny(HoldingExtension* h) { return EC<bool(HoldingExtension*)>(addr::HoldingExtension_IsHoldingAny)(h); }
inline void HoldingExtension_HoldObject(HoldingExtension* h, Object* o) { EC<void(HoldingExtension*, Object*)>(addr::HoldingExtension_HoldObject)(h, o); }
inline void HoldingExtension_ReleaseAll(HoldingExtension* h) { EC<void(HoldingExtension*)>(addr::HoldingExtension_ReleaseAll)(h); }
inline void HoldingExtension_ReleaseObject(HoldingExtension* h, Object* o) {
#ifdef _WIN32
	// MSVC-inlined inverse of HoldObject: erase o from held vector<Object*> (begin@h+0x8, end@h+0xc),
	// then restore physics/collision/visibility/update. SetVisibility=0x5c400, SetUpdate=0xab180 __thiscall(Object*,bool)
	if (!h || !o) return;
	void** begin = *(void***)((char*)h + 0x8);
	void** end   = *(void***)((char*)h + 0xc);
	bool found = false;
	for (void** p = begin; p < end; ++p) {
		if (*p == o) { for (void** q = p; q + 1 < end; ++q) *q = *(q + 1); *(void***)((char*)h + 0xc) = end - 1; found = true; break; }
	}
	if (!found) return;
	Object_EnablePhysics(o, true);
	Object_EnableCollisions(o, true);
	EC<void(Object*, bool)>(addr::resolve(0x5c400))(o, true);   // SetVisibility(true)
	EC<void(Object*, bool)>(addr::resolve(0xab180))(o, true);   // SetUpdate(true)
#else
	EC<void(HoldingExtension*, Object*)>(addr::HoldingExtension_ReleaseObject)(h, o);
#endif
}
// GetHolds returns a std::vector<Object*> const& (pointer to {begin,end,cap}); walk it
template <class Fn>
inline void ForEachHeld(HoldingExtension* h, Fn fn) {
	if (!h) return;
	char* vec = (char*)EC<void*(HoldingExtension*)>(addr::HoldingExtension_GetHolds)(h);
	if (!vec) return;
	Object** begin = *(Object***)(vec + 0);
	Object** end   = *(Object***)(vec + sizeof(void*));   // std::vector end ptr: +8 (x64) / +4 (win32)
	for (Object** p = begin; p && p < end; ++p) fn(*p);
}

inline void FlyingExtension_SetState(FlyingExtension* f, unsigned long s) { EC<void(FlyingExtension*, unsigned long)>(addr::FlyingExtension_SetState)(f, s); }
inline unsigned long FlyingExtension_GetState(FlyingExtension* f) { return EC<unsigned long(FlyingExtension*)>(addr::FlyingExtension_GetState)(f); }

inline bool EmotionExtension_RecentlyChanged(EmotionExtension* e) { return EC<bool(EmotionExtension*)>(addr::EmotionExtension_RecentlyChanged)(e); }
inline const char* EmotionExtension_GetEmotionName(EmotionExtension* e) { return EC<const char*(EmotionExtension*)>(addr::EmotionExtension_GetEmotionName)(e); }
inline void EmotionExtension_SetEmotionName(EmotionExtension* e, const char* n) {
#ifdef _WIN32
	// MSVC-inlined; name is std::string@this+0x20. overwrite in place with MSVC-layout string (SSO)
	if (e) new ((char*)e + 0x20) EString(n ? n : "");
#else
	EC<void(EmotionExtension*, const char*)>(addr::EmotionExtension_SetEmotionName)(e, n);
#endif
}

inline unsigned EmotionPlatformExtension_GetCurrentEmotion(EmotionPlatformExtension* e) { return EC<unsigned(EmotionPlatformExtension*)>(addr::EmotionPlatformExtension_GetCurrentEmotion)(e); }
inline bool EmotionPlatformExtension_MatchesCurrentEmotion(EmotionPlatformExtension* e) { return EC<bool(EmotionPlatformExtension*)>(addr::EmotionPlatformExtension_MatchesCurrentEmotion)(e); }
inline void EmotionPlatformExtension_SetEmotion(EmotionPlatformExtension* e, unsigned id) { EC<void(EmotionPlatformExtension*, unsigned)>(addr::EmotionPlatformExtension_SetEmotion)(e, id); }

// iterate accumulated collision contacts. reading enables accumulation (engine refills deque each step)
template <class Fn>
inline void ForEachCollision(Object* o, Fn fn) {
	PhysicsExtension* phys = Object_GetPhysicsExtension(o);
	if (!phys) return;
	char* dq = (char*)EC<void*(PhysicsExtension*)>(addr::PhysicsExtension_GetAccumulate)(phys);
	if (!dq) return;
#ifdef _WIN32
	// MSVC std::deque<CollisionReport>: _Map@0 (T**), _Mapsize@4, _Myoff@8, _Mysize@0xc
	// CollisionReport=0x1c bytes on Win32; _DEQUESIZ=1 so each map slot holds exactly one report
	char**   map     = *(char***)(dq + 0x0);
	unsigned mapsize = *(unsigned*)(dq + 0x4);
	unsigned myoff   = *(unsigned*)(dq + 0x8);
	unsigned mysize  = *(unsigned*)(dq + 0xc);
	if (!map || !mapsize) return;
	for (unsigned i = 0; i < mysize; ++i) {
		char* blk = map[(myoff + i) % mapsize];
		if (blk) fn(*(const CollisionReport*)blk);
	}
#else
	// libstdc++ std::deque<CollisionReport>; element 0x28 bytes, chunk holds 512/0x28 = 12 per node
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
