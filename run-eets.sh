#!/usr/bin/env bash
# Launch Eets with the native mod loader preloaded.
# Run from the game directory, or set EETS_DIR=/path/to/Eets.
set -euo pipefail

GAME="${EETS_DIR:-$(pwd)}"
[[ -x "$GAME/Eets" ]] || { echo "no Eets binary at '$GAME' (set EETS_DIR)" >&2; exit 1; }
[[ -f "$GAME/libeetsmod.so" ]] || { echo "libeetsmod.so not installed in '$GAME' (run: make -C native install GAME=$GAME)" >&2; exit 1; }

cd "$GAME"
# bundled assets must be in Data/ before boot (sound dirs are scanned at startup)
[[ -d "$GAME/mods/assets" ]] && cp -rn "$GAME/mods/assets/." "$GAME/Data/" 2>/dev/null || true
export EETS_MODS="${EETS_MODS:-$GAME/mods}"
export LD_PRELOAD="$GAME/libeetsmod.so${LD_PRELOAD:+:$LD_PRELOAD}"
exec ./Eets "$@"

# Steam launch option equivalent (Properties -> Launch Options):
#   LD_PRELOAD=/full/path/to/Eets/libeetsmod.so %command%
