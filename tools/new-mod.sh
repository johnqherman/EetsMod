#!/usr/bin/env bash
# Scaffold a new Eets native mod.
#   ./tools/new-mod.sh <name> [mods-dir]
# Default mods-dir: $HOME/.local/share/Steam/steamapps/common/Eets/mods
set -euo pipefail

NAME="${1:-}"
[[ -n "$NAME" ]] || { echo "usage: $0 <name> [mods-dir]" >&2; exit 1; }
DIR="${2:-$HOME/.local/share/Steam/steamapps/common/Eets/mods}"
mkdir -p "$DIR"
CPP="$DIR/$NAME.cpp"
CFG="$DIR/$NAME.cfg"
[[ -e "$CPP" ]] && { echo "exists: $CPP" >&2; exit 1; }

cat > "$CPP" <<EOF
// $NAME - an Eets native mod.
#include "eetsmod.h"
// #include "eets_ui.h"   // uncomment for custom menus
using namespace Eets;

extern "C" void EetsMod_Init() {
    Eets::Log("$NAME: loaded");
}

extern "C" void EetsMod_Update() {
    // runs every frame
}

extern "C" void EetsMod_OnKey(int key, int mods, int down) {
    // if (down && key == 'k' && (mods & EKMOD_CTRL)) { /* ... */ }
}

extern "C" void EetsMod_OnEvent(const char* name, void* a, void* b) {
    // events: object_spawn, object_killed, level_load, level_reset, level_complete
}
EOF

cat > "$CFG" <<EOF
# $NAME manifest + config (all optional)
version = 1.0
author = you
# priority = 0          # higher loads first
# requires = othermod   # comma-separated deps
# sim = 0               # 1 if this mod affects the simulation (replays/leaderboard)
# min_framework = 0.7.0
EOF

echo "created:"
echo "  $CPP"
echo "  $CFG"
echo "Launch the game (loader compiles it). Logs: <game>/Log/native_mods.log"
