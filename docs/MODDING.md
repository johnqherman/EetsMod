# Writing Eets mods (C++)

A mod is C++ that calls engine functions directly. While developing, drop a loose
`.cpp` in `<game>/mods/` - the loader compiles it on launch, hot-reloads on save,
runs it every frame, forwards input/events, and isolates crashes. To ship, `pack` it
into a one-file `.eetsmod` (prebuilt `.so` + manifest + assets) - see
[Dev workflow](#dev-workflow).

## How it works

Eets is a native, **non-PIE** C++ ELF that links SDL2/FNA3D dynamically. The
preloaded `libeetsmod.so` interposes their PLT calls:
- `FNA3D_SwapBuffers` -> per-frame tick (compile+load mods on first frame; run
  `EetsMod_Update`; poll for hot-reload; capture the render viewport; draw the
  MODS button + manager overlay),
- `SDL_PollEvent` -> keyboard + mouse -> mod callbacks,
- `FNA3D_SetViewport` -> the true render size (correct in fullscreen).

Engine functions sit at fixed addresses (`eets_engine.h` / `eets_addr.h`); the
detour engine (`hook.h`) can hook arbitrary engine functions.

## Install & run

```sh
make                                    # build the loader
make install GAME=/path/to/Eets        # loader + headers (or: bin/eetsmod setup)
```

`eetsmod setup` prints the Steam launch option to set once:
`LD_PRELOAD=/full/path/Eets/libeetsmod.so %command%`. Players install a prebuilt
`.eetsmod` and need no compiler; `g++` is only needed to *build* a mod (loose `.cpp`
mods are also compiled on launch, so `g++` must be on PATH then). Logs:
`<game>/Log/native_mods.log`.
In-game manager: click the **MODS** button (bottom-left of the main menu) or press
`F1` - enable/disable mods, edit config live (`-`/`+`), open the mods folder.

## Writing a mod

`eetsmod new <name>` scaffolds one, or create `mymod.cpp`:

```cpp
#include "eetsmod.h"
// #include "eets_ui.h"   // for custom menus
using namespace Eets;

extern "C" void EetsMod_Init() { Eets::Log("mymod loaded"); }
extern "C" void EetsMod_OnKey(int key, int mods, int down) {
    if (down && key == 'g' && (mods & EKMOD_CTRL))
        World_SetGravity({0, World_GetGravity().y * 0.25f}, 0);
}
extern "C" void EetsMod_OnEvent(const char* ev, void* a, void* b) {
    if (!strcmp(ev, "object_spawn")) { /* a = Object*, b = name */ }
}
```

Entry points (all optional, `extern "C"`):

| Symbol | When |
|--------|------|
| `EetsMod_Init()` | once, first frame |
| `EetsMod_Update()` | every frame |
| `EetsMod_OnKey(key,mods,down)` | keyboard (SDL keycode/modmask) |
| `EetsMod_OnMouse(x,y,button,down)` | mouse (render-space coords) |
| `EetsMod_OnWheel(dx,dy)` | scroll |
| `EetsMod_OnEvent(name,a,b)` | engine events (below) |
| `EetsMod_OnText(utf8)` | typed text (call `Eets::StartTextInput()` first) |
| `EetsMod_Shutdown()` | before unload / hot-reload |

Also: `Eets::Time()`/`DeltaTime()` (frame timing), `Eets::PlaySound(name)`,
`Eets::DrawImage(path,x,y)` (custom images), `Eets::DrawAnim(path,x,y,dt)`
(animated `.anim`, native rate), `Eets::DrawImageHUD` (screen-locked). The in-game
manager (MODS button / F1) edits each mod's config live (`-`/`+`). Key codes are
named: `EKEY_F1`, `EKEY_UP`, `EKEY_SPACE`, `'g'`, etc. (see `eetsmod.h`).

## Dev workflow

```sh
bin/eetsmod new foo                  # scaffold foo.cpp + foo.cfg + compile_flags.txt
bin/eetsmod build foo.cpp            # compile to a redistributable foo.so
bin/eetsmod pack foo.cpp -o foo.eetsmod   # one-file bundle: .so + .cpp + manifest (+ assets/)
tools/check-mod.sh foo.cpp           # compile exactly like the loader, errors in the terminal
```

- **One-file mods.** A `.eetsmod` (gzipped tar of `<name>.so` + `<name>.cpp` +
  `<name>.cfg` + optional `assets/`) is how mods are shipped.
  Players drop it in `<game>/mods`; the loader unpacks it into a hidden staging dir
  (so `mods/` only ever holds the `.eetsmod`) and runs it. `make bundles` packs the
  examples this way into `examples/build/`.
- **Custom assets travel with the mod.** Put files in a `<name>.assets/` dir next to
  your `.cpp`, laid out as they sit under the game's `Data/` (e.g.
  `soundtest.assets/Sound/Wav/pluh.snd` + `Sound/Patch/pluh.ptch`). `pack` folds them
  into the bundle's `assets/` (and prints the count), and the loader installs them into
  `Data/` on extract - no separate step on the player's machine. See `examples/soundtest`.
  For sounds, one command builds the right files (ogg `.snd` + sample-rate-matched
  `.ptch`) straight into your mod:
  ```sh
  eetsmod add-sound mymod pluh ~/pluh.ogg     # -> mymod.assets/Sound/{Wav,Patch}/
  eetsmod add-sound --music mymod theme ~/song.ogg
  ```
  `pack` does not parse your code - it bundles whatever is in `<name>.assets/`, so if a
  `PlaySound("x")` goes silent, check that `x` is in there (`pack` prints `assets=N`).
- **`compile_flags.txt`** is written next to a scaffolded mod (`eetsmod new`), so
  clangd / VS Code / any LSP editor resolves `eetsmod.h` and gives autocomplete +
  inline errors against the whole API. (It is *not* installed into a player's game.)
- **Hot-reload feedback** - save a `.cpp` while the game runs; the loader recompiles
  and shows an on-screen toast (`reloaded foo`, or the first compiler error in red).
- **Debug builds** - `EETSMOD_DEBUG=1` (env, read by both the loader and
  `check-mod.sh`) compiles mods with `-g -O0` so you can attach gdb
  (`gdb -p $(pidof Eets)`). `EETSMOD_CXXFLAGS=...` appends extra flags.
- **Full API list:** [`docs/NATIVE_API.md`](NATIVE_API.md) (generated by
  `make apidoc`).

## Engine events

Fired to `EetsMod_OnEvent`: `object_spawn` (a=Object*, b=name), `object_killed`,
`level_load`, `level_reset`, `level_complete`, `emotion_change` (a=hash, b=emotion),
`goal_check` (a=Object*). Eets-death = `object_killed` of `World_GetEets()`. The
inline hooker relocates rel32 call/jmp/jcc and puts the trampoline near the target,
so it hooks short forwarder functions too.

## Config, manifest & save data

`<game>/mods/<mod>.cfg` (`key = value`, `#` comments) is both config and manifest:

```ini
version = 1.0
author = you
priority = 0          # higher loads first
requires = othermod   # comma-separated deps (missing dep disables the mod)
sim = 1               # affects simulation -> replay/leaderboard integrity warning
min_framework = 0.8.0 # disabled if the framework is older
my_setting = 42       # your own keys -> Eets::ConfigGet*
```

```cpp
int n = Eets::ConfigGetInt("mymod", "my_setting", 0);
Eets::SaveSetInt("mymod", "highscore", 100);   // persists in mods/mymod.save
int hs = Eets::SaveGetInt("mymod", "highscore", 0);
```

## Hooks (detour any engine function)

```cpp
static void (*orig)(void*, Vector2, int) = nullptr;
static void detour(void* self, Vector2 g, int m) { /* ... */ orig(self, g, m); }
Eets::Hook((void*)0x5bc6d0, (void*)detour, (void**)&orig);  // World_SetGravity
```
The decoder refuses (returns false) rather than corrupt a prologue it can't
relocate safely.

## Custom menus (`eets_ui.h`)

Immediate-mode, Eets-styled (red panels, black outlines, geek font):

```cpp
#include "eets_ui.h"
extern "C" void EetsMod_OnMouse(int x,int y,int b,int d){ Eets::UI::FeedMouse(x,y,b,d); }
extern "C" void EetsMod_Update() {
    Eets::UI::Begin(40, 80, 240, "My Menu");
    if (Eets::UI::Button("Spawn whale")) Eets::World_CreateObject("whale", {500,300});
    static bool on=false; Eets::UI::Toggle("Option", on);
    Eets::UI::End();
}
```

## Engine API

`eets_addr.h` has the **complete address table** for all 76 Lua-binding statics
plus curated methods/UI; `eets_engine.h` adds typed wrappers and 2D primitives
(`FillRect`/`DrawRect`/`DrawLine`/`FillCircle`, `DrawText`/`DrawTextOutlined`,
`World_*`, `Object_*`, `MotionModel_*`, `ForEachObject`, `RenderWidth/Height`).
For any binding without a wrapper, cast `addr::X` to its signature (shown in the
comment). Regenerate addresses after a game update: `tools/gen_engine_header.sh`. The
full signature list lives in [`docs/NATIVE_API.md`](NATIVE_API.md)
(`make apidoc`).

**Object extensions** - the behaviour components Eets and objects carry are
reachable from native, same surface as Lua. `Object_Get<Type>Extension(o)` returns
a typed pointer or null; the methods take that pointer:

```cpp
if (WalkingExtension* w = Object_GetWalkingExtension(World_GetEets())) {
    if (WalkingExtension_GetState(w) == WES_Falling) WalkingExtension_SetWalkSpeed(w, 0);
}
ForEachHeld(Object_GetHoldingExtension(o), [](Object* held){ /* ... */ });
ForEachCollision(o, [](const CollisionReport& r){ /* r.id_2, r.relative_velocity */ });
```

Covered types: Walking, Thwacker, Edible, Lighting, Suckable, Position, Rolling,
Holding, Flying, Emotion, EmotionPlatform, Physics. `ForEachCollision` walks the
physics report deque (enabling accumulation, matching Lua `GetCollisionReports`).
See `examples/extdemo.cpp`.

## Robustness

- **Crash isolation** - a faulting mod callback is caught and the mod disabled;
  the game keeps running.
- **Hot-reload** - edit + save a `.cpp` while running; recompiles & reloads, with
  an on-screen toast on success or the first compiler error on failure.
- **BuildID guard** - warns if the running binary differs from the bound build.
- **Fullscreen-correct** - HUD + mouse use the real render viewport.

## Build / release / CI

`make release` -> `dist/eetsmod-<ver>.tar.gz` (loader + headers + sample mods +
`install.sh`). `make check` compiles every example like the loader; `make apidoc`
regenerates `docs/NATIVE_API.md`. CI builds the loader, runs `check-mod.sh` on the
samples, and fails if the API doc is stale. Windows port plan: `docs/WINDOWS.md`.

## Layout
```
include/   eetsmod.h  eets_engine.h  eets_addr.h  eets_ui.h
loader/    loader.cpp  hook.h
examples/  lowgrav.cpp sandbox.cpp(.cfg) spawner.cpp soundtest(.assets/) ...
tools/     gen_engine_header.sh  check-mod.sh  gen-api-ref.sh  new-mod.sh  add-sound.sh
Makefile   build / check / bundles / apidoc / release
```
