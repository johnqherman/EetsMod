# Known issues & caveats

## Game version / Steam updates
The engine addresses are reverse-engineered against a specific build of Eets. This release targets the
build with GNU BuildID `e81cc5504d3ef03324805df3e9fc508c1bf8c628` (`Eets::EXPECTED_BUILDID`).

If Steam updates Eets, the addresses can shift. On Linux the loader logs a `buildid: WARNING ...` line
on startup when the running binary doesn't match; it still loads, so a mod hitting a moved address may
misbehave or fault. A faulting mod is caught and disabled for the rest of the session (the game keeps
running) — but the fix is to re-tune the addresses for the new build.

- Run `eetsmod doctor` to check your install.
- Watch the loader log for the buildid warning.
- If mods stop working after a Steam update, the framework needs re-tuning against the new build
  (`tools/gen_engine_header.sh`); open an issue.

## Platform gaps
- **`World_EnterLevel()` is Linux-only.** The Windows addresses for `World::i` / `StartBuilder` /
  `GUI_OnUpdate` aren't reverse-engineered yet, so this call is a safe no-op on Windows (returns
  without doing anything — it won't crash). Mods that programmatically enter a level work on Linux only.
- Everything else in the API is available on both Linux and Windows.

## Notes
- Windows uses the `version.dll` proxy (auto-loads, no launch option). Linux uses
  `LD_PRELOAD=./libeetsmod.so %command%` as a Steam launch option. See `install.sh` / `install.bat`.
- The framework ships no game code — only original tooling and addresses derived from our own copy of
  Eets. Not affiliated with or endorsed by Klei Entertainment.
