#!/usr/bin/env bash
# Print the Steam launch-option string for the native mod loader.
#   ./tools/steam-launch-option.sh [/path/to/Eets]
set -euo pipefail

GAME="${1:-$HOME/.local/share/Steam/steamapps/common/Eets}"
GAME="$(cd "$GAME" 2>/dev/null && pwd || echo "$GAME")"
SO="$GAME/libeetsmod.so"

echo "Game dir: $GAME"
if [[ ! -f "$SO" ]]; then
	echo "WARNING: loader not installed yet. Build + install first:"
	echo "    make -C native install GAME=\"$GAME\""
	echo
fi

echo "Steam -> right-click Eets -> Properties -> Launch Options, paste:"
echo
echo "    LD_PRELOAD=$SO %command%"
echo
echo "Note: mods are .cpp compiled on launch, so g++ must be on PATH. Steam's"
echo "runtime container often lacks it - run once from a terminal first to cache"
echo "the builds (mods/.cache/*.so), or disable the Steam runtime for Eets."
echo
echo "Terminal launch (recommended for development):"
echo "    cd \"$GAME\" && LD_PRELOAD=\"\$PWD/libeetsmod.so\" ./Eets"
