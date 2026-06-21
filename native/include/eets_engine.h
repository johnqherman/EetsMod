// abi note: Vector2 { float x, y } is one SSE eightbyte in xmm0 - matching the type matches the abi.
#pragma once
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>
#include "eets_addr.h"

namespace Eets {

// valid for this exact build; loader warns if the running BuildID differs
constexpr const char* EXPECTED_BUILDID = "e81cc5504d3ef03324805df3e9fc508c1bf8c628";

struct Vector2 { float x, y; };
struct Colour { unsigned char r, g, b, a;          // 4-byte packed (engine abi)
	Colour(unsigned char R=255, unsigned char G=255, unsigned char B=255, unsigned char A=255)
		: r(R), g(G), b(B), a(A) {}
};
struct Object;        // opaque engine type
struct MotionModel;   // opaque
struct ObjectMgr;     // opaque

// FontPrintSizes enum -> pixel height: 1=13 2=14 3=20 4=28 5=35
enum FontSize { FONT_TINY = 1, FONT_SMALL = 2, FONT_NORMAL = 3, FONT_BIG = 4, FONT_HUGE = 5 };
// FontPrintStyles -> typeface. STYLE_GEEK (geekabyte) is the in-game gui font.
enum FontStyle { STYLE_KOMIKA = 1, STYLE_GEEK = 2, STYLE_NORMAL = 3, STYLE_BRADY = 4 };

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
	// sret: returns SoundHandle by value
	char handle[16] = {0};
	((void(*)(void*, const char*, bool, float, const Vector2&))addr::Sound_CreateSound)(handle, name, loop, vol, pos);
}
inline void PlaySound(const char* name, float vol = 0.0f) {
	Vector2 z{0.0f, 0.0f};
	Sound_CreateSound(name, false, vol, z);
}

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

inline void MotionModel_PushMotion(MotionModel* m, const char* name, bool a, bool b) {
	((void(*)(MotionModel*, const char*, bool, bool))addr::MotionModel_PushMotion)(m, name, a, b);
}
inline void MotionModel_PopMotion(MotionModel* m) {
	((void(*)(MotionModel*))addr::MotionModel_PopMotion)(m);
}
inline const char* MotionModel_GetCurrentMotionName(MotionModel* m) {
	return ((const char*(*)(MotionModel*))addr::MotionModel_GetCurrentMotionName)(m);
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
	Object** end   = *(Object***)(mgr + 8);
	for (Object** p = begin; p && p < end; ++p) fn(*p);
}

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
inline void DrawText(int x, int y, const char* text, Colour c = Colour()) {
	((void(*)(int, int, const char*, const Colour&))addr::printText)(x, y, text, c);
}
// warning: dir is the baseline DIRECTION not a scale - keep horizontal {1,0} or text rotates; dirx<1 shrinks horizontally.
inline void DrawTextSized(int x, int y, const char* text, int size,
                          Colour c = Colour(), int style = STYLE_NORMAL, float dirx = 1.0f) {
	std::string s = text ? text : "";
	Vector2 pos{(float)x, (float)y}, dir{dirx, 0.0f};
	((void(*)(const std::string&, int, int, Colour, Vector2, bool, const Vector2&))
	 addr::TextPrinter_DrawString)(s, size, style, c, pos, false, dir);
}
inline void DrawTextOutlined(int x, int y, const char* text, int size,
                             Colour c = Colour(), Colour shadow = Colour(0, 0, 0, 200),
                             int style = STYLE_NORMAL) {
	DrawTextSized(x + 2, y + 2, text, size, shadow, style);
	DrawTextSized(x, y, text, size, c, style);
}

inline void* GraphicsEngine_i() { return ((void*(*)())addr::GraphicsEngine_i)(); }
// GraphicsEngine geometry funcs swap R<->B internally; pre-swap so our RGBA shows correctly.
inline Colour swab(Colour c) { return Colour(c.b, c.g, c.r, c.a); }
inline void DrawLine(Vector2 a, Vector2 b, Colour c, float width = 1.0f) {
	void* g = GraphicsEngine_i(); if (!g) return;
	Colour s = swab(c);
	((void(*)(void*, const Vector2&, const Vector2&, const Colour&, float))
	 addr::GraphicsEngine_DrawLine)(g, a, b, s, width);
}
inline void FillRect(int x, int y, int w, int h, Colour c) {
	void* g = GraphicsEngine_i(); if (!g) return;
	Colour s = swab(c);
	Vector2 a{(float)x, (float)y}, b{(float)(x + w), (float)(y + h)};
	((void(*)(void*, const Vector2&, const Vector2&, const Colour&))
	 addr::GraphicsEngine_DrawSquare)(g, a, b, s);
}
inline void FillCircle(int x, int y, float r, Colour c, int segs = 24) {
	void* g = GraphicsEngine_i(); if (!g) return;
	Colour s = swab(c);
	Vector2 p{(float)x, (float)y};
	((void(*)(void*, const Vector2&, float, const Colour&, int))
	 addr::GraphicsEngine_DrawCircleFilled)(g, p, r, s, segs);
}
// built from filled bars - DrawLine ignores width
inline void DrawRect(int x, int y, int w, int h, Colour c, float t = 2.0f) {
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
inline int  SpriteWidth(void* s)  { return s ? (int)((unsigned(*)(void*))addr::Sprite_GetWidth)(s)  : 0; }
inline int  SpriteHeight(void* s) { return s ? (int)((unsigned(*)(void*))addr::Sprite_GetHeight)(s) : 0; }

inline void DrawSpriteAt(void* sprite, int x, int y, Colour tint = Colour()) {
	void* ge = GraphicsEngine_i();
	if (!ge || !sprite) return;
	Vector2 uv0{0.0f, 0.0f}, uv1{1.0f, 1.0f};
	((void(*)(void*, Vector2&, Vector2&))addr::Sprite_GetDiffuseUV)(sprite, uv0, uv1);
	Vector2 pos{(float)x, (float)y};
	((void(*)(void*, void*, const Vector2&, const Vector2&, const Vector2&, const Colour&))
	 addr::GraphicsEngine_DrawSprite)(ge, sprite, pos, uv0, uv1, tint);
}

inline bool DrawImage(const char* path, int x, int y, Colour tint = Colour()) {
	void* sprite = LoadSprite(path);
	if (!sprite) return false;
	DrawSpriteAt(sprite, x, y, tint);
	return true;
}
inline bool DrawImageHUD(const char* path, int x, int y, Colour tint = Colour()) {
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

inline bool DrawAnim(const char* path, int x, int y, float dt, float fps = 0.0f, Colour tint = Colour()) {
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
	return ((const char*(*)(void*, const char*))addr::StringPool_Resolve)(sp, id);
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
inline void World_AddBackgroundImage(const char* a, const Vector2& b, const Vector2& c, float d, const Colour& e, float f) { ((void(*)(const char*, const Vector2&, const Vector2&, float, const Colour&, float))addr::World_AddBackgroundImage)(a, b, c, d, e, f); }
inline void World_AddForegroundImage(const char* a, const Vector2& b, const Vector2& c, float d, const Colour& e, float f) { ((void(*)(const char*, const Vector2&, const Vector2&, float, const Colour&, float))addr::World_AddForegroundImage)(a, b, c, d, e, f); }
inline void World_Alert(const Vector2& a, float b) { ((void(*)(const Vector2&, float))addr::World_Alert)(a, b); }
inline bool World_AnimationsEqual(void* a, void* b) { return ((bool(*)(void*, void*))addr::World_AnimationsEqual)(a, b); }
inline void World_CheckGoal(Object* a) { ((void(*)(Object*))addr::World_CheckGoal)(a); }
inline void World_CopyItem() { ((void(*)())addr::World_CopyItem)(); }
inline void World_CreateLight(const Vector2& a, const Colour& b, float c, float d) { ((void(*)(const Vector2&, const Colour&, float, float))addr::World_CreateLight)(a, b, c, d); }
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
inline void World_SetGlobalAmbient(const Colour& a) { ((void(*)(const Colour&))addr::World_SetGlobalAmbient)(a); }
inline void World_SetLevelModulate(const Colour& a) { ((void(*)(const Colour&))addr::World_SetLevelModulate)(a); }
inline void World_SetLightAttenuation(unsigned short a, float b, float c, float d) { ((void(*)(unsigned short, float, float, float))addr::World_SetLightAttenuation)(a, b, c, d); }
inline void World_SetTextHint(const char* a, const Vector2& b) { ((void(*)(const char*, const Vector2&))addr::World_SetTextHint)(a, b); }
inline void World_SetWinFlag() { ((void(*)())addr::World_SetWinFlag)(); }
inline void World_ShowClickables() { ((void(*)())addr::World_ShowClickables)(); }
inline void World_ShowGoalBox(const char* a, const Vector2& b, const char* c, const char* d, const char* e) { ((void(*)(const char*, const Vector2&, const char*, const char*, const char*))addr::World_ShowGoalBox)(a, b, c, d, e); }
inline void World_ShowGoalBoxFX(const char* a, const Vector2& b) { ((void(*)(const char*, const Vector2&))addr::World_ShowGoalBoxFX)(a, b); }
inline void World_ShowPuzzleAsNew() { ((void(*)())addr::World_ShowPuzzleAsNew)(); }
inline void World_ShowSolutionTime(float a) { ((void(*)(float))addr::World_ShowSolutionTime)(a); }
inline void World_ShowTutorialDialog(const char* a) { ((void(*)(const char*))addr::World_ShowTutorialDialog)(a); }

// pure math on our Vector2 (no engine call needed)
inline float VecDot(const Vector2& a, const Vector2& b) { return a.x*b.x + a.y*b.y; }
inline float VecLenSqr(const Vector2& v) { return v.x*v.x + v.y*v.y; }
inline float VecLen(const Vector2& v) { float d = VecLenSqr(v); return d > 0 ? __builtin_sqrtf(d) : 0; }
inline Vector2 VecNormalize(const Vector2& v) { float l = VecLen(v); return l > 0 ? Vector2{v.x/l, v.y/l} : Vector2{0,0}; }

} // namespace Eets
