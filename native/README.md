# Eets native (C++) modding

Write a mod by dropping a single `.cpp` file in `<game>/mods/`. The loader
compiles it on launch, injects it, calls it every frame, and forwards input.
No Makefile, no build step — just edit and relaunch (or save while running for
hot-reload).

## How it works

Eets is a native, **non-PIE** C++ ELF that links SDL2 and FNA3D dynamically:

1. **Fixed addresses** — engine functions sit at stable absolute addresses, so a
   mod calls `World_SetGravity`, `World_CreateObject`, etc. directly
   (`include/eets_engine.h`).
2. **Interposable frame + input** — SDL2/FNA3D calls go through the PLT, so the
   preloaded `libeetsmod.so` interposes:
   - `FNA3D_SwapBuffers` → per-frame tick (compile + load mods on the first
     frame, `EetsMod_Update` after, poll sources for hot-reload),
   - `SDL_PollEvent` → keyboard events → `EetsMod_OnKey`.

```
Eets (native C++)
  └─ LD_PRELOAD libeetsmod.so
       ├─ compile mods/*.cpp → mods/.cache/*.so   (only when changed)
       ├─ dlopen + EetsMod_Init()
       ├─ FNA3D_SwapBuffers → EetsMod_Update() each frame
       ├─ SDL_PollEvent     → EetsMod_OnKey(key, mods, down)
       └─ main-menu banner: "Eets Mods vX.Y.Z, N mods loaded"
```

## Install (one time)

```sh
cd native
make                                   # builds build/libeetsmod.so
make install GAME=/path/to/Eets        # loader + headers + sample mods -> <game>
```

This installs the loader, the headers (to `<game>/eetsmod-include/`), and the
sample mods (`lowgrav.cpp`, `sandbox.cpp` + `sandbox.cfg`) into `<game>/mods/`.

## Run

```sh
EETS_DIR=/path/to/Eets ./run-eets.sh        # from the repo root
```

Or set the Steam launch option (Properties → Launch Options):

```
LD_PRELOAD=/full/path/to/Eets/libeetsmod.so %command%
```

The main menu shows `Eets Mods vX.Y.Z, N mods loaded` bottom-left.
`<game>/Log/native_mods.log` has compile output and load status.

## Writing a mod

Create `<game>/mods/mymod.cpp` — that's it. Relaunch (or just save it while the
game runs; it recompiles and reloads within ~0.5s).

```cpp
#include "eetsmod.h"
using namespace Eets;

extern "C" void EetsMod_Init() {
    Eets::Log("mymod: loaded");
}

extern "C" void EetsMod_OnKey(int key, int mods, int down) {
    if (down && key == 'g' && (mods & EKMOD_CTRL))
        World_SetGravity({0, World_GetGravity().y * 0.25f}, 0);
}
```

Entry points (all optional, `extern "C"`):

| Symbol | When |
|--------|------|
| `EetsMod_Init()` | once, on the first frame |
| `EetsMod_Update()` | every frame, before present |
| `EetsMod_OnKey(key, mods, down)` | each keyboard event (SDL keycode/modmask) |
| `EetsMod_Shutdown()` | before unload (hot-reload / exit) |

You never run the compiler yourself — but for reference the loader builds each
mod as `g++ -O2 -fPIC -std=c++17 -shared -I<headers> -o .cache/<name>.so <name>.cpp`.
Set `$CXX` to use a different compiler; set `$EETS_MODS` to use a different folder.

## Config

Drop `<game>/mods/<modname>.cfg` (simple `key = value` lines, `#` comments):

```ini
explosion_radius = 250
enable_speed = 1
```

Read it from the mod:

```cpp
float r = Eets::ConfigGetFloat("mymod", "explosion_radius", 200.0f);
int   n = Eets::ConfigGetInt("mymod", "enable_speed", 1);
const char* s = Eets::ConfigGet("mymod", "name", "default");
```

## Hot-reload

While the game runs, edit and save a `mods/*.cpp`; the loader recompiles and
reloads it within ~0.5s (calling `EetsMod_Shutdown` then `EetsMod_Init`). Newly
added `.cpp` files are picked up too. Compile errors are logged and the old
build keeps running.

## Engine API (include/eets_engine.h)

Provided by the loader: `Eets::Log(fmt, ...)`, `Eets::ConfigGet*`.

Direct engine calls (fixed addresses):

```
World_GetGravity() / World_SetGravity(Vector2, mode)
World_CreateObject(name, Vector2) -> Object*
World_GetEets() -> Object*          World_GetObjectByID(id) -> Object*
World_CreateExplosion(pos, radius)  World_CreateExplosionSpecial()
World_CreateEffect(name, pos)       World_Scare(pos, radius, strength)
World_SetGameSpeed(0|1|2)           World_ChangeEmotion(hash, emotion)
Sound_CreateSound(name, loop, vol, pos)
Object_GetPosition/GetVelocity/GetID/GetMotionModel(o)
Object_ApplyImpulse(o, v)           Object_EnablePhysics(o, bool)
MotionModel_PushMotion/PopMotion/GetCurrentMotionName(m)
ForEachObject([](Object* o){ ... }) // live object list
World_IsInMainMenu()  ScreenWidth()/ScreenHeight()  DrawText(x, y, text, Colour)
```

Samples: `mods/lowgrav.cpp` (CTRL+G low gravity), `mods/sandbox.cpp` (CTRL+1/2/3
speed, CTRL+E explosion, CTRL+O object count; config-driven).

## Adding more engine functions

The binary isn't stripped, so anything is callable. Find a symbol, add its
address to `eets_engine.h` (`namespace addr`) and a typed wrapper:

```sh
nm /path/to/Eets | c++filt | grep World_
./gen_engine_header.sh /path/to/Eets   # regenerate addresses after a game update
```

Prefer the `World_*` / `Object_*` binding statics (plain pointer / `Vector2` /
scalar args) over functions that take or return `std::string` or luabind
iterators by value, whose ABI is fragile.

## Layout

```
native/
  include/eetsmod.h        mod author API (entry points, config, key constants)
  include/eets_engine.h    engine bindings at fixed addresses (regenerable)
  loader/loader.cpp        libeetsmod.so (compile + inject + dispatch)
  mods/lowgrav.cpp         sample mods (plain .cpp, no Makefile)
  mods/sandbox.cpp .cfg
  gen_engine_header.sh     regenerate addresses for a new game build
  Makefile                 build/install the loader
```
