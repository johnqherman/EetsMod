# Eets engine internals (reverse-engineering notes)

Findings backing the framework. Binary: `Eets`, x86-64 ELF, **not stripped**.

## Engine

- Native C++ engine; embeds **Lua 5.0** (`$Lua: Lua 5.0 ... PUC-Rio $`) bound via
  **luabind** (boost). Renders through FNA3D / FAudio / SDL2 (`lib64/`).
- Module boot order (`Log/game.log`): Messaging -> StringPool -> Lua Game Lib ->
  GlobalSettings -> Graphics -> Window -> Input -> ObjectMgr -> Extension Mgr ->
  Register Extensions -> Physics -> Sound -> FX -> World -> Simulator -> default key
  binds -> `dofile(DATA:localexec.lua)`.

## Mod entry point

`localexec.lua` is `dofile`'d at boot, after the full Lua API and all extensions
register. It is optional (engine only warns if missing). No binary patch needed.

## Lua API recovery

luabind registers Lua names that differ from the C++ static symbol names, so the
authoritative list comes from dumping live `_G` inside `localexec` (`getfenv(1)`).
Gotchas: keybind is `Bind` (not `Misc_BindKey`), print is `Print`, resource loader
is `LoadFile`. Engine-invoked global hooks: `Update()` (per frame, confirmed),
`OnPause()`, `OnUnpause()`. ~98 globals; class ctors exposed as userdata globals.
Full list in `src/Mods/API_REFERENCE.md`.

## Objects / blueprints

- ECS: an object blueprint (`Data/Objects/<name>.lua`, global `ObjectData`)
  composes extensions (`Data/Extensions/<X>Extension.lua`) plus a
  `-- Begin Scripting --` section with `OnInit`/`Update`/`OnHit` and `this`.
- **`ObjectMgr::CreateObject(name, layer)`** (decompiled): rolling `*31` hash of
  the name -> look up a blueprint cache (`this+0x78`). On miss it builds the path
  `ObjectMgr::Data::filepath ("DATA:Objects/") + name + fileextension (".lua")`,
  constructs the `ObjectBluePrint`, caches it, then `Object::Load`. => blueprints
  are **lazy-loaded by name and cached** -> new objects are pure runtime drop-ins.
- `AllObjects.lua` is read only by `FillToolbar(Toolbar&)` (level-editor palette),
  iterating `allobjects` -> `Toolbar::AddObject`. Not the blueprint registry.

## Levels (.eet)

- `.eet` = Lua 5.0 precompiled chunk. Header
  `1b 4c 75 61 50 01 04 04 04 06 08 09 09 08 ...` (ESC"Lua", v5.0). Running the
  chunk builds the level's global table (`Level`, `Objects`, per-object `Object`,
  `posx`, `posy`, ...).
- The engine's own `string.dump(loadstring(src))` produces a **byte-identical
  header** (verified), so level source compiles to compatible `.eet` in-engine -
  no external `luac`. `World_SaveLevel` is the C-side writer (CTRL+S in editor).

## Filepath aliases

`Alias`/`Filepath` provide virtual mounts `DATA: USER: LEVELS: GUI: FX: LOG: URL:`
(`AddAlias`/`ResolveAlias`/`GetAliasedPath`). C-side only - not exposed to Lua, so
asset overrides are done by replacing files on disk, not by runtime remap.

## Ghidra MCP note

In the build used here, only address-parameter endpoints worked
(`decompile_function_by_address`, `get_function_by_address`, `list_functions`);
name-search, string-filter, and xref endpoints returned 404. Workaround: grep the
`nm`/`objdump`/`strings` output of the binary for addresses and string refs, then
decompile by address.
