# Eets native (C++) modding

Write mods in C++ as shared objects. A small preloaded loader injects them into
the native engine, calls them every frame, and forwards input — and your mod
calls engine functions directly.

## How it works

Eets is a native, **non-PIE** C++ ELF that links SDL2 and FNA3D dynamically.
Two facts make native modding clean:

1. **Fixed addresses.** Non-PIE means every engine function sits at a stable
   absolute address, so a mod can call `World_SetGravity`, `World_CreateObject`,
   etc. directly (see `include/eets_engine.h`).
2. **Interposable frame + input.** Calls into SDL2/FNA3D go through the PLT, so an
   `LD_PRELOAD` library can interpose them (the engine's *internal*, statically
   linked Lua calls cannot be). The loader hooks:
   - `FNA3D_SwapBuffers` → once-per-frame tick (loads mods on the first frame,
     calls `EetsMod_Update` after),
   - `SDL_PollEvent` → keyboard events dispatched to `EetsMod_OnKey`.

```
Eets (native C++)
  └─ LD_PRELOAD libeetsmod.so
       ├─ interpose FNA3D_SwapBuffers  → EetsMod_Update() each frame
       ├─ interpose SDL_PollEvent      → EetsMod_OnKey(key, mods, down)
       ├─ dlopen mods/*.so (first frame) → EetsMod_Init()
       └─ mods call engine funcs directly by fixed address
```

## Build & install

```sh
make                                   # -> build/libeetsmod.so + build/mods/*.so
make install GAME=/path/to/Eets        # copy loader to <GAME>/, mods to <GAME>/mods/
```

Requires a C++17 compiler. Addresses target one game build; if Eets updates,
regenerate them: `./gen_engine_header.sh /path/to/Eets`.

## Run

```sh
EETS_DIR=/path/to/Eets ../run-eets.sh      # wrapper (sets LD_PRELOAD)
```

Or set the Steam launch option (Properties → Launch Options):

```
LD_PRELOAD=/full/path/to/Eets/libeetsmod.so %command%
```

Verify: `<game>/Log/native_mods.log` should list the loaded mods. The sample
`lowgrav` toggles low gravity with **CTRL+G** in a level.

## Writing a mod

`mymod.cpp`:

```cpp
#include "eetsmod.h"
using namespace Eets;

extern "C" void EetsMod_Init() {
    Eets::Log("mymod: loaded");
}

extern "C" void EetsMod_Update() {
    // runs every frame
}

extern "C" void EetsMod_OnKey(int key, int mods, int down) {
    if (down && key == 'b' && (mods & EKMOD_CTRL)) {
        Object* eets = World_GetEets();
        // ... call engine functions directly
    }
}
```

Build it as a shared object against `include/` and drop the `.so` in `<game>/mods/`:

```sh
g++ -O2 -fPIC -std=c++17 -Iinclude -shared -o mymod.so mymod.cpp
```

Entry points (all optional, `extern "C"`):

| Symbol | When |
|--------|------|
| `EetsMod_Init()` | once, on the first rendered frame |
| `EetsMod_Update()` | every frame, before present |
| `EetsMod_OnKey(key, mods, down)` | each keyboard event (SDL keycode/modmask) |

Engine API: see `include/eets_engine.h` (`World_*`, `Object_*`, `Vector2`).
`Eets::Log(fmt, ...)` is provided by the loader.

## Layout

```
native/
  include/eetsmod.h        mod author API (entry points, key constants)
  include/eets_engine.h    engine bindings at fixed addresses (regenerable)
  loader/loader.cpp        libeetsmod.so (LD_PRELOAD interposer)
  mods/lowgrav/            sample mod
  gen_engine_header.sh     regenerate addresses for a new game build
  Makefile                 build + install
```

## Adding engine functions

Anything in the binary is callable — it's not stripped. Find the symbol and add
a typed wrapper:

```sh
nm /path/to/Eets | c++filt | grep World_
```

Add the address to `eets_engine.h` (`namespace addr`) and an inline wrapper with
the matching signature. Functions that take/return `std::string` or luabind
iterators by value have a fragile ABI — prefer the `World_*`/`Object_*` binding
statics, which use plain pointer/`Vector2`/scalar arguments.
