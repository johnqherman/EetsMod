# Native API reference

Auto-generated from `native/include/*.h` by `tools/gen-api-ref.sh`. Do not edit by hand.
All symbols live in namespace `Eets`. Signatures only; see the headers for offset/ABI notes.

## Engine wrappers (`eets_engine.h`)
```cpp
  bool DrawAnim(const char* path, int x, int y, float dt, float fps = 0.0f, Colour tint = Colour())
  bool DrawImage(const char* path, int x, int y, Colour tint = Colour())
  bool DrawImageHUD(const char* path, int x, int y, Colour tint = Colour())
  bool EdibleExtension_GetEaten(EdibleExtension* e)
  bool EdibleExtension_IsEatenBy(EdibleExtension* e, unsigned id)
  bool EmotionExtension_RecentlyChanged(EmotionExtension* e)
  bool EmotionPlatformExtension_MatchesCurrentEmotion(EmotionPlatformExtension* e)
  bool HoldingExtension_IsHoldingAny(HoldingExtension* h)
  bool HoldingExtension_IsHolding(HoldingExtension* h, Object* o)
  bool LightingExtension_IsLit(LightingExtension* l)
  bool Object_GetFlipped(Object* o)
  bool Object_IsHolding(Object* a, Object* b)
  bool Object_IsPhysicsEnabled(Object* a)
  bool Object_IsRolling(Object* a)
  bool Object_IsWalker(Object* a)
  bool PositionExtension_IsBackground(PositionExtension* p)
  bool PositionExtension_IsForeground(PositionExtension* p)
  bool RollingExtension_IsRolling(RollingExtension* r)
  bool SuckableExtension_WasRecentlySucked(SuckableExtension* s)
  bool ThwackerExtension_IsThwacking(ThwackerExtension* t)
  bool World_AnimationsEqual(void* a, void* b)
  bool World_IsInLevelEditor()
  bool World_IsInMainMenu()
  bool World_IsPaused()
  bool World_IsSimulating()
  bool World_LightExists(unsigned short a)
  Colour swab(Colour c)
  const char* EmotionExtension_GetEmotionName(EmotionExtension* e)
  const char* Localize(const char* id)
  const char* MotionModel_GetCurrentMotionName(MotionModel* m)
  const char* Object_GetBlueprintName(Object* a)
  double World_GetDeterministicDouble()
  EdibleExtension* Object_GetEdibleExtension(Object* o)
  EmotionExtension* Object_GetEmotionExtension(Object* o)
  EmotionPlatformExtension* Object_GetEmotionPlatformExtension(Object* o)
  float AnimFrameDuration(void* a)
  float VecDot(const Vector2& a, const Vector2& b)
  float VecLen(const Vector2& v)
  float VecLenSqr(const Vector2& v)
  FlyingExtension* Object_GetFlyingExtension(Object* o)
  HoldingExtension* Object_GetHoldingExtension(Object* o)
  int AnimFrameCount(void* a)
  int ScreenHeight()
  int ScreenWidth()
  int  SpriteHeight(void* s)
  int  SpriteWidth(void* s)
  int World_GetDeterministicIntRange(int a, int b)
  LightingExtension* Object_GetLightingExtension(Object* o)
  MotionModel* Object_GetMotionModel(Object* o)
  ObjectMgr* ObjectMgr_i()
  Object* World_CreateObject(const char* name, Vector2 pos)
  Object* World_GetEets()
  Object* World_GetObjectByID(unsigned long id)
  Object* World_ScanForClosestEdible(Object* a, const Vector2& b, float c, float d, bool e)
  PhysicsExtension* Object_GetPhysicsExtension(Object* o)
  PositionExtension* Object_GetPositionExtension(Object* o)
  RollingExtension* Object_GetRollingExtension(Object* o)
  SuckableExtension* Object_GetSuckableExtension(Object* o)
  ThwackerExtension* Object_GetThwackerExtension(Object* o)
  unsigned Anim_GetCurrentFrameIndex(void* a)
  unsigned EdibleExtension_GetEater(EdibleExtension* e)
  unsigned EmotionPlatformExtension_GetCurrentEmotion(EmotionPlatformExtension* e)
  unsigned long FlyingExtension_GetState(FlyingExtension* f)
  unsigned long Object_GetBlueprintHash(Object* o)
  unsigned long Object_GetID(Object* o)
  unsigned long WalkingExtension_GetState(WalkingExtension* w)
  Vector2 Object_GetFacing(Object* o)
  Vector2 Object_GetPosition(Object* o)
  Vector2 Object_GetVelocity(Object* o)
  Vector2 ThwackerExtension_GetCentre(ThwackerExtension* t)
  Vector2 VecNormalize(const Vector2& v)
  Vector2 World_GetGravity()
  void Anim_SetCurrentFrameIndex(void* a, unsigned int b)
  void Creator_Undo()
  void DrawLine(Vector2 a, Vector2 b, Colour c, float width = 1.0f)
  void DrawRect(int x, int y, int w, int h, Colour c, float t = 2.0f)
  void DrawSpriteAt(void* sprite, int x, int y, Colour tint = Colour())
  void DrawText(int x, int y, const char* text, Colour c = Colour())
  void DrawTextOutlined(int x, int y, const char* text, int size,
  void DrawTextSized(int x, int y, const char* text, int size,
  void EmotionExtension_SetEmotionName(EmotionExtension* e, const char* n)
  void EmotionPlatformExtension_SetEmotion(EmotionPlatformExtension* e, unsigned id)
  void FillCircle(int x, int y, float r, Colour c, int segs = 24)
  void FillRect(int x, int y, int w, int h, Colour c)
  void FlyingExtension_SetState(FlyingExtension* f, unsigned long s)
  void ForEachCollision(Object* o, Fn fn)
  void ForEachHeld(HoldingExtension* h, Fn fn)
  void ForEachObject(Fn fn)
  void* GraphicsEngine_i()
  void HoldingExtension_HoldObject(HoldingExtension* h, Object* o)
  void HoldingExtension_ReleaseAll(HoldingExtension* h)
  void HoldingExtension_ReleaseObject(HoldingExtension* h, Object* o)
  void* LoadAnim(const char* path)
  void* LoadSprite(const char* path, int format = 0)
  void Misc_BindKey(const char* a, const char* b)
  void Misc_PauseProfile()
  void Misc_Print(const char* s)
  void Misc_Profile()
  void Misc_SaveScreenshot()
  void MotionModel_PopMotion(MotionModel* m)
  void MotionModel_PushMotion(MotionModel* m, const char* name, bool a, bool b)
  void Object_ApplyImpulse(Object* o, const Vector2& impulse)
  void Object_CallFunction(Object* o, const char* fn)
  void Object_CreateEffect(Object* o, const char* fx)
  void Object_EnableCollisions(Object* o, bool e)
  void Object_EnablePhysics(Object* o, bool enabled)
  void Object_ForcePosition(Object* o, const Vector2& p)
  void Object_HoldObject(Object* a, Object* b)
  void Object_KillMe(Object* o)
  void Object_ReleaseAll(Object* a)
  void Object_SetAltCollision(Object* a, const char* b)
  void Object_SetFacing(Object* o, const Vector2& f)
  void Object_SetFlipped(Object* o, bool f)
  void Object_SetPosition(Object* o, const Vector2& p)
  void PlaySound(const char* name, float vol = 0.0f)
  void PositionExtension_SetIsBackground(PositionExtension* p, bool v)
  void PositionExtension_SetIsForeground(PositionExtension* p, bool v)
  void Sound_CreateSound(const char* name, bool loop, float vol, const Vector2& pos)
  void Sound_PlayMusic(const char* name, bool loop)
  void Sound_SetMusicVolume(long v)
  void SuckableExtension_SetSucked(SuckableExtension* s)
  void ThwackerExtension_SetThwackSpeed(ThwackerExtension* t, float s)
  void WalkingExtension_ForceReset(WalkingExtension* w)
  void WalkingExtension_KnockDown(WalkingExtension* w, const Vector2& dir)
  void WalkingExtension_Reset(WalkingExtension* w)
  void WalkingExtension_SetActive(WalkingExtension* w, bool a)
  void WalkingExtension_SetNoWalkFrame(WalkingExtension* w, int f)
  void WalkingExtension_SetWalkSpeed(WalkingExtension* w, float s)
  void WalkingExtension_StartWalking(WalkingExtension* w)
  void WalkingExtension_StopWalking(WalkingExtension* w)
  void World_AddBackgroundImage(const char* a, const Vector2& b, const Vector2& c, float d, const Colour& e, float f)
  void World_AddForegroundImage(const char* a, const Vector2& b, const Vector2& c, float d, const Colour& e, float f)
  void World_Alert(const Vector2& a, float b)
  void World_ChangeEmotion(unsigned long objHash, unsigned int emotion)
  void World_CheckGoal(Object* a)
  void World_CopyItem()
  void World_CreateEffect(const char* name, Vector2 pos)
  void World_CreateExplosion(const Vector2& pos, float radius)
  void World_CreateExplosionSpecial()
  void World_CreateLight(const Vector2& a, const Colour& b, float c, float d)
  void World_IncrementStat(const char* a)
  void World_MerchMerchPickUp()
  void World_MoveLight(unsigned short a, const Vector2& b)
  void World_PasteItem()
  void World_Pause()
  void World_RemoveLight(unsigned short a)
  void World_SaveLevel()
  void World_Scare(const Vector2& pos, float radius, int strength)
  void World_SelectAll()
  void World_SetFPS(int a)
  void World_SetGameSpeed(unsigned long speed)
  void World_SetGFXViewOffset(const Vector2& a, const Vector2& b)
  void World_SetGlobalAmbient(const Colour& a)
  void World_SetGravity(Vector2 g, int mode = 0)
  void World_SetLevelModulate(const Colour& a)
  void World_SetLightAttenuation(unsigned short a, float b, float c, float d)
  void World_SetMaximumSpeed(float s)
  void World_SetTextHint(const char* a, const Vector2& b)
  void World_SetWinFlag()
  void World_ShowClickables()
  void World_ShowGoalBox(const char* a, const Vector2& b, const char* c, const char* d, const char* e)
  void World_ShowGoalBoxFX(const char* a, const Vector2& b)
  void World_ShowPuzzleAsNew()
  void World_ShowSolutionTime(float a)
  void World_ShowTutorialDialog(const char* a)
  WalkingExtension* Object_GetWalkingExtension(Object* o)
```

## UI toolkit (`eets_ui.h`)
```cpp
  bool Button(const char* label)
  bool hit(int x, int y, int w, int h)
  bool hover(int x, int y, int w, int h)
  bool Toggle(const char* label, bool& value)
  Colour black()
  Colour btn()
  Colour btnHover()
  Colour cream()
  Colour label()
  Colour off()
  Colour on()
  Colour panel()
  Colour shadow()
  Colour textC()
  Colour textHover()
  Colour title()
  Colour titlebar()
  float Slider(const char* label, float& value, float lo, float hi)
  State& S()
  void Begin(int x, int y, int w, const char* title = nullptr)
  void End()
  void FeedMouse(int x, int y, int button, int down)
  void Label(const char* text)
```

## Loader services (`eetsmod.h`)
```cpp
  bool Hook(void* target, void* detour, void** original);
  const char* ConfigGet(const char* mod, const char* key, const char* def);
  const char* SaveGet(const char* mod, const char* key, const char* def);
  double DeltaTime();
  double Time();
  float ConfigGetFloat(const char* mod, const char* key, float def);
  float SaveGetFloat(const char* mod, const char* key, float def);
  int   ConfigGetInt(const char* mod, const char* key, int def);
  int MouseX();
  int MouseY();
  int RenderHeight();
  int RenderWidth();
  int   SaveGetInt(const char* mod, const char* key, int def);
  void Log(const char* fmt, ...);
  void  SaveSet(const char* mod, const char* key, const char* val);
  void  SaveSetFloat(const char* mod, const char* key, float v);
  void  SaveSetInt(const char* mod, const char* key, int v);
  void StartTextInput();
  void StopTextInput();
```

## Mod entry points

Define any subset as `extern "C"`; the loader resolves each by name:
```cpp
  EetsMod_Init
  EetsMod_OnEvent
  EetsMod_OnKey
  EetsMod_OnMouse
  EetsMod_OnText
  EetsMod_OnWheel
  EetsMod_Shutdown
  EetsMod_Update
```
