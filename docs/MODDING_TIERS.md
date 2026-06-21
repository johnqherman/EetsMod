# Modding tiers, trust & content modding

## Two tiers

| | Native (C++) | Lua |
|---|---|---|
| Power | full — any engine call, hooks, UI, input | scripting only |
| Loading | preloaded loader (`LD_PRELOAD`) | zero-patch (`Data/localexec.lua`) |
| Trust | **arbitrary native code** | runs in the embedded Lua VM |
| Use for | tools, gameplay overhauls, UI, dev | content, tweaks, level logic |

Native mods are unrestricted machine code — treat them like any executable.
**Only run native mods you trust.** The Lua tier is the safer surface for
sharing untrusted content; it still has file/io access (Lua 5.0 `io`), so it is
not a hard sandbox, but it cannot call arbitrary syscalls the way native can. A
fully sandboxed content tier (stripped Lua env, no `io`/`os`) would be the next
step for safe distribution — not yet implemented.

## Content modding status

| Content | Status | How |
|---|---|---|
| **Gameplay / behaviour** | full | native hooks + bindings, or Lua object scripts |
| **New objects/blueprints** | works (runtime drop-in) | `Data/Objects/<name>.lua` + `World_CreateObject` (lazy-loaded; verified) |
| **Levels (.eet)** | works | Lua toolchain `Mods.eet.compile/build/read` (in-engine `string.dump`) or the in-game editor |
| **Localization** | binding available | `Eets::Localize("$id")` resolves; `StringPool::LoadFile` (addr) adds strings |
| **Custom textures/sprites** | advanced/experimental | `Texture::Load` + `IGraphicsEngine::DrawTexture` addresses exposed; no validated wrapper (caller-allocated Texture + GPU upload). File-replace under `Data/` is the reliable route today |
| **Custom animations** | advanced | `AnimExt::LoadAnimation` address exposed |
| **Sound** | partial | `Sound_CreateSound`/`Sound_PlayMusic` bindings; add files by replacing under `Data/Sound` |
| **Asset replacement** | works | overwrite the file under `Data/` (the `DATA:` alias is C-side only) |

## Integrity (replays / leaderboards)

Eets has a deterministic simulation and replays. A mod that changes simulation
behaviour invalidates replays and any leaderboard. Declare such mods with
`sim = 1` in their `.cfg`; the loader logs an integrity warning when one is
active. An official build should additionally disable score submission while a
`sim` mod is enabled.

## Recommended for an official launch
1. A stripped, sandboxed Lua content tier (no `io`/`os`) for Workshop-style
   sharing of untrusted content.
2. A validated asset pipeline (texture/anim/sound import) wrapping the addresses
   above, so content mods don't ship raw machine code.
3. Score/replay gating driven by the `sim` flag.
