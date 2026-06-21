# Contributing

This repo is the **framework + example mods** - the thing people build mods *with*.
Individual mods live in their authors' own repos and are distributed as `.eetsmod`
files; they are not collected here.

## Work on the framework

```sh
make -C native           # build the loader
make -C native check      # compile every example like the loader does
make -C native apidoc     # regenerate docs/NATIVE_API.md after header changes
make -C native bundles    # pack the examples into native/examples/build/*.eetsmod
./tools/check-mod.sh <mod.cpp>   # compile one mod without launching the game
```

CI (`.github/workflows/ci.yml`) builds the loader, compiles the examples, lints the
shell tools, and fails if `docs/NATIVE_API.md` is stale. Run those before a PR.

## Good contributions

- New engine wrappers in `native/include/` (with an address + type-signature comment
  in `eets_addr.h`).
- Loader robustness, tooling, and developer experience.
- A new **example mod** that teaches a technique not already shown - keep it small and
  focused, add a `<name>.cfg` manifest, and make sure `make -C native check` passes.
- The Windows port: the seams exist (`platform.h`, `eets_addr_win.h`); see
  [`docs/WINDOWS.md`](docs/WINDOWS.md).

Steam Workshop integration is intentionally out of scope.

## Conventions

- **Comments:** lean and meaningful only - no restating code, no stating givens, no
  explaining past bugs. Keep only the non-obvious *why* (magic offsets, ABI/sret
  notes, gotchas). Lowercase, ASCII, no em dashes.
- **Shell:** scripts target bash; use `[[:space:]]` (not `\t`) in `grep`/`sed` so
  behavior matches across environments. Keep `bash -n` clean.
- **Engine addresses** live in `native/include/eets_addr.h`; regenerate with
  `native/gen_engine_header.sh` after a game update.
