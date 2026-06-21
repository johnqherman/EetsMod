# Eets Lua API Reference

Verified from the live `_G` dump + binary symbols + shipped-script usage.
Lua 5.0. Globals are callable from any mod; classes are luabind userdata with
constructors exposed as globals.

> Note: `#t` length operator does **not** exist in 5.0 - use `table.getn(t)`.
> String patterns use `string.gfind` (not `gmatch`). `math.mod` (not `%`).

---

## Global functions

### World
```
World_CreateObject(name, Vector2) -> Object       World_GetObjects() -> iterator
World_GetObjectByID(id) -> Object                 World_GetAddedObjects() -> iterator
World_GetEets() -> Object                          World_HashString(name) -> hash
World_GetGravity() -> Vector2                       World_SetGravity(Vector2, int)
World_CreateEffect(name, Vector2)                  World_CreateExplosion(Vector2, radius)
World_CreateExplosionSpecial()                     World_CreateLight(Vector2, Colour, a, b)
World_MoveLight(id, Vector2)                        World_RemoveLight(id)
World_LightExists(id) -> bool                       World_SetLightAttenuation(id,f,f,f)
World_SetGlobalAmbient(Colour)                      World_SetLevelModulate(Colour)
World_Alert(Vector2, radius)                        World_Scare(Vector2, radius, int)
World_ChangeEmotion(hash, emotionId)               World_ScanForClosestEdible(obj,pos,f,f,bool)
World_CheckGoal(Object)                             World_SetWinFlag()
World_Pause()  World_IsPaused()  World_IsSimulating()
World_SetGameSpeed(n)  World_SetFPS(n)  World_SetMaximumSpeed(f)
World_IsInLevelEditor()  World_IsInMainMenu()
World_AddBackgroundImage(name,Vector2,Vector2,f,Colour,f)   (Foreground variant too)
World_SetGFXViewOffset(Vector2, Vector2)            World_SetTextHint(text, Vector2)
World_GetDeterministicDouble() -> n                 World_GetDeterministicIntRange(a,b) -> n
World_IncrementStat(name)                           World_SaveLevel()
World_ShowGoalBox(...)  World_ShowGoalBoxFX(name,Vector2)  World_ShowClickables()
World_ShowTutorialDialog(t)  World_ShowSolutionTime(f)  World_ShowPuzzleAsNew()
World_SelectAll()  World_CopyItem()  World_PasteItem()  World_MerchMerchPickUp()
World_AnimationsEqual(a, b) -> bool
```
> `World_GetObjects()` / `GetAddedObjects()` return Lua iterators:
> `for obj in World_GetObjects() do ... end`

### Object / Sound / FX / Profile / Misc
```
Object_SetAttributeFloat(this, name, value)        Sound_CreateSound(name,loop,vol,Vector2)
Sound_PlayMusic(name, loop)                         Sound_SetMusicVolume(n)
FX_SpawnEffect(name, Vector2)                       Creator_Undo()
Profile_Start(tag)   Profile_Stop(tag)   Profile()  PauseProfile()
Print(s)             Misc_SaveScreenshot()          LoadFile(path)
Bind(key, luaCodeString)        -- e.g. Bind("CTRL+G", "myfn()")
Debug_DrawLine(...)  Debug_DrawText(...)            -- debug builds
```

### Engine global hooks (override/chain these - framework already does)
```
Update()        -- called every frame
OnPause()  OnUnpause()
stepOrToggleSimulation()  toggleStepping()  g_stepping (bool)
```

---

## Classes (constructors are globals)

### Vector2(x, y)
Fields `.x .y` (read/write). Methods:
`Length() -> f`, `LengthSqr() -> f`, `Normalize()`, `PerpDot(Vector2) -> f`.

### Colour(...)  - for lights / ambient / modulate / particle tint.

### Object  (the core entity)
```
GetPosition() -> Vector2        SetPosition(Vector2)     ForcePosition(Vector2)
GetVelocity() -> Vector2        ApplyImpulse(Vector2)
GetID() -> id                   GetBlueprintHash() -> h   GetBlueprintName() -> s
GetFacing() -> f  SetFacing(f)  ForceFacing(f)
GetFlipped() -> bool  SetFlipped(bool)
IsVisible() / SetVisibility(b) / GetVisibility()
IsValid() -> bool   Validate()   KillMe()
EnablePhysics(bool)  IsPhysicsEnabled()  EnableCollisions(bool)
GetCollisionReports() -> iterator   SetAltCollision(name)
CallFunction(name)              CreateEffect(name) -> FXHandle
IsWalker() IsRolling()  HoldObject(obj) IsHolding(obj) IsHoldingAny() ReleaseAll()
GetMotionModel() -> MotionModel
-- extension accessors (nil if object lacks it):
GetWalkingExtension() GetHoldingExtension() GetFlyingExtension()
GetEmotionExtension() GetEmotionPlatformExtension() GetPositionExtension()
GetLightingExtension() GetEdibleExtension() GetSuckableExtension()
GetThwackerExtension() GetRollingExtension()
```

### MotionModel  (animation state machine on an Object)
```
PushMotion(name, loopBool, b)   PopMotion()   ClearMotions()
SetMotion(name)  SetDefaultMotion(name)
GetCurrentMotion() -> Motion    GetCurrentMotionName() -> s
GetCurrentAnim() -> Animation   GetCurrentAnimName() -> s
```

### Motion -> GetCurrentAnim() -> Animation
### Animation
```
GetCurrentFrameIndex() -> n   SetCurrentFrameIndex(n)   FrameCount() -> n
```

### Extensions (selected, by observed methods)
```
WalkingExtension:  GetState() SetState(s) SetActive(bool) SetWalkSpeed(f)
HoldingExtension:  IsHoldingAny() IsHolding(obj) HoldObject(obj) ReleaseAll() GetHolds()
FlyingExtension:   GetState() ...
EmotionExtension:  MatchesCurrentEmotion() RecentlyChanged()
EmotionPlatformExtension: (emotion id query)
PositionExtension: IsBackground() IsForeground() SetIsBackground(b) SetIsForeground(b)
LightingExtension: IsLit()
EdibleExtension:   GetEaten()
SuckableExtension: WasRecentlySucked()
ThwackerExtension: SetThwackSpeed(f) Trigger()
RollingExtension / SoundHandle / FXHandle / FXParticle: see symbol dump
```

### FX
```
FXHandle:    IsValid()  Stop()  StopPatch()  SetPosition(Vector2)  ...
FXParticle:  SetScale(f) SetParticleAnim(name) SetPosition(Vector2)
             GetColour()->Colour SetColour(Colour) ...
SoundHandle: Stop()
```

---

## Object/Extension data schema (for blueprint mods)

`Data/Objects/<name>.lua` defines `ObjectData = { MotionModel = {...},
Extensions = { PhysicsExtension={}, PositionExtension={}, ... } }` plus a
`-- Begin Scripting --` section with `OnInit()` / `Update()` / `OnHit()` and
`this` bound to the Object. Register the blueprint in `Data/AllObjects.lua`.

Extension defaults live in `Data/Extensions/<X>Extension.lua` (e.g.
`PhysicsExtension = { mass=10, restitution=0.6, static_friction=0.7, ... }`).
