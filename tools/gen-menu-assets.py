#!/usr/bin/env python3
# Regenerate loader/menu_assets.h from data/menu/ (embedded title .anim + ModsDialog lua block).
import os
root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
anim = open(os.path.join(root,'data/menu/mods_title.anim'),'rb').read()
lua  = open(os.path.join(root,'data/menu/ModsDialog.lua')).read().rstrip('\n')
o = ['// AUTO-GENERATED from data/menu/ - embedded native-menu assets the loader installs at startup.',
     '// Regenerate: python3 tools/gen-menu-assets.py. Do not hand-edit.', '#pragma once',
     'static const unsigned char MODS_TITLE_ANIM[] = {']
for i in range(0,len(anim),16): o.append('  '+','.join(str(b) for b in anim[i:i+16])+',')
o += ['};', 'static const unsigned MODS_TITLE_ANIM_LEN = %d;'%len(anim), '',
      'static const char* MODS_DIALOG_LUA = R"EETSLUA(', lua, ')EETSLUA";']
open(os.path.join(root,'loader/menu_assets.h'),'w').write('\n'.join(o)+'\n')
print('wrote loader/menu_assets.h')
