# Security

## What you are running

Eets mods are **native code** (`.so` plugins) that the loader injects into the
game process. A mod can do anything your user account can do - read/write files,
open the network, etc. There is no sandbox.

This is the same trust model as any native game mod (Skyrim SKSE plugins, Source
mods, etc.): power in exchange for trust.

## Rules of thumb

- **Only install mods from sources you trust.** A `.eetsmod`/`.so` from a stranger
  is as risky as any random program you would run.
- **Source is auditable.** A `.eetsmod` ships its `.cpp` alongside the prebuilt `.so`;
  you can read what a mod does before trusting it. The loader keeps both side by side.
- **Undo is clean.** Remove a mod by deleting its `.eetsmod` from `<game>/mods`
  (or `eetsmod remove <name>`). Clear the Steam launch option to turn the loader off
  entirely. Stock game files are never modified.

## The loader itself

- Verifies the game's BuildID and warns if it differs from the one the addresses
  were generated against (stale offsets are a crash risk, not a security one).
- Isolates crashes: a faulting mod is caught and disabled; the game keeps running.
- Unpacks a `.eetsmod` (a gzipped tar) into `mods/` with `tar`. A bundle is already
  trusted to the extent its `.so` is - it runs as native code - so treat the archive
  with the same caution as the mod itself.

## Reporting

Found a vulnerability in the loader or tooling (not in a third-party mod)? Open a
private security advisory on the GitHub repo, or email the maintainer. Please do not
file public issues for exploitable bugs until a fix is out.
