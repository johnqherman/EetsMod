# Eets Mod Framework

Zero-patch Lua modding for **Eets** (the Klei puzzle game). No binary edits, no
`LD_PRELOAD` — the game `dofile`s an optional `Data/localexec.lua` at boot, which
bootstraps a loader that discovers and runs mods from `Data/Mods/<name>/mod.lua`.

## Why it works

Eets is a native C++ engine that embeds **Lua 5.0** (via luabind) and renders
through FNA3D/FAudio/SDL2. At boot the engine runs, in order:

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
    name    = "mymod",
    version = "1.0",
    init      = function() Mods.log("hello") end,   -- once at boot
    update    = function() end,                      -- every frame (Mods.frame)
    onpause   = function() end,
    onunpause = function() end,
    keys      = { ["CTRL+G"] = function() end },     -- engine Bind()
}
```

Full API in [`src/Mods/API_REFERENCE.md`](src/Mods/API_REFERENCE.md).

## Capabilities

**Runtime (this framework):** boot init, per-frame `update`, `onpause`/`onunpause`,
key binds, the gameplay API (`World_*`, `Object_*`, `Sound_*`, `FX_*`, `Profile_*`),
luabind class ctors (`Vector2`, `Colour`, `Object`, `MotionModel`, `*Extension`),
and the full Lua 5.0 stdlib (`os io string table math debug coroutine`).

**Static drop-ins (loaded before `localexec`):** new object blueprints need a
`Data/Objects/<x>.lua` file plus an entry in `Data/AllObjects.lua`; new extensions
go in `Data/Extensions/`.

**Assets** (textures/sounds/anims/levels): replace the file on disk under `Data/`;
the `DATA:`/`USER:`/`LEVELS:` virtual mounts are C-side only, not exposed to Lua.

> Lua 5.0 notes: no `#` length operator (use `table.getn`), `string.gfind` not
> `gmatch`, `math.mod` not `%`.

## License

MIT.
