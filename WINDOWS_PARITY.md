# Windows parity — status & path to 100%

Goal: one platform-agnostic codebase; a mod author writes one `.cpp` and it runs on
Linux (x86-64 `LD_PRELOAD` `.so`) and Windows (32-bit `version.dll` under Proton/native).
Target binary: **Eets.exe, PE32 i386, ImageBase 0x400000, ASLR on** (preserved at
`~/eets-win-ref/`; Ghidra serving it on `http://127.0.0.1:8089`).

## ✅ Validated working on Windows (under Proton)
- **Injection:** `version.dll` proxy (forwards all 16 `version` exports to System32) +
  IAT-patch `FNA3D_SwapBuffers`/`SDL_PollEvent`/`FNA3D_SetViewport` + `DllMain`
  (spawns a worker thread — must NOT shell out/wait under the loader lock) + VEH crash-guard.
- **Loader:** loads native `.dll` mods, dispatches `Init`/`Update`/`OnKey`, `Eets::Log`/
  `Save`/`Config` cross the boundary via the import lib `build/libeetsmod.dll.a`.
- **Engine API:** free functions (gravity ✓), scalar member methods (GetFlipped/IsWalker/
  GetID/IsPhysicsEnabled ✓), struct-return members (GetFacing ✓), all via the right ABI.
- **Graphics ✓ (validated in-game):** GraphicsEngine_i + FillRect/DrawRect/DrawLine/sprites +
  sized & styled text (`EString` MSVC-string ABI shim). The **in-game MODS manager UI** renders + works.
- **Singletons ✓:** ObjectMgr/Simulator/StringPool/TextureManager/(I)GraphicsEngine resolve; ForEachObject
  walks 42 objs (ptr-size-correct vector). MotionModel_* + Localize + Misc_BindKey validated.
- **Engine event hooks ✓ (Phase 3):** 32-bit E9 inline-hook path; World_ChangeEmotion/World_CheckGoal hook +
  dispatch `goal_check`/`emotion_change` to mods, game stable. (6 more hook targets pending RVA recovery.)
- **Bundles ✓ (Phase 4):** dual-binary `.eetsmod` (one bundle carries `.so`+`.dll`); fully in-process untar +
  asset install (no tar/rm/cp/find shell-out, no inflate dep). Validated: lowgrav.eetsmod extracted + loaded.
- **Build/CI:** mingw link recipe (`--whole-archive libwinpthread`) + a `build-windows` CI job. README Windows section.

## Hard-won facts (don't relearn these)
- **Proton requires** Steam launch option `WINEDLLOVERRIDES="version=n,b" %command%` —
  otherwise Wine loads its builtin version.dll and ours never runs. (Native Windows: no option needed.)
- **Link fully static** (`-static` for the loader; mods: `-static-libgcc -static-libstdc++
  -Wl,-Bstatic,-lwinpthread,-Bdynamic -leetsmod`). A stray `libwinpthread-1.dll` import = DLL-load failure.
- **`-Wl,--kill-at`** so the proxy exports undecorated names SDL2 imports.
- A proxy DLL must forward **every** export of the DLL it replaces (Wine aborts on the first missing one).

## The 4 engine-call ABI patterns (Win32; all `#ifdef _WIN32` in eets_engine.h, `ECALL`=`__thiscall`)
1. **cdecl free function** (World_/Sound_/Misc_): cast as-is; struct-by-value params pass on the
   stack same as MSVC. `World_SetGravity(Vector2,int)` works unchanged.
2. **cdecl + struct return** (World_GetGravity): MSVC returns 8-byte structs via a hidden first
   pointer: `Vector2 v; ((void(*)(Vector2*))addr::X)(&v); return v;`
3. **`__thiscall` scalar/void method** (GetFlipped, ApplyImpulse, …): `((R(ECALL*)(This*,…))addr::X)(o,…)`
4. **`__thiscall` + struct return — two sub-cases, check the disasm per function:**
   - **sret** (GetVelocity/GetFacing — take a hidden `[EBP+8]` ptr): `Vector2 r; ((void(ECALL*)(This*,Vector2*))addr::X)(o,&r); return r;`
   - **pointer-return** (GetPosition — returns `Vector2*` in EAX): `return *((Vector2*(ECALL*)(This*))addr::X)(o);`

## Address recovery — what's proven
- **luabind free functions** (World registrar `FUN_004d9dc0`): struct `[ESI+8]=name,[ESI+0xc]=funcptr`
  → real functions. Done (~59), validated.
- **luabind class methods** (Object `FUN_004ed440`, extensions `FUN_004d38f0`): pattern is
  `MOV [EBP-N],funcptr` **before** the `PUSH name` (funcptr is PUSHed into the `def` call).
  ⚠️ The funcptr is the MOV **before** the name, not after — the original parse was **off by one**.
  Backward-paired parse → real methods (scalars validated). Recovery scripts left in `/tmp`
  (`methods_fixed.json`, `objreg2.asm`, `extreg2.asm`, `strings.txt`).
- **Runtime validation loop:** build a tiny `.dll` canary that calls the target on `World_GetEets()`,
  log before each call, drop in `<game>/mods/`, `CTRL+<key>` in a level, read `Log/native_mods.log`.

## ✅ Also done + validated since
- **Asset pipeline wired:** Texture_Load/UploadTexture, IGraphicsEngine_DrawTexture, all Animation_* (Animation is
  0x54 bytes on Win, ctor __thiscall), TextureManager_LoadTexture, AnimExt_LoadAnimation, Anim_Get/SetCurrentFrameIndex.
  Sprite load+draw runtime-validated (SpriteManager_Load is __thiscall+sret+MSVC-string, confirmed vs disasm).
- **All extension accessors fixed:** they are **cdecl luabind free fns** (Object* on the stack), not __thiscall - the
  wrappers used EC and crashed; switched the 11 to FC. Validated: walk/emotion/position/holding all resolve + read.
- **5 engine hooks live + validated** (CreateObject/LoadWin/Reset/Complete/KillMe). Detours carry ECALL (the targets
  are __thiscall members; cdecl detours corrupted the stack -> splash crash). emotion/goal already worked (cdecl).
- **Object_GetPosition** `0xaa0e0` is ABI-correct (Vector2* in EAX, identical to Linux). Its garbage-on-Eets is an
  engine weakness shared with the Linux build, not a port bug - no separate general getter exists.

## ⏳ Genuinely remaining (irreducible niche tail)
1. **Functions MSVC inlined/COMDAT-folded out of existence** (no standalone address in the Win binary). Reimplemented
   in the wrappers via raw struct access so the Eets:: API still matches: ThwackerExtension_IsThwacking (state@+0x30),
   EdibleExtension_GetEater (MSVC set walk), EmotionExtension_SetEmotionName (in-place EString), DrawCircleFilled
   (FillRect scanlines). **Two could not be faithfully faked:** ThwackerExtension_GetCentre (computed inline from the
   held object, no field) → returns {0,0}; HoldingExtension_ReleaseObject (single-erase semantics inlined; only bulk
   ReleaseAll survives) → no-op.
2. **hook_Creator_OnEndEetsDeadDialog** — the dead-dialog-end event; not recovered. (StartEetsDeadDialog is a generic
   name-based opener, so the eets_death hook is deferred; derive from object_killed of World_GetEets() meanwhile.)
3. **PhysicsExtension collision deque on Win32** — GetAccumulate/GetCollisions resolve, but ForEachCollision needs an
   MSVC-`std::deque` walker (`CollisionReport.unsigned long` is 4 bytes on Win vs 8 on Linux). Guarded to no-op.

## Branch
`windows-parity` (pushed). Publish-prep on `main`. Graphics+UI, singletons, 32-bit hooks, and dual-binary
`.eetsmod`/in-process untar all landed + validated under Proton; Linux build/examples green throughout.

## Bulk recovery pipeline (for the 55 internals)
Per-function MCP calls are too slow. Use the headless decompile dump:
```
/opt/ghidra/support/analyzeHeadless /tmp/ghproj eetsdump -import ~/eets-win-ref/Eets.exe \
    -postScript DumpDecomp.java -scriptPath tools/win        # ~12 min, -> /tmp/eets_decomp.c
```
`tools/win/DumpDecomp.java` exports all 7981 functions as `//FUNC <addr>` + decompiled C.
Parse to `{addr: body}` and grep. Validated: `004dbef0` = World_GetGravity (gravity getter).

Recovery leads found in the decomp:
- **Singletons** (`*_i`): tiny `return DAT_xxxx;` accessors — 39 candidates. e.g. `004f8600 -> DAT_00ee3cbc`
  (the singleton World_CreateObject uses). Map each DAT to its class via the constructor that writes
  it + sets the (RTTI-named) vtable. GraphicsEngine vtable @ `0x564668` (meta-ptr `0x564664`, COL `0x58b630`).
- **Rendering**: FNA3D imports are named (`FNA3D_DrawPrimitives` ×10 callers = the draw layer; the public
  DrawSquare/Line/Circle batch, so they don't call FNA3D directly — trace from the GraphicsEngine ctor's
  method cluster, ~`0x487-0x48e`).
- **Methods**: reachable from the known World wrappers (e.g. `World_CreateObject 0x4dbe60 -> FUN_004f6ff0`
  = the create path; `World_Alert/ShowGoalBox/ShowTutorial -> text`; `World_CreateLight -> 0x487* GE`).
- **Off-by-one** in the class registrars is fixed; `methods_fixed.json` in /tmp has the corrected pairs.

Remaining is the per-function identity-mapping RE on top of this dump + a runtime launch per batch.

## Graphics recovery — ✅ VALIDATED ON WINDOWS (runtime, in-game panel rendered)
- **GraphicsEngine_i = global `DAT_00aae51c` (VA 0xaae51c → RVA `0x6ae51c`)** holding GE*; deref on Win32.
  GE has **vtable `0x564668`** (ctor `FUN_0048b7f0` writes `GraphicsEngine::vftable`); **device@+0x20**
  (proven: `FNA3D_VerifySampler(DAT_00aae51c+0x20,...)`); accessor `FUN_00481c80` (`MOV EAX,[0xaae51c]`).
  ⚠️ The old keystone `0xae3db0` (`DAT_00ee3db0`) was the **SDL_Window** (`= SDL_CreateWindow(...)`), NOT the GE —
  draws on it silently no-op'd. Confirmed by in-process probe (GE vtableRVA must = `0x164668`).
- **Draw methods are vtable entries** of that GE (`__thiscall`, read device at `this+0x20`), wired + validated:
  - **DrawSquare `0x8e030`** (FNA3D prim 0, 2 tris = filled rect; `RET 0xc` = 3 args) — `FillRect`. R↔B color swizzle.
  - **DrawLine `0x8d880`** (FNA3D prim 2; **`RET 0x10` = 4 args incl. `width`** — must pass width or stack corrupts/crashes).
  - **DrawSprite `0x8dbd0`** (prim 0). Sprite deps: `SpriteManager_i 0x89ff0`, `SpriteManager_Load 0x896c0`,
    `Sprite_GetWidth 0x88170`, `Sprite_GetHeight 0x88130`, `Sprite_GetDiffuseUV 0x88100` (sprite path not runtime-validated yet).
  - **printText `0xb9b30`** (cdecl `(int x,int y,char const*,Color const&)`) — builds the MSVC std::string internally.
  - **TextPrinter_DrawString `0x83a40`** (sized/styled text) — takes an MSVC `std::string` **by const-ref** (reads only,
    caller owns). mingw `std::string` is NOT layout-compatible, so the shared header uses **`EString`** (compile-time
    polymorphism: `std::string` on Linux, a struct mirroring MSVC's layout — buf[16]\@0, size\@0x10, cap\@0x14, SSO≤0xf —
    on Win32). One call site, ABI absorbed by the type. ✅ Validated: all FontSizes + styles + the heap `_Ptr` (>15 char) path.
  - **DrawCircleFilled: ABSENT** in the Win build (full GE vtable enumerated; Eets draws circles as whitecircle sprites). FillCircle guards on 0.
- **Build-recipe fix (all Win mods using std::string/static-locals):** the documented `-Wl,-Bstatic,-lwinpthread,-Bdynamic`
  LEAKS a `libwinpthread-1.dll` import (load-fails: "Module not found"). Use
  `-Wl,-Bstatic,--whole-archive,-l:libwinpthread.a,--no-whole-archive,-Bdynamic` to force-include the static pthread objects.
- GE ctor (FNA3D device wrapper) `0x48e670`; fullscreen-fade `0x48eb00`. Vtable order: slot4=DrawLine(0x8d880), slot5=DrawSquare(0x8e030).
