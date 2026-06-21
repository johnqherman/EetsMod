// eets_addr.h - complete address table for the Eets engine's Lua-binding
// functions (all 76) plus curated class methods / singletons / UI helpers.
// Generated for BuildID e81cc5504d3ef03324805df3e9fc508c1bf8c628 by
// gen_engine_header.sh. The binary is non-PIE so these are runtime-stable.
//
// Itanium mangling does not encode return types, so the arg signature is shown
// in a comment; cast addr::X to the correct full signature to call it. Common
// ones have typed wrappers in eets_engine.h.
#pragma once
#include <cstdint>
namespace Eets { namespace addr {

// ===== Lua-binding statics (all 76) =====
constexpr uintptr_t Anim_GetCurrentFrameIndex          = 0x506630;  // (Anim::Animation*)
constexpr uintptr_t Anim_SetCurrentFrameIndex          = 0x506640;  // (Anim::Animation*, unsigned int)
constexpr uintptr_t Creator_Undo                       = 0x5bce20;  // ()
constexpr uintptr_t Misc_BindKey                       = 0x5bce70;  // (char const*, char const*)
constexpr uintptr_t Misc_PauseProfile                  = 0x5bceb0;  // ()
constexpr uintptr_t Misc_Print                         = 0x5bce60;  // (char const*)
constexpr uintptr_t Misc_Profile                       = 0x5bcea0;  // ()
constexpr uintptr_t Misc_SaveScreenshot                = 0x5bcec0;  // ()
constexpr uintptr_t Object_ApplyImpulse                = 0x5c9d40;  // (Object*, Vector2 const&)
constexpr uintptr_t Object_CallFunction                = 0x5ca030;  // (Object*, char const*)
constexpr uintptr_t Object_CreateEffect                = 0x5c9cd0;  // (Object*, char const*)
constexpr uintptr_t Object_EnableCollisions            = 0x5c9d30;  // (Object*, bool)
constexpr uintptr_t Object_EnablePhysics               = 0x5c9d10;  // (Object*, bool)
constexpr uintptr_t Object_GetBlueprintHash            = 0x5c9cb0;  // (Object*)
constexpr uintptr_t Object_GetBlueprintName            = 0x5c9cc0;  // (Object*)
constexpr uintptr_t Object_GetCollisionReports         = 0x5c9e90;  // (Object*)
constexpr uintptr_t Object_HoldObject                  = 0x5c9f10;  // (Object*, Object*)
constexpr uintptr_t Object_IsHolding                   = 0x5c9f40;  // (Object*, Object*)
constexpr uintptr_t Object_IsPhysicsEnabled            = 0x5ca020;  // (Object*)
constexpr uintptr_t Object_IsRolling                   = 0x5c9fa0;  // (Object*)
constexpr uintptr_t Object_IsWalker                    = 0x5c9d70;  // (Object*)
constexpr uintptr_t Object_ReleaseAll                  = 0x5c9f70;  // (Object*)
constexpr uintptr_t Object_SetAltCollision             = 0x5c9e30;  // (Object*, char const*)
constexpr uintptr_t Sound_CreateSound                  = 0x5bcdd0;  // (char const*, bool, float, Vector2 const&)
constexpr uintptr_t Sound_PlayMusic                    = 0x5bcd80;  // (char const*, bool)
constexpr uintptr_t Sound_SetMusicVolume               = 0x5bcdb0;  // (long)
constexpr uintptr_t World_AddBackgroundImage           = 0x5bc000;  // (char const*, Vector2 const&, Vector2 const&, float, Colour const&, float)
constexpr uintptr_t World_AddForegroundImage           = 0x5bbfb0;  // (char const*, Vector2 const&, Vector2 const&, float, Colour const&, float)
constexpr uintptr_t World_Alert                        = 0x5bc990;  // (Vector2 const&, float)
constexpr uintptr_t World_AnimationsEqual              = 0x5bbb30;  // (Anim::Animation const*, Anim::Animation const*)
constexpr uintptr_t World_ChangeEmotion                = 0x5bcb90;  // (unsigned long, unsigned int)
constexpr uintptr_t World_CheckGoal                    = 0x5bcc00;  // (Object*)
constexpr uintptr_t World_CopyItem                     = 0x5bbbc0;  // ()
constexpr uintptr_t World_CreateEffect                 = 0x5bc090;  // (char const*, Vector2)
constexpr uintptr_t World_CreateExplosion              = 0x5bc0d0;  // (Vector2 const&, float)
constexpr uintptr_t World_CreateExplosionSpecial       = 0x5bc170;  // ()
constexpr uintptr_t World_CreateLight                  = 0x5bc470;  // (Vector2 const&, Colour const&, float, float)
constexpr uintptr_t World_CreateObject                 = 0x5bc660;  // (char const*, Vector2)
constexpr uintptr_t World_GetAddedObjects              = 0x5bc980;  // ()
constexpr uintptr_t World_GetDeterministicDouble       = 0x5bbfa0;  // ()
constexpr uintptr_t World_GetDeterministicIntRange     = 0x5bbf90;  // (int, int)
constexpr uintptr_t World_GetEets                      = 0x5bca40;  // ()
constexpr uintptr_t World_GetGravity                   = 0x5bc710;  // ()
constexpr uintptr_t World_GetObjectByID                = 0x5bc6b0;  // (unsigned long)
constexpr uintptr_t World_GetObjects                   = 0x5bc960;  // ()
constexpr uintptr_t World_IncrementStat                = 0x5bbb00;  // (char const*)
constexpr uintptr_t World_IsInLevelEditor              = 0x5bca70;  // ()
constexpr uintptr_t World_IsInMainMenu                 = 0x5bc870;  // ()
constexpr uintptr_t World_IsPaused                     = 0x5bc950;  // ()
constexpr uintptr_t World_IsSimulating                 = 0x5bc860;  // ()
constexpr uintptr_t World_LightExists                  = 0x5bc5b0;  // (unsigned short)
constexpr uintptr_t World_MerchMerchPickUp             = 0x5bbb40;  // ()
constexpr uintptr_t World_MoveLight                    = 0x5bc550;  // (unsigned short, Vector2 const&)
constexpr uintptr_t World_PasteItem                    = 0x5bbbf0;  // ()
constexpr uintptr_t World_Pause                        = 0x5bc920;  // ()
constexpr uintptr_t World_RemoveLight                  = 0x5bc5e0;  // (unsigned short)
constexpr uintptr_t World_SaveLevel                    = 0x5bc7b0;  // ()
constexpr uintptr_t World_ScanForClosestEdible         = 0x5bbc30;  // (Object const*, Vector2 const&, float, float, bool)
constexpr uintptr_t World_Scare                        = 0x5bc9e0;  // (Vector2 const&, float, int)
constexpr uintptr_t World_SelectAll                    = 0x5bcd50;  // ()
constexpr uintptr_t World_SetFPS                       = 0x5bc690;  // (int)
constexpr uintptr_t World_SetGameSpeed                 = 0x5bc070;  // (unsigned long)
constexpr uintptr_t World_SetGFXViewOffset             = 0x5bbad0;  // (Vector2 const&, Vector2 const&)
constexpr uintptr_t World_SetGlobalAmbient             = 0x5bc410;  // (Colour const&)
constexpr uintptr_t World_SetGravity                   = 0x5bc6d0;  // (Vector2, int)
constexpr uintptr_t World_SetLevelModulate             = 0x5bc050;  // (Colour const&)
constexpr uintptr_t World_SetLightAttenuation          = 0x5bc5f0;  // (unsigned short, float, float, float)
constexpr uintptr_t World_SetMaximumSpeed              = 0x5bbc20;  // (float)
constexpr uintptr_t World_SetTextHint                  = 0x5bccd0;  // (char const*, Vector2 const&)
constexpr uintptr_t World_SetWinFlag                   = 0x5bc880;  // ()
constexpr uintptr_t World_ShowClickables               = 0x5bcd30;  // ()
constexpr uintptr_t World_ShowGoalBox                  = 0x5bcac0;  // (char const*, Vector2 const&, char const*, char const*, char const*)
constexpr uintptr_t World_ShowGoalBoxFX                = 0x5bcb40;  // (char const*, Vector2 const&)
constexpr uintptr_t World_ShowPuzzleAsNew              = 0x5bc8b0;  // ()
constexpr uintptr_t World_ShowSolutionTime             = 0x5bc8e0;  // (float)
constexpr uintptr_t World_ShowTutorialDialog           = 0x5bcbc0;  // (char const*)

// ===== class methods (__thiscall: first arg is the object) =====
constexpr uintptr_t Object_GetPosition                 = 0x5739c0;  // (Object*) -> Vector2
constexpr uintptr_t Object_GetVelocity                 = 0x573a00;  // (Object*) -> Vector2
constexpr uintptr_t Object_GetID                       = 0x574250;  // (Object*) -> unsigned long
constexpr uintptr_t Object_GetMotionModel              = 0x5ca360;  // (Object*) -> MotionModel*
constexpr uintptr_t Object_SetPosition                 = 0x5739a0;  // (Object*, Vector2 const&)
constexpr uintptr_t Object_ForcePosition               = 0x5739d0;  // (Object*, Vector2 const&)
constexpr uintptr_t Object_SetFacing                   = 0x573a10;  // (Object*, Vector2 const&)
constexpr uintptr_t Object_GetFacing                   = 0x574280;  // (Object*) -> Vector2
constexpr uintptr_t Object_SetFlipped                  = 0x5ca380;  // (Object*, bool)
constexpr uintptr_t Object_GetFlipped                  = 0x5ca370;  // (Object*) -> bool
constexpr uintptr_t Object_KillMe                      = 0x5742c0;  // (Object*)

// ===== event hook targets (detoured by the loader to fire EetsMod_OnEvent) =====
constexpr uintptr_t hook_Simulator_LoadWinCondition    = 0x62b770;  // -> "level_load"
constexpr uintptr_t hook_Simulator_ResetSimulation     = 0x62e140;  // -> "level_reset"
constexpr uintptr_t hook_LevelManager_CompleteLevel    = 0x5a7f00;  // -> "level_complete"
constexpr uintptr_t hook_Creator_StartEetsDeadDialog   = 0x61cf30;  // -> "eets_death"
constexpr uintptr_t hook_Object_KillMe                 = 0x5742c0;  // -> "object_killed"
constexpr uintptr_t hook_ObjectMgr_CreateObject        = 0x576290;  // -> "object_spawn"
constexpr uintptr_t MotionModel_PushMotion             = 0x50d830;  // (MotionModel*, char const*, bool, bool)
constexpr uintptr_t MotionModel_PopMotion              = 0x50dc60;  // (MotionModel*)
constexpr uintptr_t MotionModel_GetCurrentMotionName   = 0x50dc40;  // (MotionModel*) -> char const*

// ===== singletons / engine / UI =====
constexpr uintptr_t ObjectMgr_i                        = 0x576280;  // () -> ObjectMgr*
constexpr uintptr_t Simulator_i                        = 0x62ae90;  // () -> Simulator*
constexpr uintptr_t GraphicsEngine_i                   = 0x549100;  // () -> GraphicsEngine*  (w@+0x40 h@+0x44)
constexpr uintptr_t printText                          = 0x58a020;  // (int x,int y,char const*,Colour const&)
constexpr uintptr_t TextPrinter_DrawString             = 0x541380;  // (string,size,style,Colour,Vector2,bool,Vector2 const&)
constexpr uintptr_t GraphicsEngine_DrawLine            = 0x54a2f0;  // (GE*,Vector2 const&,Vector2 const&,Colour const&,float) 1px line (width ignored)
constexpr uintptr_t GraphicsEngine_DrawSquare          = 0x54a050;  // (GE*,Vector2 const& topLeft,Vector2 const& botRight,Colour const&) FILLED rect
constexpr uintptr_t GraphicsEngine_DrawCircleFilled    = 0x54a030;  // (GE*,Vector2 const&,float r,Colour const&,int segs)

// ===== localization (StringPool) =====
constexpr uintptr_t StringPool_instance                = 0x1210838; // StringPool* (read the pointer)
constexpr uintptr_t StringPool_Resolve                 = 0x58b0d0;  // (StringPool*, char const* id) -> char const*
constexpr uintptr_t StringPool_LoadFile                = 0x58ac50;  // (char const* path) load extra strings

// ===== assets (ADVANCED/experimental: ABI not wrapper-validated) =====
// Texture::Load is __thiscall into a caller-allocated Texture; you must allocate
// the object, Load(path), UploadTexture(), then IGraphicsEngine::DrawTexture.
constexpr uintptr_t Texture_Load                       = 0x542b60;  // (Texture* this, char const* path)
constexpr uintptr_t Texture_UploadTexture              = 0x552b00;  // (Texture* this, int)
constexpr uintptr_t IGraphicsEngine_i                  = 0x53e440;  // () -> IGraphicsEngine*
constexpr uintptr_t IGraphicsEngine_DrawTexture        = 0x53e450;  // (IGE*, Texture const*, Vector2 const&)
constexpr uintptr_t AnimExt_LoadAnimation              = 0x553940;  // (char const* path) -> anim
constexpr uintptr_t TextureManager_instance            = 0x120b708; // TextureManager* (cache = unordered_map<string,KLEITEXTURE*> at +0)
constexpr uintptr_t TextureManager_LoadTexture         = 0x5512c0;  // (TM*, string const&, ImageFormat, bool) load+cache
constexpr uintptr_t SpriteManager_i                    = 0x545c50;  // () -> SpriteManager*
constexpr uintptr_t SpriteManager_Load                 = 0x546940;  // (sret{Sprite*,ctrl}, SM*, string const&, ImageFormat)
constexpr uintptr_t GraphicsEngine_DrawSprite          = 0x54be10;  // (GE*, Sprite*, Vector2 pos, Vector2 uv0, Vector2 uv1, Colour)
constexpr uintptr_t Sprite_GetWidth                    = 0x545510;  // (Sprite*) -> unsigned
constexpr uintptr_t Sprite_GetHeight                   = 0x545520;  // (Sprite*) -> unsigned

}} // namespace Eets::addr
