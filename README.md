[![build](https://github.com/johnqherman/eets-mod-framework/actions/workflows/ci.yml/badge.svg)](https://github.com/johnqherman/eets-mod-framework/actions/workflows/ci.yml)

<p align="center">
  <img width="676" height="322" alt="eetsmod" src="https://github.com/user-attachments/assets/447a13fa-5127-4ac8-9c50-68edebce5798" />
</p>

A modding framework for **Eets** (the Klei puzzle game).
Mods are native plugins (`.dll` on Windows, `.so` on Linux), injected by a small
loader, packaged as one self-contained `.eetsmod` file, and managed from inside the
game.

## Play with mods

A mod is one **`.eetsmod`** file with everything inside (code, assets, config, etc).

Turning on mod support takes three steps. The first two differ by platform; the third
is the same everywhere.

1. **Open the game folder.** In Steam, right click **Eets > Manage > Browse local
   files**, then drop the loader (from a [release](../../releases)) into it:
   - **Windows:** `version.dll` (a proxy the game loads on startup)
   - **Linux:** `libeetsmod.so`
2. **Set the launch option** (Steam > right click **Eets > Properties > General >
   Launch Options**), pasted exactly:
   - **Windows:** none needed; Windows loads `version.dll` from the game folder
     automatically.
   - **Linux:**
     ```
     LD_PRELOAD=./libeetsmod.so %command%
     ```
3. **Add mods.** Put `.eetsmod` files in the `mods` folder. Press **Play**.

In-game: on the main menu, click the **MODS** button (bottom left) to enable/disable
mods, change their settings, or open the mods folder.

> **Heads up:** Mods are native code that runs as part of the game (no sandbox).
> Only install mods from people you trust.

## Make mods

A mod is one C++ source file that hooks engine events. This walkthrough builds a mod
that cuts gravity on **Ctrl+G**, start to finish.

### Before you start

The `eetsmod` CLI used below lives in this repo at `bin/eetsmod`. Clone the repo and
put it on your `PATH`:

```sh
git clone https://github.com/johnqherman/eets-mod-framework
export PATH="$PWD/eets-mod-framework/bin:$PATH"   # so `eetsmod` runs from anywhere
```

You also need a C++ compiler (`g++`). To build the Windows `.dll` as well, install
MinGW (`i686-w64-mingw32-g++`); without it `pack` still works but the bundle is
Linux-only.

### 1. Scaffold

```sh
eetsmod new mymod
```

This writes three files:

- **`mymod.cpp`**: your code, pre-filled with empty event hooks.
- **`mymod.cfg`**: the manifest (version, author, dependencies; all optional).
- **`compile_flags.txt`**: include path so clangd/your editor resolves the API.

### 2. Write hooks

Implement the engine callbacks you care about; leave the rest out. The main ones:

| Hook | Fires |
|------|-------|
| `EetsMod_Init()` | once at load |
| `EetsMod_Update()` | every frame |
| `EetsMod_OnKey(key, mods, down)` | key press/release |
| `EetsMod_OnEvent(name, a, b)` | game events: `object_spawn`, `object_killed`, `level_load`, `level_reset`, `level_complete` |

The gravity mod needs only one:

```cpp
#include "eetsmod.h"
using namespace Eets;

extern "C" void EetsMod_OnKey(int key, int mods, int down) {
    if (down && key == EKEY_g && (mods & EKMOD_CTRL))
        World_SetGravity({0, World_GetGravity().y * 0.25f}, 0);   // Ctrl+G: low gravity
}
```

The full engine API is in [`API.md`](API.md) and the `include/` headers.

### 3. Add assets (optional)

Custom images, sounds, and anims live in a sibling **`mymod.assets/`** folder whose
layout **mirrors the game's `Data/` tree** (e.g. `mymod.assets/Sound/Music/song.ogg`).
Drop files there and the next step bundles them automatically.

Sounds are the one exception: the engine needs its own patch format, not a raw audio
file. Supply an **`.ogg`** and convert it first:

```sh
eetsmod add-sound mymod song.ogg           # writes the engine-format files into mymod.assets/
```

Then play it by name from code: `PlaySound("song")`.

### 4. Pack

```sh
eetsmod pack mymod.cpp -o mymod.eetsmod
```

`pack` does everything in one shot: compiles the `.dll` (Windows) and `.so` (Linux),
then bundles them with the source, the manifest, and `mymod.assets/` into a single
`mymod.eetsmod` file.

### 5. Install and run

Drop `mymod.eetsmod` in `<game>/mods`, then launch
and toggle it from the in-game **MODS** button. On launch the loader unpacks the bundle
and overlays its assets onto the game's `Data/` *before* the engine reads them, so
custom assets load as if they shipped with the game. Build logs land in
`<game>/Log/native_mods.log`.

> **Tip:** while developing you can skip packing: drop `mymod.cpp` straight into
> `mods/` and the loader compiles it on launch.

Learn by example in [`examples/`](examples): gravity, custom images/sounds/anims, the UI
toolkit, object extensions, collisions.

## Layout

```
bin/eetsmod    build/pack/manage CLI
loader/        the injected loader (hooks, crash isolation, .eetsmod staging)
include/       mod headers: engine API, addresses, UI toolkit
examples/      example mods (source + <name>.assets/)
tools/         check-mod, add-sound, gen-api-ref, new-mod
API.md         generated engine-API reference
Makefile       build / check / bundles / apidoc / release
```

## License

MIT.

Not affiliated with or endorsed by Klei Entertainment; *Eets* and its assets belong to
their owners. This framework ships no game code, only original tooling and addresses
derived from our own copy of the game. Use at your own risk.
