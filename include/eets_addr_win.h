#pragma once
// Windows (PE32 i386) RVAs for Eets.exe, resolved at runtime. Object/extension methods
// from the class registrars (backward-paired funcptr). Struct-return methods return a
// Vector2* (pointer in EAX), deref in the wrapper - NOT sret.
#include <cstdint>
#include <windows.h>
namespace Eets { namespace addr {
inline uintptr_t resolve(uintptr_t rva){ return rva?(uintptr_t)GetModuleHandleA(nullptr)+rva:0; }

// ===== Lua-binding statics (all 76) =====
inline uintptr_t Anim_GetCurrentFrameIndex          = resolve(0);  // (Anim::Animation*)  // TODO
inline uintptr_t Anim_SetCurrentFrameIndex          = resolve(0);  // (Anim::Animation*, unsigned int)  // TODO
inline uintptr_t Creator_Undo                       = resolve(0xd9c30);  // ()
inline uintptr_t Misc_BindKey                       = resolve(0);  // (char const*, char const*)  // TODO
inline uintptr_t Misc_PauseProfile                  = resolve(0xd9d80);  // ()
inline uintptr_t Misc_Print                         = resolve(0xd9d90);  // (char const*)
inline uintptr_t Misc_Profile                       = resolve(0xd9da0);  // ()
inline uintptr_t Misc_SaveScreenshot                = resolve(0xd9db0);  // ()
inline uintptr_t Object_ApplyImpulse                = resolve(0xed170);  // (Object*, Vector2 const&)
inline uintptr_t Object_CallFunction                = resolve(0xed190);  // (Object*, char const*)
inline uintptr_t Object_CreateEffect                = resolve(0xed1a0);  // (Object*, char const*)
inline uintptr_t Object_EnableCollisions            = resolve(0xed1d0);  // (Object*, bool)
inline uintptr_t Object_EnablePhysics               = resolve(0xed1f0);  // (Object*, bool)
inline uintptr_t Object_GetBlueprintHash            = resolve(0xed210);  // (Object*)
inline uintptr_t Object_GetBlueprintName            = resolve(0xed220);  // (Object*)
inline uintptr_t Object_GetCollisionReports         = resolve(0xed240);  // (Object*)
inline uintptr_t Object_HoldObject                  = resolve(0xed290);  // (Object*, Object*)
inline uintptr_t Object_IsHolding                   = resolve(0xed2d0);  // (Object*, Object*)
inline uintptr_t Object_IsPhysicsEnabled            = resolve(0xed310);  // (Object*)
inline uintptr_t Object_IsRolling                   = resolve(0xed320);  // (Object*)
inline uintptr_t Object_IsWalker                    = resolve(0xed370);  // (Object*)
inline uintptr_t Object_ReleaseAll                  = resolve(0xed390);  // (Object*)
inline uintptr_t Object_SetAltCollision             = resolve(0xed3d0);  // (Object*, char const*)
inline uintptr_t Sound_CreateSound                  = resolve(0xdb790);  // (char const*, bool, float, Vector2 const&)
inline uintptr_t Sound_PlayMusic                    = resolve(0xdb7c0);  // (char const*, bool)
inline uintptr_t Sound_SetMusicVolume               = resolve(0xdb7e0);  // (long)
inline uintptr_t World_AddBackgroundImage           = resolve(0xdb800);  // (char const*, Vector2 const&, Vector2 const&, float, Color const&, float)
inline uintptr_t World_AddForegroundImage           = resolve(0xdb840);  // (char const*, Vector2 const&, Vector2 const&, float, Color const&, float)
inline uintptr_t World_Alert                        = resolve(0xdb880);  // (Vector2 const&, float)
inline uintptr_t World_AnimationsEqual              = resolve(0xdb8e0);  // (Anim::Animation const*, Anim::Animation const*)
inline uintptr_t World_ChangeEmotion                = resolve(0xdb8f0);  // (unsigned long, unsigned int)
inline uintptr_t World_CheckGoal                    = resolve(0xdb910);  // (Object*)
inline uintptr_t World_CopyItem                     = resolve(0xdb9d0);  // ()
inline uintptr_t World_CreateEffect                 = resolve(0xdb9f0);  // (char const*, Vector2)
inline uintptr_t World_CreateExplosion              = resolve(0xdba10);  // (Vector2 const&, float)
inline uintptr_t World_CreateExplosionSpecial       = resolve(0xdba90);  // ()
inline uintptr_t World_CreateLight                  = resolve(0xdbd20);  // (Vector2 const&, Color const&, float, float)
inline uintptr_t World_CreateObject                 = resolve(0xdbe60);  // (char const*, Vector2)
inline uintptr_t World_GetAddedObjects              = resolve(0xdbe80);  // ()
inline uintptr_t World_GetDeterministicDouble       = resolve(0xdbe90);  // ()
inline uintptr_t World_GetDeterministicIntRange     = resolve(0xdbea0);  // (int, int)
inline uintptr_t World_GetEets                      = resolve(0xdbec0);  // ()
inline uintptr_t World_GetGravity                   = resolve(0xdbef0);  // ()
inline uintptr_t World_GetObjectByID                = resolve(0xdbfc0);  // (unsigned long)
inline uintptr_t World_GetObjects                   = resolve(0xdbfe0);  // ()
inline uintptr_t World_IncrementStat                = resolve(0xdc000);  // (char const*)
inline uintptr_t World_IsInLevelEditor              = resolve(0xdc030);  // ()
inline uintptr_t World_IsInMainMenu                 = resolve(0xdc070);  // ()
inline uintptr_t World_IsPaused                     = resolve(0xdc080);  // ()
inline uintptr_t World_IsSimulating                 = resolve(0xdc090);  // ()
inline uintptr_t World_LightExists                  = resolve(0xdc0a0);  // (unsigned short)
inline uintptr_t World_MerchMerchPickUp             = resolve(0xdc0d0);  // ()
inline uintptr_t World_MoveLight                    = resolve(0xdc140);  // (unsigned short, Vector2 const&)
inline uintptr_t World_PasteItem                    = resolve(0xdc190);  // ()
inline uintptr_t World_Pause                        = resolve(0xdc1b0);  // ()
inline uintptr_t World_RemoveLight                  = resolve(0xdc1d0);  // (unsigned short)
inline uintptr_t World_SaveLevel                    = resolve(0xdc1e0);  // ()
inline uintptr_t World_ScanForClosestEdible         = resolve(0xdc260);  // (Object const*, Vector2 const&, float, float, bool)
inline uintptr_t World_Scare                        = resolve(0xdc510);  // (Vector2 const&, float, int)
inline uintptr_t World_SelectAll                    = resolve(0xdc580);  // ()
inline uintptr_t World_SetFPS                       = resolve(0xdc5b0);  // (int)
inline uintptr_t World_SetGameSpeed                 = resolve(0xdc5f0);  // (unsigned long)
inline uintptr_t World_SetGFXViewOffset             = resolve(0xdc5d0);  // (Vector2 const&, Vector2 const&)
inline uintptr_t World_SetGlobalAmbient             = resolve(0xdc610);  // (Color const&)
inline uintptr_t World_SetGravity                   = resolve(0xdc6d0);  // (Vector2, int)
inline uintptr_t World_SetLevelModulate             = resolve(0xdc700);  // (Color const&)
inline uintptr_t World_SetLightAttenuation          = resolve(0xdc720);  // (unsigned short, float, float, float)
inline uintptr_t World_SetMaximumSpeed              = resolve(0xdc770);  // (float)
inline uintptr_t World_SetTextHint                  = resolve(0xdc790);  // (char const*, Vector2 const&)
inline uintptr_t World_SetWinFlag                   = resolve(0xdc7d0);  // ()
inline uintptr_t World_ShowClickables               = resolve(0xdc800);  // ()
inline uintptr_t World_ShowGoalBox                  = resolve(0xdc820);  // (char const*, Vector2 const&, char const*, char const*, char const*)
inline uintptr_t World_ShowGoalBoxFX                = resolve(0xdc870);  // (char const*, Vector2 const&)
inline uintptr_t World_ShowPuzzleAsNew              = resolve(0xdc8a0);  // ()
inline uintptr_t World_ShowSolutionTime             = resolve(0xdc8d0);  // (float)
inline uintptr_t World_ShowTutorialDialog           = resolve(0xdc900);  // (char const*)

// ===== class methods (__thiscall: first arg is the object) =====
inline uintptr_t Object_GetPosition                 = resolve(0xaa0e0);  // (Object*) -> Vector2
inline uintptr_t Object_GetVelocity                 = resolve(0xaa190);  // (Object*) -> Vector2
inline uintptr_t Object_GetID                       = resolve(0xaa0b0);  // (Object*) -> unsigned long
inline uintptr_t Object_GetMotionModel              = resolve(0xed050);  // (Object*) -> MotionModel*
inline uintptr_t Object_SetPosition                 = resolve(0xab170);  // (Object*, Vector2 const&)
inline uintptr_t Object_ForcePosition               = resolve(0xaa040);  // (Object*, Vector2 const&)
inline uintptr_t Object_SetFacing                   = resolve(0xab0a0);  // (Object*, Vector2 const&)
inline uintptr_t Object_GetFacing                   = resolve(0xaa0f0);  // (Object*) -> Vector2
inline uintptr_t Object_SetFlipped                  = resolve(0xee1b0);  // (Object*, bool)
inline uintptr_t Object_GetFlipped                  = resolve(0xed040);  // (Object*) -> bool
inline uintptr_t Object_KillMe                      = resolve(0xaa2a0);  // (Object*)

// ===== event hook targets (detoured by the loader to fire EetsMod_OnEvent) =====
inline uintptr_t hook_Simulator_LoadWinCondition    = resolve(0);  // -> "level_load"  // TODO
inline uintptr_t hook_Simulator_ResetSimulation     = resolve(0);  // -> "level_reset"  // TODO
inline uintptr_t hook_LevelManager_CompleteLevel    = resolve(0);  // -> "level_complete"  // TODO
inline uintptr_t hook_Creator_StartEetsDeadDialog   = resolve(0);  // -> "eets_death"  // TODO
inline uintptr_t hook_Object_KillMe                 = resolve(0);  // -> "object_killed"  // TODO
inline uintptr_t hook_ObjectMgr_CreateObject        = resolve(0);  // -> "object_spawn"  // TODO
inline uintptr_t hook_World_ChangeEmotion           = resolve(0xdb8f0);  // -> "emotion_change" (ulong hash, uint emotion)
inline uintptr_t hook_World_CheckGoal               = resolve(0xdb910);  // -> "goal_check" (Object*)
inline uintptr_t hook_Creator_OnEndEetsDeadDialog   = resolve(0);  // -> "eets_death" (Creator*, int)  // TODO
inline uintptr_t MotionModel_PushMotion             = resolve(0);  // (MotionModel*, char const*, bool, bool)  // TODO
inline uintptr_t MotionModel_PopMotion              = resolve(0);  // (MotionModel*)  // TODO
inline uintptr_t MotionModel_GetCurrentMotionName   = resolve(0);  // (MotionModel*) -> char const*  // TODO

// ===== singletons / engine / UI =====
inline uintptr_t ObjectMgr_i                        = resolve(0);  // () -> ObjectMgr*  // TODO
inline uintptr_t Simulator_i                        = resolve(0);  // () -> Simulator*  // TODO
inline uintptr_t GraphicsEngine_i                   = resolve(0);  // () -> GraphicsEngine*  (w@+0x40 h@+0x44)  // TODO
inline uintptr_t printText                          = resolve(0);  // (int x,int y,char const*,Color const&)  // TODO
inline uintptr_t TextPrinter_DrawString             = resolve(0);  // (string,size,style,Color,Vector2,bool,Vector2 const&)  // TODO
inline uintptr_t GraphicsEngine_DrawLine            = resolve(0);  // (GE*,Vector2 const&,Vector2 const&,Color const&,float) 1px line (width ignored)  // TODO
inline uintptr_t GraphicsEngine_DrawSquare          = resolve(0);  // (GE*,Vector2 const& topLeft,Vector2 const& botRight,Color const&) FILLED rect  // TODO
inline uintptr_t GraphicsEngine_DrawCircleFilled    = resolve(0);  // (GE*,Vector2 const&,float r,Color const&,int segs)  // TODO

// ===== localization (StringPool) =====
inline uintptr_t StringPool_instance                = resolve(0);  // StringPool* (read the pointer)  // TODO
inline uintptr_t StringPool_Resolve                 = resolve(0);  // (StringPool*, char const* id) -> char const*  // TODO
inline uintptr_t StringPool_LoadFile                = resolve(0);  // (char const* path) load extra strings  // TODO

// ===== assets (ADVANCED/experimental: ABI not wrapper-validated) =====
inline uintptr_t Texture_Load                       = resolve(0);  // (Texture* this, char const* path)  // TODO
inline uintptr_t Texture_UploadTexture              = resolve(0);  // (Texture* this, int)  // TODO
inline uintptr_t IGraphicsEngine_i                  = resolve(0);  // () -> IGraphicsEngine*  // TODO
inline uintptr_t IGraphicsEngine_DrawTexture        = resolve(0);  // (IGE*, Texture const*, Vector2 const&)  // TODO
inline uintptr_t AnimExt_LoadAnimation              = resolve(0);  // (char const* path) -> anim  // TODO
inline uintptr_t Animation_operator_new             = resolve(0);  // (0x78) -> Animation*  // TODO
inline uintptr_t Animation_ctor                     = resolve(0);  // (Animation* this[0x78], char const* path, bool)  // TODO
inline uintptr_t Animation_Update                   = resolve(0);  // (Animation*, float dt)  // TODO
inline uintptr_t Animation_GetCurrentFrame          = resolve(0);  // (Animation*) -> Sprite*  // TODO
inline uintptr_t Animation_FrameCount               = resolve(0);  // (Animation*) -> unsigned  // TODO
inline uintptr_t Animation_SetCurrentFrame          = resolve(0);  // (Animation*, unsigned idx)  // TODO
inline uintptr_t Animation_Restart                  = resolve(0);  // (Animation*)  // TODO
inline uintptr_t TextureManager_instance            = resolve(0);  // TextureManager* (cache = unordered_map<string,KLEITEXTURE*> at +0)  // TODO
inline uintptr_t TextureManager_LoadTexture         = resolve(0);  // (TM*, string const&, ImageFormat, bool) load+cache  // TODO
inline uintptr_t SpriteManager_i                    = resolve(0);  // () -> SpriteManager*  // TODO
inline uintptr_t SpriteManager_Load                 = resolve(0);  // (sret{Sprite*,ctrl}, SM*, string const&, ImageFormat)  // TODO
inline uintptr_t GraphicsEngine_DrawSprite          = resolve(0);  // (GE*, Sprite*, Vector2 pos, Vector2 uv0, Vector2 uv1, Color)  // TODO
inline uintptr_t Sprite_GetWidth                    = resolve(0);  // (Sprite*) -> unsigned  // TODO
inline uintptr_t Sprite_GetHeight                   = resolve(0);  // (Sprite*) -> unsigned  // TODO
inline uintptr_t Sprite_GetDiffuseUV                = resolve(0);  // (Sprite*, Vector2& uv0, Vector2& uv1) per-frame texcoords  // TODO

// ===== object -> extension accessors (each reads a fixed Object+offset, returns Ext* or null) =====
inline uintptr_t Object_GetPositionExtension        = resolve(0xe6050);  // (Object*) -> PositionExtension*
inline uintptr_t Object_GetSuckableExtension        = resolve(0xe6070);  // (Object*) -> SuckableExtension*
inline uintptr_t Object_GetWalkingExtension         = resolve(0xe6090);  // (Object*) -> WalkingExtension*
inline uintptr_t Object_GetThwackerExtension        = resolve(0xe6080);  // (Object*) -> ThwackerExtension*
inline uintptr_t Object_GetEmotionPlatformExtension = resolve(0xe6010);  // (Object*) -> EmotionPlatformExtension*
inline uintptr_t Object_GetEdibleExtension          = resolve(0xe5ff0);  // (Object*) -> EdibleExtension*
inline uintptr_t Object_GetEmotionExtension         = resolve(0xe6000);  // (Object*) -> EmotionExtension*
inline uintptr_t Object_GetFlyingExtension          = resolve(0xe6020);  // (Object*) -> FlyingExtension*
inline uintptr_t Object_GetLightingExtension        = resolve(0xe6040);  // (Object*) -> LightingExtension*
inline uintptr_t Object_GetHoldingExtension         = resolve(0xe6030);  // (Object*) -> HoldingExtension*
inline uintptr_t Object_GetRollingExtension         = resolve(0xe6060);  // (Object*) -> RollingExtension*
inline uintptr_t PhysicsExtension_GetAccumulate     = resolve(0);  // (Physics*) -> deque<WorldCollisionReport>& (sets accumulate flag)  // TODO
inline uintptr_t PhysicsExtension_GetCollisions     = resolve(0);  // (Physics*) const -> deque<WorldCollisionReport>&  // TODO

// ===== extension methods (thiscall: first arg is the extension pointer) =====
inline uintptr_t WalkingExtension_SetWalkSpeed      = resolve(0x107170);  // (Walking*, float)
inline uintptr_t WalkingExtension_SetActive         = resolve(0x106fe0);  // (Walking*, bool)
inline uintptr_t WalkingExtension_GetState          = resolve(0xf75e0);  // (Walking*) const -> unsigned long (WES_*)
inline uintptr_t WalkingExtension_StartWalking      = resolve(0x1071b0);  // (Walking*)
inline uintptr_t WalkingExtension_StopWalking       = resolve(0x107200);  // (Walking*)
inline uintptr_t WalkingExtension_KnockDown         = resolve(0x106370);  // (Walking*, Vector2 const&)
inline uintptr_t WalkingExtension_SetNoWalkFrame    = resolve(0x107030);  // (Walking*, int)
inline uintptr_t WalkingExtension_ForceReset        = resolve(0);  // (Walking*)  // TODO
inline uintptr_t WalkingExtension_Reset             = resolve(0x105ab0);  // (Walking*)
inline uintptr_t ThwackerExtension_SetThwackSpeed   = resolve(0x1036b0);  // (Thwacker*, float)
inline uintptr_t ThwackerExtension_IsThwacking      = resolve(0);  // (Thwacker*) const -> bool  // TODO
inline uintptr_t ThwackerExtension_GetCentre        = resolve(0);  // (Thwacker*) const -> Vector2  // TODO
inline uintptr_t EdibleExtension_GetEaten           = resolve(0xfcca0);  // (Edible*) const -> bool
inline uintptr_t EdibleExtension_GetEater           = resolve(0);  // (Edible*) const -> unsigned int  // TODO
inline uintptr_t EdibleExtension_IsEatenBy          = resolve(0);  // (Edible*, unsigned int) const -> bool  // TODO
inline uintptr_t LightingExtension_IsLit            = resolve(0xff550);  // (Lighting*) const -> bool
inline uintptr_t SuckableExtension_WasRecentlySucked = resolve(0x102f50);  // (Suckable*) const -> bool
inline uintptr_t SuckableExtension_SetSucked        = resolve(0xf8930);  // (Suckable*)
inline uintptr_t PositionExtension_IsForeground     = resolve(0xfedd0);  // (Position*) const -> bool
inline uintptr_t PositionExtension_IsBackground     = resolve(0xfedf0);  // (Position*) const -> bool
inline uintptr_t PositionExtension_SetIsForeground  = resolve(0xd45c0);  // (Position*, bool)
inline uintptr_t PositionExtension_SetIsBackground  = resolve(0xd45b0);  // (Position*, bool)
inline uintptr_t RollingExtension_IsRolling         = resolve(0x1025d0);  // (Rolling*) const -> bool
inline uintptr_t HoldingExtension_IsHolding         = resolve(0xfe900);  // (Holding*, Object const*) const -> bool
inline uintptr_t HoldingExtension_IsHoldingAny      = resolve(0xfe930);  // (Holding*) const -> bool
inline uintptr_t HoldingExtension_HoldObject        = resolve(0xfe850);  // (Holding*, Object*)
inline uintptr_t HoldingExtension_ReleaseAll        = resolve(0xfe940);  // (Holding*)
inline uintptr_t HoldingExtension_ReleaseObject     = resolve(0);  // (Holding*, Object const*)  // TODO
inline uintptr_t HoldingExtension_GetHolds          = resolve(0xd38e0);  // (Holding*) const -> vector<Object*>&
inline uintptr_t FlyingExtension_SetState           = resolve(0xfe4e0);  // (Flying*, unsigned long FS_*)
inline uintptr_t FlyingExtension_GetState           = resolve(0xf75e0);  // (Flying*) const -> unsigned long (FS_*)
inline uintptr_t EmotionExtension_RecentlyChanged   = resolve(0xfda90);  // (Emotion*) const -> bool
inline uintptr_t EmotionExtension_GetEmotionName    = resolve(0);  // (Emotion*) const -> char const*  // TODO
inline uintptr_t EmotionExtension_SetEmotionName    = resolve(0);  // (Emotion*, char const*)  // TODO
inline uintptr_t EmotionPlatformExtension_GetCurrentEmotion = resolve(0xfdd20);  // (EmoPlat*) const -> unsigned int
inline uintptr_t EmotionPlatformExtension_MatchesCurrentEmotion = resolve(0xfdf40);  // (EmoPlat*) const -> bool
inline uintptr_t EmotionPlatformExtension_SetEmotion = resolve(0);  // (EmoPlat*, unsigned int)  // TODO

}} // namespace Eets::addr
