# Eets Mod Framework

[![build](https://github.com/johnqherman/eets-mod-framework/actions/workflows/ci.yml/badge.svg)](https://github.com/johnqherman/eets-mod-framework/actions/workflows/ci.yml)

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

> **Heads up:** mods are native code that runs as part of the game (no sandbox).
> Only install mods from people you trust.

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

`eetsmod new` scaffolds a mod; `eetsmod pack` bundles it; the in-game **MODS** button
manages it. The engine API is in [`API.md`](API.md) and the
`include/` headers; learn by example in [`examples/`](examples) (gravity, custom
images/sounds/anims, the UI toolkit, object extensions, collisions).

## How it works

Eets is a non-PIE C++ ELF that links SDL2/FNA3D dynamically, so a preloaded loader
interposes `FNA3D_SwapBuffers` (per-frame) and `SDL_PollEvent` (input), `dlopen`s mod
`.so`s, and calls engine functions at their fixed addresses. Everything is derived by
reverse-engineering the binary (non-PIE, so addresses are stable per build). The Windows
build is supported too (see below); one mod `.cpp` builds for both platforms.

## Windows / Proton

The Windows build of Eets.exe (PE32 i386) is supported. The loader ships as a `version.dll`
proxy that injects into the game, IAT-patches the same SDL2/FNA3D entry points, and loads
native `.dll` mods - same engine API, same mod source. A mod author writes one `.cpp`;
`eetsmod pack` cross-builds both the Linux `.so` and the Windows `.dll` into one `.eetsmod`.

- Build the loader: `make win` -> `build/version.dll`.
- Install: drop `version.dll` in the game folder and your mod `.dll` (or `.eetsmod`) in `mods/`.
- Under **Proton**, set the Steam launch option `WINEDLLOVERRIDES="version=n,b" %command%`
  so Wine loads our `version.dll` instead of its builtin. (Native Windows needs no option.)
- Build a mod `.dll` directly (the link recipe avoids a fatal `libwinpthread-1.dll` import):
  ```
  i686-w64-mingw32-g++ -O2 -std=c++17 -Iinclude -shared -static-libgcc -static-libstdc++ \
    -o mymod.dll mymod.cpp -L build -leetsmod \
    -Wl,-Bstatic,--whole-archive,-l:libwinpthread.a,--no-whole-archive,-Bdynamic
  ```

Working on Windows: the in-game MODS manager UI, the engine API (draw/text/sprites,
singletons, object + extension methods), engine event hooks (32-bit inline hooks), and
in-process `.eetsmod` extraction. See `WINDOWS_PARITY.md` for the full status.

## Layout

```
bin/eetsmod    build/pack/manage CLI
loader/        the LD_PRELOAD loader (hooks, crash isolation, .eetsmod staging)
include/       mod headers: engine API, addresses, UI toolkit
examples/      example mods (source + <name>.assets/)
tools/         check-mod, add-sound, gen-api-ref, new-mod
API.md         generated engine-API reference
Makefile       build / check / bundles / apidoc / release
```

## License

MIT.

Not affiliated with or endorsed by Klei Entertainment; *Eets* and its assets belong to
their owners. This framework ships no game code - only original tooling and addresses
derived from your own copy of the game. Use at your own risk.
