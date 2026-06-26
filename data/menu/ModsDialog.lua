-- MODS Dialog (eetsmod): native modal, native static-text children. The loader sets their text each
-- frame (empty string = hidden) and hit-tests clicks against the known row positions below.
-- title is an image like vanilla OPTIONS (komika, orange + black outline) - bigger than the max text style
ModsDialogTitle = CreateImage( "ModsDialogTitle", [[GUI\main_menu\mods_title.anim]], {0, -157} )
local mods_dialog_children = { ModsDialogTitle }
-- list rows fill from the cream top; the loader puts the mods then "+ ADD A MOD" as the last used row
for i = 0, 5 do
  local row = CreateClickableText( "ModsDialogRow"..i, "", {0, -100 + i*24}, "brady large dropshadow centre" )
  row.width = 290; row.height = 22
  row.text_ext.colour = { r = 255, g = 255, b = 255, alpha = 255 }
  row.text_ext.hover_colour = { r = 255, g = 232, b = 40, alpha = 255 }
  table.insert( mods_dialog_children, row )
end
ModsDialogBack = CreateClickableText( "ModsDialogBack", "BACK", {100, 135}, "brady larger dropshadow centre" )
ModsDialogBack.width = 130; ModsDialogBack.height = 30
ModsDialogBack.text_ext.colour = { r = 255, g = 232, b = 40, alpha = 255 }
ModsDialogBack.text_ext.hover_colour = { r = 0, g = 255, b = 0, alpha = 255 }
table.insert( mods_dialog_children, ModsDialogBack )
ModsDialog = CreateModalDialog( "ModsDialog", mods_dialog_children, [[GUI\generic\medium_dialog.anim]], {506, 362} )
