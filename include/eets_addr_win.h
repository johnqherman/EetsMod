#pragma once
// Windows (PE32 i386) RVAs for Eets.exe, resolved at runtime. Object/extension methods
// from class registrars (backward-paired funcptr). struct-return methods return Vector2*
// (pointer in EAX), deref in wrapper - not sret
#include <cstdint>
#include <windows.h>
namespace Eets { namespace addr {
inline uintptr_t resolve(uintptr_t rva){ return rva?(uintptr_t)GetModuleHandleA(nullptr)+rva:0; }

// ===== Lua-binding statics (all 76) =====
inline uintptr_t Anim_GetCurrentFrameIndex          = resolve(0x53a70);  // (Anim::Animation*) -> [this+0x30]+1 (1-based)
inline uintptr_t Anim_SetCurrentFrameIndex          = resolve(0x50880);  // (Anim::Animation*, unsigned) same setter as Animation_SetCurrentFrame
inline uintptr_t Creator_Undo                       = resolve(0xd9c30);  // ()
inline uintptr_t Misc_BindKey                       = resolve(0xd9d60);  // (char const*, char const*) wraps FUN_00498650
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
inline uintptr_t Object_GetPosition                 = resolve(0xaa0e0);  // (Object*) -> Vector2* in EAX (ptr-return, matches Linux ABI). this+0x38->[+0x14]+0xc draw-anchor chain; standard objects, Eets-special for player
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
inline uintptr_t hook_Simulator_LoadWinCondition    = resolve(0x1378c0);  // Simulator::LoadWinCondition (level_load)
inline uintptr_t hook_Simulator_ResetSimulation     = resolve(0x136090);  // Simulator::ResetSimulation (level_reset)
inline uintptr_t hook_LevelManager_CompleteLevel    = resolve(0xc98f0);  // LevelManager::CompleteLevel (level_complete); sets complete byte, unlock, save
inline uintptr_t hook_Creator_StartEetsDeadDialog   = resolve(0x12b5d0);  // Creator name-based modal opener; filter name=='EetsDeadDialog'
inline uintptr_t hook_Object_KillMe                 = resolve(0xaa2a0);  // Object::KillMe (object_killed); sets death flag this+0x30=0x100, fires OnDestroy
inline uintptr_t hook_ObjectMgr_CreateObject        = resolve(0xac560);  // ObjectMgr::CreateObject (object_spawn); hashes name, allocs Object, pushes to vector
inline uintptr_t hook_World_ChangeEmotion           = resolve(0xdb8f0);  // -> "emotion_change" (ulong hash, uint emotion)
inline uintptr_t hook_World_CheckGoal               = resolve(0xdb910);  // -> "goal_check" (Object*)
inline uintptr_t hook_Creator_OnEndEetsDeadDialog   = resolve( 0x1293d0);  // Creator::OnEndEetsDeadDialog (eets_death dialog close); __thiscall(Creator*, int) RET 4
inline uintptr_t MotionModel_PushMotion             = resolve(0x58300);  // (MotionModel*, char const*, bool, bool) thiscall
inline uintptr_t MotionModel_PopMotion              = resolve(0x582a0);  // (MotionModel*) thiscall
inline uintptr_t MotionModel_GetCurrentMotionName   = resolve(0x57d10);  // (MotionModel*) -> char const* thiscall

// ===== singletons / engine / UI =====
inline uintptr_t ObjectMgr_i                        = resolve(0xad2f0);  // () -> ObjectMgr* (MOV EAX,[0xedf168]); ctor FUN_004ace30
inline uintptr_t Simulator_i                        = resolve(0x138d80);  // () -> Simulator* (MOV EAX,[0xee3d9c]); vftable 0x56cf34
inline uintptr_t GraphicsEngine_i                   = resolve(0x6ae51c);  // global DAT_00aae51c = GraphicsEngine (vtable 0x564668); +0x20 = FNA3D device; `this` for vtable draw 0x48d880/0x48e030; accessor 0x481c80
inline uintptr_t printText                          = resolve(0xb9b30);  // (int x,int y,char const*,Color const&) cdecl wrapper -> DrawString(size=3,style=3)
inline uintptr_t TextPrinter_DrawString             = resolve(0x83a40);  // (string const&,size,style,Color,Vector2,bool,Vector2 const&) cdecl; string by ptr, caller-owned (printText frees it)
inline uintptr_t GraphicsEngine_DrawLine            = resolve(0x8d880);  // (GE*,Vector2 const&,Vector2 const&,Color const&) FNA3D prim 2 (LineList,1); R<->B swizzle confirmed
inline uintptr_t GraphicsEngine_DrawSquare          = resolve(0x8e030);  // (GE*,Vector2 const& topLeft,Vector2 const& botRight,Color const&) FNA3D prim 0 (TriList,2) filled rect
inline uintptr_t GraphicsEngine_DrawCircleFilled    = resolve(0);  // absent in Win build: full GE vtable @0x564668 has no circle method; Eets draws circles as whitecircle sprites. FillCircle guards on 0

// ===== localization (StringPool) =====
inline uintptr_t StringPool_instance                = resolve(0xae3be8);  // StringPool* global DAT_00ee3be8 (read the pointer)
inline uintptr_t StringPool_Resolve                 = resolve(0xba5f0);  // (StringPool* this, char const* id) thiscall RET 4; '$' keys -> map lookup
inline uintptr_t StringPool_LoadFile                = resolve(0xba0e0);  // (StringPool* this, char const* path) thiscall; fopen+parse id->string

// ===== assets (ADVANCED/experimental: ABI not wrapper-validated) =====
inline uintptr_t Texture_Load                       = resolve(0x90210);  // (char const* path) SDL_RWFromFile->FNA3D_Image_Load->alloc 0x10 Texture
inline uintptr_t Texture_UploadTexture              = resolve(0x8cb30);  // (Texture* this, int src) FNA3D_CreateTexture2D+SetTextureData2D
inline uintptr_t IGraphicsEngine_i                  = resolve(0x81c80);  // () -> IGraphicsEngine*; same accessor as GraphicsEngine_i (offset-0 base; MOV EAX,[0xaae51c])
inline uintptr_t IGraphicsEngine_DrawTexture        = resolve(0x8df10);  // GE vtable slot0 (Texture const*, Vector2 const&)
inline uintptr_t AnimExt_LoadAnimation              = resolve(0x4e540);  // (this, char const* path) FNV cache + LoadVersion2Anim
inline uintptr_t Animation_operator_new             = resolve(0x4dea0);  // (size) heap alloc; Animation is 0x54 bytes on Win (not 0x78)
inline uintptr_t Animation_ctor                     = resolve(0x4d8b0);  // (Animation* this[0x54], char const* path, bool) loop@0x1c, path SSO@0x50
inline uintptr_t Animation_Update                   = resolve(0x508b0);  // (Animation*, float dt) time@0x34 vs dur@0x18, index@0x30, done@0x38
inline uintptr_t Animation_GetCurrentFrame          = resolve(0x4e530);  // (Animation*) -> Sprite* (base@0 + index@0x30*8)
inline uintptr_t Animation_FrameCount               = resolve(0x4e520);  // (Animation*) -> unsigned ((end-begin)>>3)
inline uintptr_t Animation_SetCurrentFrame          = resolve(0x50880);  // (Animation*, unsigned) writes index@0x30
inline uintptr_t Animation_Restart                  = resolve(0x50860);  // (Animation*) zero index/time/done
inline uintptr_t TextureManager_instance            = resolve(0xadf090);  // TextureManager* global DAT_00edf090 (cache map<string,tex*> at +0)
inline uintptr_t TextureManager_LoadTexture         = resolve(0x91f80);  // (TM*, string const&, int fmt, bool) cache+decode+upload
inline uintptr_t SpriteManager_i                    = resolve(0x89ff0);  // () -> SpriteManager* (mov eax,[0xe7ef38]; ret)
inline uintptr_t SpriteManager_Load                 = resolve(0x896c0);  // (sret{Sprite*,ctrl}, SM* this, string const&, ImageFormat) thiscall, RET 0xc; caches in hashmap
inline uintptr_t GraphicsEngine_DrawSprite          = resolve(0x8dbd0);  // (GE*, Sprite*, Vector2 pos, Vector2 uv0, Vector2 uv1, Color) FNA3D prim 0 (TriList,2)
inline uintptr_t Sprite_GetWidth                    = resolve(0x88170);  // (Sprite*) -> unsigned (mov eax,[ecx+0x60])
inline uintptr_t Sprite_GetHeight                   = resolve(0x88130);  // (Sprite*) -> unsigned (mov eax,[ecx+0x64])
inline uintptr_t Sprite_GetDiffuseUV                = resolve(0x88100);  // (Sprite*, Vector2& uv0, Vector2& uv1) thiscall RET 8; uv0=+0x40/+0x44 uv1=+0x48/+0x4c

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
inline uintptr_t PhysicsExtension_GetAccumulate     = resolve(0x100410);  // (Physics*) -> deque<WorldCollisionReport>& (sets accumulate flag this+0x2a=1; returns &deque@this+0x5c in EAX)
inline uintptr_t PhysicsExtension_GetCollisions     = resolve(0x100410);  // (Physics*) const -> deque<WorldCollisionReport>&; const getter COMDAT-folded, reuse GetAccumulate (same &deque@this+0x5c); flag write harmless

// ===== extension methods (thiscall: first arg is the extension pointer) =====
inline uintptr_t WalkingExtension_SetWalkSpeed      = resolve(0x107170);  // (Walking*, float)
inline uintptr_t WalkingExtension_SetActive         = resolve(0x106fe0);  // (Walking*, bool)
inline uintptr_t WalkingExtension_GetState          = resolve(0xf75e0);  // (Walking*) const -> unsigned long (WES_*)
inline uintptr_t WalkingExtension_StartWalking      = resolve(0x1071b0);  // (Walking*)
inline uintptr_t WalkingExtension_StopWalking       = resolve(0x107200);  // (Walking*)
inline uintptr_t WalkingExtension_KnockDown         = resolve(0x106370);  // (Walking*, Vector2 const&)
inline uintptr_t WalkingExtension_SetNoWalkFrame    = resolve(0x107030);  // (Walking*, int)
inline uintptr_t WalkingExtension_ForceReset        = resolve(0x106e00);  // (Walking*) resets walk-state this+0xe0=2; called by Reset/StopWalking/KnockDown
inline uintptr_t WalkingExtension_Reset             = resolve(0x105ab0);  // (Walking*)
inline uintptr_t ThwackerExtension_SetThwackSpeed   = resolve(0x1036b0);  // (Thwacker*, float)
inline uintptr_t ThwackerExtension_IsThwacking      = resolve(0);  // (Thwacker*) const -> bool; not found standalone (inlined/folded). closest FUN_00503600 (0x103600) = (state@this+0x30==0) inverse; thwacking == state!=0
inline uintptr_t ThwackerExtension_GetCentre        = resolve(0);  // (Thwacker*) const -> Vector2; not found: no Vector2-returning method in Thwacker cluster (0x503000-0x503e00); inlined/folded
inline uintptr_t EdibleExtension_GetEaten           = resolve(0xfcca0);  // (Edible*) const -> bool
inline uintptr_t EdibleExtension_GetEater           = resolve(0);  // (Edible*) const -> unsigned int; not found standalone (inlined/folded). eaters std::set<uint>@this+0x8; would return *begin(); use IsEatenBy(0xfccb0) for membership
inline uintptr_t EdibleExtension_IsEatenBy          = resolve(0xfccb0);  // (Edible*, unsigned int) const -> bool; set<uint> membership lookup on eater-set @this+0x8; RET 0x4
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
inline uintptr_t HoldingExtension_ReleaseObject     = resolve(0);  // (Holding*, Object const*); not found standalone (inlined/folded). container vector<Object*>@this+0x8..0xc; only bulk ReleaseAll(0xfe940) survives; single-object erase inlined at call sites
inline uintptr_t HoldingExtension_GetHolds          = resolve(0xd38e0);  // (Holding*) const -> vector<Object*>&
inline uintptr_t FlyingExtension_SetState           = resolve(0xfe4e0);  // (Flying*, unsigned long FS_*)
inline uintptr_t FlyingExtension_GetState           = resolve(0xf75e0);  // (Flying*) const -> unsigned long (FS_*)
inline uintptr_t EmotionExtension_RecentlyChanged   = resolve(0xfda90);  // (Emotion*) const -> bool
inline uintptr_t EmotionExtension_GetEmotionName    = resolve(0xfd940);  // (Emotion*) const -> char const*; c_str of emotion-name std::string @this+0x20
inline uintptr_t EmotionExtension_SetEmotionName    = resolve(0);  // (Emotion*, char const*); not found: no standalone char* setter (inlined/folded); member std::string @this+0x20 (use assign helper FUN_0043fea0)
inline uintptr_t EmotionPlatformExtension_GetCurrentEmotion = resolve(0xfdd20);  // (EmoPlat*) const -> unsigned int
inline uintptr_t EmotionPlatformExtension_MatchesCurrentEmotion = resolve(0xfdf40);  // (EmoPlat*) const -> bool
inline uintptr_t EmotionPlatformExtension_SetEmotion = resolve(0xfdfe0);  // (EmoPlat*, unsigned int) stores idx to this+0x14 (GetCurrentEmotion reads) + fires change notify; RET 0x4

}} // namespace Eets::addr
