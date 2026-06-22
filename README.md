# Eets Mod Framework

A modding framework for **Eets** (the Klei puzzle game), a native C++ engine.
Mods are native `.so` plugins, injected by a small `LD_PRELOAD` loader, packaged as
one self-contained `.eetsmod` file - and managed from inside the game.

## Play with mods

A mod is one file: **`mymod.eetsmod`**, with everything inside - code, settings, art.
Turning on mod support is three steps:

1. **Open the game folder.** In Steam, right-click **Eets -> Manage -> Browse local
   files**. Drop **`libeetsmod.so`** (from a [release](../../releases)) into it.
2. **Set the launch option.** Steam -> right-click **Eets -> Properties -> General ->
   Launch Options**, paste exactly:
   ```
   LD_PRELOAD=./libeetsmod.so %command%
   ```
3. **Add mods.** Put `.eetsmod` files in the `mods` folder (next to `libeetsmod.so`;
   it appears after the first launch, or just make it). Press **Play**.

In-game: on the main menu, click the **MODS** button (bottom-left) to enable/disable
mods, change their settings, or open the mods folder. (`F1` also opens it.)

> Prefer a command? `eetsmod setup` does step 1 + prints step 2 for you.

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
build details: **[`docs/MODDING.md`](docs/MODDING.md)**. API reference:
[`docs/NATIVE_API.md`](docs/NATIVE_API.md). Example mods: [`examples/`](examples).

## How it works

Eets is a non-PIE C++ ELF that links SDL2/FNA3D dynamically, so a preloaded loader
interposes `FNA3D_SwapBuffers` (per-frame) and `SDL_PollEvent` (input), `dlopen`s mod
`.so`s, and calls engine functions at their fixed addresses. Everything is derived by
reverse-engineering the binary; see [`docs/INTERNALS.md`](docs/INTERNALS.md).

## Layout

```
bin/eetsmod    setup + build/pack/manage CLI
loader/        the LD_PRELOAD loader (hooks, crash isolation, .eetsmod staging)
include/       mod headers: engine API, addresses, UI toolkit
examples/      example mods (source + <name>.assets/)
tools/         check-mod, add-sound, gen-api-ref, new-mod
docs/          MODDING guide, NATIVE_API, INTERNALS, WINDOWS (port plan)
Makefile       build / check / bundles / apidoc / release
```

## License

MIT.
