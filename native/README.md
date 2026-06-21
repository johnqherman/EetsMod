# Eets native (C++) modding

Write a mod by dropping a single `.cpp` file in `<game>/mods/`. The loader
compiles it on launch, injects it, calls it every frame, forwards input/events,
and isolates crashes. Mods call engine functions directly.

## How it works

Eets is a native, **non-PIE** C++ ELF that links SDL2/FNA3D dynamically. The
preloaded `libeetsmod.so` interposes their PLT calls:
- `FNA3D_SwapBuffers` → per-frame tick (compile+load mods on first frame; run
  `EetsMod_Update`; poll for hot-reload; capture the render viewport; draw the
  banner / F1 manager),
- `SDL_PollEvent` → keyboard + mouse → mod callbacks,
- `FNA3D_SetViewport` → the true render size (correct in fullscreen).

Engine functions sit at fixed addresses (`eets_engine.h` / `eets_addr.h`); the
detour engine (`hook.h`) can hook arbitrary engine functions.

## Install & run

```sh
cd native
make
make install GAME=/path/to/Eets        # loader + headers + sample mods
EETS_DIR=/path/to/Eets ../run-eets.sh   # or Steam launch option (below)
```

Steam: Properties → Launch Options → `LD_PRELOAD=/full/path/Eets/libeetsmod.so %command%`
(`tools/steam-launch-option.sh` prints it). Mods are `.cpp` compiled on launch, so
`g++` must be on PATH; Steam's runtime may lack it — run once from a terminal to
cache builds, or ship prebuilt `.so`. Logs: `<game>/Log/native_mods.log`.
`F1` = interactive mod manager (click rows to enable/disable; persists).

## Writing a mod

`tools/new-mod.sh <name>` scaffolds one, or create `<game>/mods/mymod.cpp`:

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
`Eets::DrawImage(path,x,y)` (custom images), the F1 manager edits each mod's
config live (`-`/`+`).

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
comment). Regenerate addresses after a game update: `gen_engine_header.sh`.

## Robustness

- **Crash isolation** — a faulting mod callback is caught and the mod disabled;
  the game keeps running.
- **Hot-reload** — edit + save a `.cpp` while running; recompiles & reloads.
- **BuildID guard** — warns if the running binary differs from the bound build.
- **Fullscreen-correct** — HUD + mouse use the real render viewport.

## Build / release / CI

`make release` → `dist/eetsmod-<ver>.tar.gz` (loader + headers + sample mods +
`install.sh`). CI (`.github/workflows/ci.yml`) builds the loader + syntax-checks
the sample mods. Windows port plan: `docs/WINDOWS.md`.

## Layout
```
native/
  include/  eetsmod.h  eets_engine.h  eets_addr.h  eets_ui.h
  loader/   loader.cpp  hook.h
  mods/     lowgrav.cpp  sandbox.cpp(.cfg)  spawner.cpp  menudemo.cpp
  gen_engine_header.sh   Makefile
```
