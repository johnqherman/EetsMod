# Native MODS menu assets

These back the native main-menu MODS button + modal (the loader drives them at runtime).

- `mods_title.anim` — the "MODS" title image (komika/brady, orange + black outline + drop shadow,
  DXT3, styled to match vanilla `options_title.anim`). Used by the dialog title `CreateImage`.
- `ModsDialog.lua` — the `ModsDialog` definition block (a real `CreateModalDialog` with the title image,
  6 clickable row slots, and a BACK button). The loader finds these widgets by name and drives them.

## Install (NOT yet automated — currently applied by hand to the game's Data/ for testing)

For shipping, the loader must, at startup:
1. Copy `mods_title.anim` → `<game>/Data/Animations/GUI/main_menu/mods_title.anim`.
2. Inject the `ModsDialog.lua` block into `<game>/Data/GUI/MainMenu_Screen.lua` **and** add `ModsDialog,`
   to the `widgets = { ... }` registration table near the end of that file (without that entry the GUI
   never instantiates the dialog and `GUI::FindWidget("ModsDialog")` returns null).
