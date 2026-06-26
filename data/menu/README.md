# Native MODS menu assets

These back the native main-menu MODS button + modal (the loader drives them at runtime).

- `mods_title.anim` — the "MODS" title image (komika/brady, orange + black outline + drop shadow,
  DXT3, styled to match vanilla `options_title.anim`). Used by the dialog title `CreateImage`.
- `ModsDialog.lua` — the `ModsDialog` definition block (a real `CreateModalDialog` with the title image,
  6 clickable row slots, and a BACK button). The loader finds these widgets by name and drives them.

## Install — automated by the loader at preboot

`install_menu_assets()` (loader.cpp, run from `eetsmod_preboot()` before the menu screen loads) does this
idempotently from the copies embedded in `loader/menu_assets.h`:
1. Writes `mods_title.anim` → `<game>/Data/Animations/GUI/main_menu/` (if missing or wrong size).
2. Injects the `ModsDialog` block into `<game>/Data/GUI/MainMenu_Screen.lua` **and** adds `ModsDialog,`
   to its `widgets = { ... }` table (without that entry `GUI::FindWidget("ModsDialog")` returns null).
   Skipped if the file already contains `ModsDialog`.

`loader/menu_assets.h` is generated from these source files — regenerate after editing either:

    python3 tools/gen-menu-assets.py
