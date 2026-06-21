# Eets decompilation (reference)

Full Ghidra decompilation of the `Eets` binary, for documentation and context
when writing native mods (the binary is not stripped, so names are preserved).

- **7,395 functions** decompiled (of 7,725; the remainder are thunks/data stubs
  that don't decompile), split into `part_001.c` … `part_008.c` (~1000 functions
  each, ~1.5 MB per file so they stay browseable).
- **`INDEX.txt`** — tab-separated `address  name  file`, one line per function.
  Find a function's part quickly:

  ```sh
  grep -i World_CreateObject INDEX.txt
  # 005bc660	World_CreateObject	part_00X.c
  ```

This is decompiler output (Ghidra), not original source — types and control flow
are reconstructed and imperfect, but it's accurate enough to read behaviour and
find the addresses/ABIs used by `native/include/eets_engine.h`.

## Regenerate

After a game update (addresses change), regenerate with a local Ghidra:

```sh
GHIDRA_DIR=/opt/ghidra ./tools/gen_decompiled.sh /path/to/Eets
```

Uses `tools/DecompAll.java` (a headless `GhidraScript` that decompiles every
function to one file) + a splitter.
