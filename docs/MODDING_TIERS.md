# Modding tiers, trust & content modding

## Two tiers

| | Native (C++) | Lua |
|---|---|---|
| Power | full — any engine call, hooks, UI, input | scripting only |
| Loading | preloaded loader (`LD_PRELOAD`) | zero-patch (`Data/localexec.lua`) |
| Trust | **arbitrary native code** | runs in the embedded Lua VM |
| Use for | tools, gameplay overhauls, UI, dev | content, tweaks, level logic |

Native mods are unrestricted machine code — treat them like any executable.
**Only run native mods you trust.**

### Sandboxed Lua content tier (implemented)
Drop a `.lua` in `Data/Mods/content/`. It runs in a restricted environment with
**no `io`/`os`/`dofile`/`loadfile`/`loadstring`/`require`/`package`/`debug`** —
only the gameplay API (`World_*`, `Object_*`, class ctors), safe stdlib
(`math`/`string`/`table` + base), and a limited `Mods` API (`register`/`log`/
`spawn`/`config`). This is the safer surface for sharing untrusted content
(verified: `io`/`os`/`dofile` resolve to `nil` inside a content mod). It is not a
hard *resource* sandbox — infinite loops / huge allocations are still possible.
Trusted full-access Lua mods still go in `Data/Mods/<name>/mod.lua`.

## Content modding status

| Content | Status | How |
|---|---|---|
| **Gameplay / behaviour** | full | native hooks + bindings, or Lua object scripts |
| **New objects/blueprints** | works (runtime drop-in) | `Data/Objects/<name>.lua` + `World_CreateObject` (lazy-loaded; verified) |
| **Levels (.eet)** | works | Lua toolchain `Mods.eet.compile/build/read` (in-engine `string.dump`) or the in-game editor |
| **Localization** | binding available | `Eets::Localize("$id")` resolves; `StringPool::LoadFile` (addr) adds strings |
| **Custom images** | works | `Eets::DrawImage(path, x, y, tint)` + `Eets::LoadSprite(path)` via `SpriteManager::Load` + `GraphicsEngine::DrawSprite` (the game's own sprite renderer; verified rendering a 512x512 PNG). jpg/tga/dds/png; draws at native size in the sprite render space |
| **Custom animations** | works | `Eets::DrawAnim(path, x, y, dt[, fps])` (`.anim` spritesheets; per-frame UV via `GetDiffuseUV`; native rate from `AnimFrameDuration`). `Eets::DrawImageHUD` screen-locks images in-level |
| **Sound** | partial | `Sound_CreateSound`/`Sound_PlayMusic` bindings; add files by replacing under `Data/Sound` |
| **Asset bundling** | works | native loader copies `mods/assets/<rel>` -> `Data/<rel>` at boot; reference by name via the engine's content path |
| **Asset replacement** | works | overwrite the file under `Data/` (the `DATA:` alias is C-side only) |

## Integrity (replays / leaderboards)

Eets has a deterministic simulation and replays. A mod that changes simulation
behaviour invalidates replays and any leaderboard. Declare such mods with
`sim = 1` in their `.cfg`; the loader logs an integrity warning when one is
active. An official build should additionally disable score submission while a
`sim` mod is enabled.

## Recommended for an official launch
1. ~~Sandboxed Lua content tier~~ — **done** (`Data/Mods/content/`).
2. ~~Asset bundling + custom image draw~~ — **done** (`mods/assets/` → `Data/`;
   `Eets::DrawImage` via the engine's Sprite renderer, verified rendering).
3. Score/replay gating driven by the `sim` flag (warning emitted today; actual
   gating needs the score-submit hook).
