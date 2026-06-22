#pragma once
// Windows (PE32 i386) addresses for Eets.exe, as RVAs resolved against the runtime PE
// base. Recovered from the luabind registrars (see ~/eets-win-ref/recovered-rvas.txt).
// Mirrors include/eets_addr.h so eets_engine.h consumes addr::NAME identically.
#include <cstdint>
#include <windows.h>
namespace Eets { namespace addr {
inline uintptr_t resolve(uintptr_t rva) { return rva ? (uintptr_t)GetModuleHandleA(nullptr) + rva : 0; }

// ===== Lua-binding statics (all 76) =====
inline uintptr_t Anim_GetCurrentFrameIndex          = resolve(0x0);  // (Anim::Animation*)  // TODO
inline uintptr_t Anim_SetCurrentFrameIndex          = resolve(0x0);  // (Anim::Animation*, unsigned int)  // TODO
inline uintptr_t Creator_Undo                       = resolve(0x-3263d0);  // ()
inline uintptr_t Misc_BindKey                       = resolve(0x0);  // (char const*, char const*)  // TODO
inline uintptr_t Misc_PauseProfile                  = resolve(0x-326280);  // ()
inline uintptr_t Misc_Print                         = resolve(0x-326270);  // (char const*)
inline uintptr_t Misc_Profile                       = resolve(0x-326260);  // ()
inline uintptr_t Misc_SaveScreenshot                = resolve(0x-326250);  // ()
inline uintptr_t Object_ApplyImpulse                = resolve(0x-312c90);  // (Object*, Vector2 const&)
inline uintptr_t Object_CallFunction                = resolve(0x-355f40);  // (Object*, char const*)
inline uintptr_t Object_CreateEffect                = resolve(0x-354e90);  // (Object*, char const*)
inline uintptr_t Object_EnableCollisions            = resolve(0x-354eb0);  // (Object*, bool)
inline uintptr_t Object_EnablePhysics               = resolve(0x-312e30);  // (Object*, bool)
inline uintptr_t Object_GetBlueprintHash            = resolve(0x-312de0);  // (Object*)
inline uintptr_t Object_GetBlueprintName            = resolve(0x-312e60);  // (Object*)
inline uintptr_t Object_GetCollisionReports         = resolve(0x-312d70);  // (Object*)
inline uintptr_t Object_HoldObject                  = resolve(0x-312d30);  // (Object*, Object*)
inline uintptr_t Object_IsHolding                   = resolve(0x-312c70);  // (Object*, Object*)
inline uintptr_t Object_IsPhysicsEnabled            = resolve(0x-312e70);  // (Object*)
inline uintptr_t Object_IsRolling                   = resolve(0x-3a3c10);  // (Object*) [VERIFY: dup]
inline uintptr_t Object_IsWalker                    = resolve(0x-319fb0);  // (Object*)
inline uintptr_t Object_ReleaseAll                  = resolve(0x-312ce0);  // (Object*)
inline uintptr_t Object_SetAltCollision             = resolve(0x-312dc0);  // (Object*, char const*)
inline uintptr_t Sound_CreateSound                  = resolve(0x-324870);  // (char const*, bool, float, Vector2 const&)
inline uintptr_t Sound_PlayMusic                    = resolve(0x-324840);  // (char const*, bool)
inline uintptr_t Sound_SetMusicVolume               = resolve(0x-324820);  // (long)
inline uintptr_t World_AddBackgroundImage           = resolve(0x-324800);  // (char const*, Vector2 const&, Vector2 const&, float, Color const&, float)
inline uintptr_t World_AddForegroundImage           = resolve(0x-3247c0);  // (char const*, Vector2 const&, Vector2 const&, float, Color const&, float)
inline uintptr_t World_Alert                        = resolve(0x-324780);  // (Vector2 const&, float)
inline uintptr_t World_AnimationsEqual              = resolve(0x-324720);  // (Anim::Animation const*, Anim::Animation const*)
inline uintptr_t World_ChangeEmotion                = resolve(0x-324710);  // (unsigned long, unsigned int) [VERIFY: dup]
inline uintptr_t World_CheckGoal                    = resolve(0x-3246f0);  // (Object*) [VERIFY: dup]
inline uintptr_t World_CopyItem                     = resolve(0x-324630);  // ()
inline uintptr_t World_CreateEffect                 = resolve(0x-324610);  // (char const*, Vector2)
inline uintptr_t World_CreateExplosion              = resolve(0x-3245f0);  // (Vector2 const&, float)
inline uintptr_t World_CreateExplosionSpecial       = resolve(0x-324570);  // ()
inline uintptr_t World_CreateLight                  = resolve(0x-3242e0);  // (Vector2 const&, Color const&, float, float)
inline uintptr_t World_CreateObject                 = resolve(0x-3241a0);  // (char const*, Vector2)
inline uintptr_t World_GetAddedObjects              = resolve(0x-324180);  // ()
inline uintptr_t World_GetDeterministicDouble       = resolve(0x-324170);  // ()
inline uintptr_t World_GetDeterministicIntRange     = resolve(0x-324160);  // (int, int)
inline uintptr_t World_GetEets                      = resolve(0x-324140);  // ()
inline uintptr_t World_GetGravity                   = resolve(0x-324110);  // ()
inline uintptr_t World_GetObjectByID                = resolve(0x-324040);  // (unsigned long)
inline uintptr_t World_GetObjects                   = resolve(0x-324020);  // ()
inline uintptr_t World_IncrementStat                = resolve(0x-324000);  // (char const*)
inline uintptr_t World_IsInLevelEditor              = resolve(0x-323fd0);  // ()
inline uintptr_t World_IsInMainMenu                 = resolve(0x-323f90);  // ()
inline uintptr_t World_IsPaused                     = resolve(0x-323f80);  // ()
inline uintptr_t World_IsSimulating                 = resolve(0x-323f70);  // ()
inline uintptr_t World_LightExists                  = resolve(0x-323f60);  // (unsigned short)
inline uintptr_t World_MerchMerchPickUp             = resolve(0x-323f30);  // ()
inline uintptr_t World_MoveLight                    = resolve(0x-323ec0);  // (unsigned short, Vector2 const&)
inline uintptr_t World_PasteItem                    = resolve(0x-323e70);  // ()
inline uintptr_t World_Pause                        = resolve(0x-323e50);  // ()
inline uintptr_t World_RemoveLight                  = resolve(0x-323e30);  // (unsigned short)
inline uintptr_t World_SaveLevel                    = resolve(0x-323e20);  // ()
inline uintptr_t World_ScanForClosestEdible         = resolve(0x-323da0);  // (Object const*, Vector2 const&, float, float, bool)
inline uintptr_t World_Scare                        = resolve(0x-323af0);  // (Vector2 const&, float, int)
inline uintptr_t World_SelectAll                    = resolve(0x-323a80);  // ()
inline uintptr_t World_SetFPS                       = resolve(0x-323a50);  // (int)
inline uintptr_t World_SetGameSpeed                 = resolve(0x-323a10);  // (unsigned long)
inline uintptr_t World_SetGFXViewOffset             = resolve(0x-323a30);  // (Vector2 const&, Vector2 const&)
inline uintptr_t World_SetGlobalAmbient             = resolve(0x-3239f0);  // (Color const&)
inline uintptr_t World_SetGravity                   = resolve(0x-323930);  // (Vector2, int)
inline uintptr_t World_SetLevelModulate             = resolve(0x-323900);  // (Color const&)
inline uintptr_t World_SetLightAttenuation          = resolve(0x-3238e0);  // (unsigned short, float, float, float)
inline uintptr_t World_SetMaximumSpeed              = resolve(0x-323890);  // (float)
inline uintptr_t World_SetTextHint                  = resolve(0x-323870);  // (char const*, Vector2 const&)
inline uintptr_t World_SetWinFlag                   = resolve(0x-323830);  // ()
inline uintptr_t World_ShowClickables               = resolve(0x-323800);  // ()
inline uintptr_t World_ShowGoalBox                  = resolve(0x-3237e0);  // (char const*, Vector2 const&, char const*, char const*, char const*)
inline uintptr_t World_ShowGoalBoxFX                = resolve(0x-323790);  // (char const*, Vector2 const&)
inline uintptr_t World_ShowPuzzleAsNew              = resolve(0x-323760);  // ()
inline uintptr_t World_ShowSolutionTime             = resolve(0x-323730);  // (float)
inline uintptr_t World_ShowTutorialDialog           = resolve(0x-323700);  // (char const*)

// ===== class methods (__thiscall: first arg is the object) =====
inline uintptr_t Object_GetPosition                 = resolve(0x-355e70);  // (Object*) -> Vector2
inline uintptr_t Object_GetVelocity                 = resolve(0x-355f10);  // (Object*) -> Vector2
inline uintptr_t Object_GetID                       = resolve(0x-312e90);  // (Object*) -> unsigned long
inline uintptr_t Object_GetMotionModel              = resolve(0x-355fb0);  // (Object*) -> MotionModel*
inline uintptr_t Object_SetPosition                 = resolve(0x-311e50);  // (Object*, Vector2 const&)
inline uintptr_t Object_ForcePosition               = resolve(0x-3a3c10);  // (Object*, Vector2 const&) [VERIFY: dup]
inline uintptr_t Object_SetFacing                   = resolve(0x-356000);  // (Object*, Vector2 const&)
inline uintptr_t Object_GetFacing                   = resolve(0x-312fc0);  // (Object*) -> Vector2
inline uintptr_t Object_SetFlipped                  = resolve(0x-3a3e40);  // (Object*, bool)
inline uintptr_t Object_GetFlipped                  = resolve(0x-312df0);  // (Object*) -> bool
inline uintptr_t Object_KillMe                      = resolve(0x-355f50);  // (Object*) [VERIFY: dup]

// ===== event hook targets (detoured by the loader to fire EetsMod_OnEvent) =====
inline uintptr_t hook_Simulator_LoadWinCondition    = resolve(0x0);  // -> "level_load"  // TODO
inline uintptr_t hook_Simulator_ResetSimulation     = resolve(0x0);  // -> "level_reset"  // TODO
inline uintptr_t hook_LevelManager_CompleteLevel    = resolve(0x0);  // -> "level_complete"  // TODO
inline uintptr_t hook_Creator_StartEetsDeadDialog   = resolve(0x0);  // -> "eets_death"  // TODO
inline uintptr_t hook_Object_KillMe                 = resolve(0x-355f50);  // -> "object_killed" [VERIFY: dup]
inline uintptr_t hook_ObjectMgr_CreateObject        = resolve(0x0);  // -> "object_spawn"  // TODO
inline uintptr_t hook_World_ChangeEmotion           = resolve(0x-324710);  // -> "emotion_change" (ulong hash, uint emotion) [VERIFY: dup]
inline uintptr_t hook_World_CheckGoal               = resolve(0x-3246f0);  // -> "goal_check" (Object*) [VERIFY: dup]
inline uintptr_t hook_Creator_OnEndEetsDeadDialog   = resolve(0x0);  // -> "eets_death" (Creator*, int)  // TODO
inline uintptr_t MotionModel_PushMotion             = resolve(0x0);  // (MotionModel*, char const*, bool, bool)  // TODO
inline uintptr_t MotionModel_PopMotion              = resolve(0x0);  // (MotionModel*)  // TODO
inline uintptr_t MotionModel_GetCurrentMotionName   = resolve(0x0);  // (MotionModel*) -> char const*  // TODO

// ===== singletons / engine / UI =====
inline uintptr_t ObjectMgr_i                        = resolve(0x0);  // () -> ObjectMgr*  // TODO
inline uintptr_t Simulator_i                        = resolve(0x0);  // () -> Simulator*  // TODO
inline uintptr_t GraphicsEngine_i                   = resolve(0x0);  // () -> GraphicsEngine*  (w@+0x40 h@+0x44)  // TODO
inline uintptr_t printText                          = resolve(0x0);  // (int x,int y,char const*,Color const&)  // TODO
inline uintptr_t TextPrinter_DrawString             = resolve(0x0);  // (string,size,style,Color,Vector2,bool,Vector2 const&)  // TODO
inline uintptr_t GraphicsEngine_DrawLine            = resolve(0x0);  // (GE*,Vector2 const&,Vector2 const&,Color const&,float) 1px line (width ignored)  // TODO
inline uintptr_t GraphicsEngine_DrawSquare          = resolve(0x0);  // (GE*,Vector2 const& topLeft,Vector2 const& botRight,Color const&) FILLED rect  // TODO
inline uintptr_t GraphicsEngine_DrawCircleFilled    = resolve(0x0);  // (GE*,Vector2 const&,float r,Color const&,int segs)  // TODO

// ===== localization (StringPool) =====
inline uintptr_t StringPool_instance                = resolve(0x0);  // StringPool* (read the pointer)  // TODO
inline uintptr_t StringPool_Resolve                 = resolve(0x0);  // (StringPool*, char const* id) -> char const*  // TODO
inline uintptr_t StringPool_LoadFile                = resolve(0x0);  // (char const* path) load extra strings  // TODO

// ===== assets (ADVANCED/experimental: ABI not wrapper-validated) =====
inline uintptr_t Texture_Load                       = resolve(0x0);  // (Texture* this, char const* path)  // TODO
inline uintptr_t Texture_UploadTexture              = resolve(0x0);  // (Texture* this, int)  // TODO
inline uintptr_t IGraphicsEngine_i                  = resolve(0x0);  // () -> IGraphicsEngine*  // TODO
inline uintptr_t IGraphicsEngine_DrawTexture        = resolve(0x0);  // (IGE*, Texture const*, Vector2 const&)  // TODO
inline uintptr_t AnimExt_LoadAnimation              = resolve(0x0);  // (char const* path) -> anim  // TODO
inline uintptr_t Animation_operator_new             = resolve(0x0);  // (0x78) -> Animation*  // TODO
inline uintptr_t Animation_ctor                     = resolve(0x0);  // (Animation* this[0x78], char const* path, bool)  // TODO
inline uintptr_t Animation_Update                   = resolve(0x0);  // (Animation*, float dt)  // TODO
inline uintptr_t Animation_GetCurrentFrame          = resolve(0x0);  // (Animation*) -> Sprite*  // TODO
inline uintptr_t Animation_FrameCount               = resolve(0x0);  // (Animation*) -> unsigned  // TODO
inline uintptr_t Animation_SetCurrentFrame          = resolve(0x0);  // (Animation*, unsigned idx)  // TODO
inline uintptr_t Animation_Restart                  = resolve(0x0);  // (Animation*)  // TODO
inline uintptr_t TextureManager_instance            = resolve(0x0);  // TextureManager* (cache = unordered_map<string,KLEITEXTURE*> at +0)  // TODO
inline uintptr_t TextureManager_LoadTexture         = resolve(0x0);  // (TM*, string const&, ImageFormat, bool) load+cache  // TODO
inline uintptr_t SpriteManager_i                    = resolve(0x0);  // () -> SpriteManager*  // TODO
inline uintptr_t SpriteManager_Load                 = resolve(0x0);  // (sret{Sprite*,ctrl}, SM*, string const&, ImageFormat)  // TODO
inline uintptr_t GraphicsEngine_DrawSprite          = resolve(0x0);  // (GE*, Sprite*, Vector2 pos, Vector2 uv0, Vector2 uv1, Color)  // TODO
inline uintptr_t Sprite_GetWidth                    = resolve(0x0);  // (Sprite*) -> unsigned  // TODO
inline uintptr_t Sprite_GetHeight                   = resolve(0x0);  // (Sprite*) -> unsigned  // TODO
inline uintptr_t Sprite_GetDiffuseUV                = resolve(0x0);  // (Sprite*, Vector2& uv0, Vector2& uv1) per-frame texcoords  // TODO

// ===== object -> extension accessors (each reads a fixed Object+offset, returns Ext* or null) =====
inline uintptr_t Object_GetPositionExtension        = resolve(0x-319f90);  // (Object*) -> PositionExtension*
inline uintptr_t Object_GetSuckableExtension        = resolve(0x-319f70);  // (Object*) -> SuckableExtension*
inline uintptr_t Object_GetWalkingExtension         = resolve(0x-319f80);  // (Object*) -> WalkingExtension*
inline uintptr_t Object_GetThwackerExtension        = resolve(0x-319ff0);  // (Object*) -> ThwackerExtension*
inline uintptr_t Object_GetEmotionPlatformExtension = resolve(0x-31a010);  // (Object*) -> EmotionPlatformExtension*
inline uintptr_t Object_GetEdibleExtension          = resolve(0x-31a000);  // (Object*) -> EdibleExtension*
inline uintptr_t Object_GetEmotionExtension         = resolve(0x-319fe0);  // (Object*) -> EmotionExtension*
inline uintptr_t Object_GetFlyingExtension          = resolve(0x-319fc0);  // (Object*) -> FlyingExtension*
inline uintptr_t Object_GetLightingExtension        = resolve(0x-319fd0);  // (Object*) -> LightingExtension*
inline uintptr_t Object_GetHoldingExtension         = resolve(0x-319fa0);  // (Object*) -> HoldingExtension*
inline uintptr_t Object_GetRollingExtension         = resolve(0x-312c30);  // (Object*) -> RollingExtension*
inline uintptr_t PhysicsExtension_GetAccumulate     = resolve(0x0);  // (Physics*) -> deque<WorldCollisionReport>& (sets accumulate flag)  // TODO
inline uintptr_t PhysicsExtension_GetCollisions     = resolve(0x0);  // (Physics*) const -> deque<WorldCollisionReport>&  // TODO

// ===== extension methods (thiscall: first arg is the extension pointer) =====
inline uintptr_t WalkingExtension_SetWalkSpeed      = resolve(0x106370);  // (Walking*, float)
inline uintptr_t WalkingExtension_SetActive         = resolve(0x0);  // (Walking*, bool)  // TODO
inline uintptr_t WalkingExtension_GetState          = resolve(0x0);  // (Walking*) const -> unsigned long (WES_*)  // TODO
inline uintptr_t WalkingExtension_StartWalking      = resolve(0x105ab0);  // (Walking*)
inline uintptr_t WalkingExtension_StopWalking       = resolve(0x1071b0);  // (Walking*)
inline uintptr_t WalkingExtension_KnockDown         = resolve(0x107200);  // (Walking*, Vector2 const&)
inline uintptr_t WalkingExtension_SetNoWalkFrame    = resolve(0x107170);  // (Walking*, int)
inline uintptr_t WalkingExtension_ForceReset        = resolve(0x0);  // (Walking*)  // TODO
inline uintptr_t WalkingExtension_Reset             = resolve(0x106fe0);  // (Walking*)
inline uintptr_t ThwackerExtension_SetThwackSpeed   = resolve(0x0);  // (Thwacker*, float)  // TODO
inline uintptr_t ThwackerExtension_IsThwacking      = resolve(0x0);  // (Thwacker*) const -> bool  // TODO
inline uintptr_t ThwackerExtension_GetCentre        = resolve(0x0);  // (Thwacker*) const -> Vector2  // TODO
inline uintptr_t EdibleExtension_GetEaten           = resolve(0x0);  // (Edible*) const -> bool  // TODO
inline uintptr_t EdibleExtension_GetEater           = resolve(0x0);  // (Edible*) const -> unsigned int  // TODO
inline uintptr_t EdibleExtension_IsEatenBy          = resolve(0x0);  // (Edible*, unsigned int) const -> bool  // TODO
inline uintptr_t LightingExtension_IsLit            = resolve(0x0);  // (Lighting*) const -> bool  // TODO
inline uintptr_t SuckableExtension_WasRecentlySucked = resolve(0xf8930);  // (Suckable*) const -> bool
inline uintptr_t SuckableExtension_SetSucked        = resolve(0x0);  // (Suckable*)  // TODO
inline uintptr_t PositionExtension_IsForeground     = resolve(0xfedf0);  // (Position*) const -> bool
inline uintptr_t PositionExtension_IsBackground     = resolve(0xd45c0);  // (Position*) const -> bool
inline uintptr_t PositionExtension_SetIsForeground  = resolve(0xd45b0);  // (Position*, bool)
inline uintptr_t PositionExtension_SetIsBackground  = resolve(0x0);  // (Position*, bool)  // TODO
inline uintptr_t RollingExtension_IsRolling         = resolve(0x0);  // (Rolling*) const -> bool  // TODO
inline uintptr_t HoldingExtension_IsHolding         = resolve(0xfe930);  // (Holding*, Object const*) const -> bool
inline uintptr_t HoldingExtension_IsHoldingAny      = resolve(0xfe850);  // (Holding*) const -> bool
inline uintptr_t HoldingExtension_HoldObject        = resolve(0xd38e0);  // (Holding*, Object*)
inline uintptr_t HoldingExtension_ReleaseAll        = resolve(0x0);  // (Holding*)  // TODO
inline uintptr_t HoldingExtension_ReleaseObject     = resolve(0x0);  // (Holding*, Object const*)  // TODO
inline uintptr_t HoldingExtension_GetHolds          = resolve(0xfe940);  // (Holding*) const -> vector<Object*>&
inline uintptr_t FlyingExtension_SetState           = resolve(0xf75e0);  // (Flying*, unsigned long FS_*)
inline uintptr_t FlyingExtension_GetState           = resolve(0x0);  // (Flying*) const -> unsigned long (FS_*)  // TODO
inline uintptr_t EmotionExtension_RecentlyChanged   = resolve(0x0);  // (Emotion*) const -> bool  // TODO
inline uintptr_t EmotionExtension_GetEmotionName    = resolve(0x0);  // (Emotion*) const -> char const*  // TODO
inline uintptr_t EmotionExtension_SetEmotionName    = resolve(0x0);  // (Emotion*, char const*)  // TODO
inline uintptr_t EmotionPlatformExtension_GetCurrentEmotion = resolve(0xfdf40);  // (EmoPlat*) const -> unsigned int
inline uintptr_t EmotionPlatformExtension_MatchesCurrentEmotion = resolve(0x0);  // (EmoPlat*) const -> bool  // TODO
inline uintptr_t EmotionPlatformExtension_SetEmotion = resolve(0x0);  // (EmoPlat*, unsigned int)  // TODO

}} // namespace Eets::addr
