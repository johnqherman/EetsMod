# Eets engine internals (reverse-engineering notes)

Findings backing the framework. Binary: `Eets`, x86-64 ELF, **not stripped**.

## Engine

- Native C++ engine; embeds **Lua 5.0** (`$Lua: Lua 5.0 ... PUC-Rio $`) bound via
  **luabind** (boost). Renders through FNA3D / FAudio / SDL2 (`lib64/`).
- Module boot order (`Log/game.log`): Messaging -> StringPool -> Lua Game Lib ->
  GlobalSettings -> Graphics -> Window -> Input -> ObjectMgr -> Extension Mgr ->
  Register Extensions -> Physics -> Sound -> FX -> World -> Simulator -> default key
  binds -> `dofile(DATA:localexec.lua)`.

## Finding an engine function (to add a wrapper)

luabind exposes ~98 globals whose Lua names differ from the C++ static symbols, so
the address table is built by mapping names to symbols. To wrap an unmapped function,
dump live `_G` (`getfenv(1)`) from the engine's optional `localexec.lua` boot hook to
recover its name, then locate the symbol and decompile by address. Naming gotchas:
keybind is `Bind` (not `Misc_BindKey`), print is `Print`, loader is `LoadFile`;
engine-invoked global hooks are `Update()`, `OnPause()`, `OnUnpause()`.

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
