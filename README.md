# Eets Mod Framework

A modding framework for **Eets** (the Klei puzzle game), a native C++ engine.
Mods are native `.so` plugins, injected by a small `LD_PRELOAD` loader, packaged as
one self-contained `.eetsmod` file - and managed from inside the game.

## Play with mods

A mod is one file: **`mymod.eetsmod`** (like a Minecraft `.jar`). It has everything
inside - code, settings, art.

1. **Turn on mod support once.** In the repo or a release, run `eetsmod setup`, then
   paste the line it prints into the Eets **Steam launch options** (right-click Eets
   in your library -> Properties -> General -> Launch Options):
   ```
   LD_PRELOAD=/path/to/Eets/libeetsmod.so %command%
   ```
2. **Add a mod.** Drop the `.eetsmod` file into the game's `mods` folder. That's it -
   no compiler, no extra tools.
3. **Play.** On the main menu, click the **MODS** button (bottom-left) to turn mods
   on/off, change their settings, or open the mods folder to add more. (`F1` also
   opens it.)

> **Heads up:** mods are native code that runs as part of the game. Only install
> mods from people you trust. See [`SECURITY.md`](SECURITY.md).

## Make mods

```sh
eetsmod new mymod                    # scaffold mymod.cpp + mymod.cfg
# ...write your mod...
eetsmod add-sound mymod ~/sounds/    # (optional) bundle custom sounds
eetsmod pack mymod.cpp -o mymod.eetsmod   # one-file bundle: .so + source + manifest + assets
```

```cpp
#include "eetsmod.h"
using namespace Eets;
extern "C" void EetsMod_OnKey(int key, int mods, int down) {
    if (down && key == EKEY_g && (mods & EKMOD_CTRL))
        World_SetGravity({0, World_GetGravity().y * 0.25f}, 0);   // CTRL+G: low gravity
}
```

Full guide, the engine API, custom images/sounds/anims, the in-game UI toolkit, and
build details: **[`native/README.md`](native/README.md)**. API reference:
[`docs/NATIVE_API.md`](docs/NATIVE_API.md). Example mods: [`native/examples/`](native/examples).

## How it works

Eets is a non-PIE C++ ELF that links SDL2/FNA3D dynamically, so a preloaded loader
interposes `FNA3D_SwapBuffers` (per-frame) and `SDL_PollEvent` (input), `dlopen`s mod
`.so`s, and calls engine functions at their fixed addresses. Everything is derived by
reverse-engineering the binary; see [`docs/INTERNALS.md`](docs/INTERNALS.md).

## Layout

```
bin/eetsmod          setup + build/pack/manage CLI
native/
  loader/            the LD_PRELOAD loader (hooks, crash isolation, .eetsmod staging)
  include/           mod headers: engine API, addresses, UI toolkit
  examples/          example mods (source + <name>.assets/)
tools/               check-mod, add-sound, gen-api-ref, decompile helpers
docs/                INTERNALS, NATIVE_API, WINDOWS (port plan), decompiled/ (RE dump)
```

## License

MIT.
