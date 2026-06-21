# Eets Mod Framework

A modding framework for **Eets** (the Klei puzzle game), a native C++ engine.
Two ways to write mods:

- **Native C++ mods** — `.so` plugins injected via a small `LD_PRELOAD` loader;
  call engine functions directly, get a per-frame tick and input. Full power.
  **See [`native/README.md`](native/README.md).**
- **Lua mods** — zero-patch, no preload: the game `dofile`s an optional
  `Data/localexec.lua` at boot. Lighter, scripting-only. Documented below.

Both come from reverse-engineering the engine; see [`docs/INTERNALS.md`](docs/INTERNALS.md).

---

## Native C++ mods (recommended)

Eets is a non-PIE C++ ELF that links SDL2/FNA3D dynamically, so a preloaded
loader can interpose `FNA3D_SwapBuffers` (per-frame) and `SDL_PollEvent` (input),
`dlopen` mod `.so`s, and let them call engine functions at their fixed addresses.

```sh
cd native
make
make install GAME=/path/to/Eets
EETS_DIR=/path/to/Eets ../run-eets.sh      # or set the Steam LD_PRELOAD launch option
```

```cpp
#include "eetsmod.h"
using namespace Eets;
extern "C" void EetsMod_OnKey(int key, int mods, int down) {
    if (down && key == 'g' && (mods & EKMOD_CTRL))
        World_SetGravity({0, World_GetGravity().y * 0.25f}, 0);
}
```

Full guide, API, and build details: **[`native/README.md`](native/README.md)**.

---

## Lua mods (zero-patch alternative)

No preload required. Eets embeds **Lua 5.0** (via luabind) and renders through
FNA3D/FAudio/SDL2. At boot the engine runs, in order:

```
Lua Game Lib -> Register Extensions -> Physics/Sound/FX/World/Simulator
-> default key binds -> dofile(DATA:localexec.lua)   <-- entry point
```

`localexec.lua` is optional (the engine only warns if it's missing), and by the
time it runs the full Lua C API and all luabind classes are registered. That
makes it a clean, patch-free mod entry point.

## Layout

```
src/
  localexec.lua              -> install to <game>/Data/localexec.lua
  Mods/
    framework.lua            loader + Mods.* API
    API_REFERENCE.md         full verified Lua API (globals, classes, hooks)
    example_lowgrav/mod.lua  sample mod (CTRL+G toggles low gravity)
install.sh                   copies src/ into the game's Data/
```

## Install

```sh
./install.sh "/path/to/steamapps/common/Eets"
```

Default path if omitted: `~/.local/share/Steam/steamapps/common/Eets`.
The script backs up any existing `Data/localexec.lua` before copying.

To verify: launch the game and check `<game>/Log/mods.log`:

```
[Mods] framework 0.2.0 scanning Data/Mods
[Mods] loaded 'example_lowgrav' 1.0
[Mods] done; 1 mod(s) from 1 candidate(s)
```

## Writing a mod

`<game>/Data/Mods/mymod/mod.lua`:

```lua
Mods.register{
    name     = "mymod",
    version  = "1.0",
    priority = 0,                                    -- higher loads first
    init      = function() Mods.log("hello") end,   -- once at boot
    update    = function() end,                      -- every frame (Mods.frame)
    onpause   = function() end,
    onunpause = function() end,
    keys      = { ["CTRL+G"] = function() end },     -- engine Bind()
}
```

A mod directory may also contain:

```
mymod/
  mod.lua            required
  config.lua         optional: `return { ... }` -> Mods.config("mymod", defaults)
  objects/*.lua      new blueprints, copied to Data/Objects/ (lazy-loaded, no restart)
  extensions/*.lua   copied to Data/Extensions/ (needs a restart to register)
  assets/<rel>       file overrides, copied to Data/<rel> (textures/sounds/anims)
```

Framework auto-copies `objects/`, `extensions/`, and `assets/` into the game's
`Data/` at boot. Stock files are never clobbered (only framework-managed ones).

Full API in [`src/Mods/API_REFERENCE.md`](src/Mods/API_REFERENCE.md).
See `example_lowgrav` (config + keys) and `example_customobject` (drop-in blueprint).

## `Mods` API

```lua
Mods.log(s)                       -- -> Log/mods.log + engine console
Mods.register(def)                -- register a mod (see above)
Mods.config(name, defaults)       -- load <mod>/config.lua merged over defaults
Mods.spawn(name, x, y)            -- World_CreateObject(name, Vector2(x,y))
Mods.bindkey(key, fn)             -- bind a key to a Lua function
Mods.frame                        -- running frame counter
-- level (.eet) toolchain (uses the engine's own VM -> byte-compatible):
Mods.eet.compile(src) -> bytes    -- Lua level source -> .eet bytecode
Mods.eet.build(src, outpath)      -- compile and write a .eet file
Mods.eet.read(path) -> table      -- run a .eet/.lua level in a sandbox, get its globals
Mods.eet.serialize(name, tbl)     -- table -> readable Lua source (.eet -> .lua)
```

## Capabilities

**Runtime (this framework):** boot init, per-frame `update`, `onpause`/`onunpause`,
key binds, config, load-order, the gameplay API (`World_*`, `Object_*`, `Sound_*`,
`FX_*`, `Profile_*`), luabind class ctors (`Vector2`, `Colour`, `Object`,
`MotionModel`, `*Extension`), and the full Lua 5.0 stdlib.

**New object blueprints — runtime drop-in.** Verified by reverse-engineering
`ObjectMgr::CreateObject`: blueprints load lazily by name from
`DATA:Objects/<name>.lua` on first `World_CreateObject` and are then cached. So a
mod just ships `objects/<name>.lua` and calls `World_CreateObject("<name>", pos)` —
no `AllObjects.lua` entry, no restart. (`AllObjects.lua` only populates the level
editor's toolbar palette.) New objects may use any of the ~27 existing extensions.

**New extension *types*** are registered before `localexec`, so adding one needs a
file in `Data/Extensions/` and a restart.

**Levels** are Lua 5.0 precompiled chunks (`.eet`). The toolchain above compiles
Lua source to byte-compatible `.eet` using the engine's own `string.dump`, and
reads existing `.eet` back to a data table — so procedural level generation and
`.eet`-to-readable-Lua both work. The in-game editor (CTRL+S) remains the
interactive authoring path.

**Asset overrides** (textures/sounds/anims): drop the file in `<mod>/assets/<rel>`;
the framework copies it to `Data/<rel>`. (The `DATA:`/`USER:`/`LEVELS:` virtual
mounts are C-side only, not exposed to Lua, so there is no live path remap.)

> Lua 5.0 notes: no `#` length operator (use `table.getn`), `string.gfind` not
> `gmatch`, `math.mod` not `%`.

## License

MIT.
