#!/usr/bin/env bash
# Install the Eets mod framework into a game directory.
# Usage: ./install.sh [/path/to/steamapps/common/Eets]
set -euo pipefail

GAME="${1:-$HOME/.local/share/Steam/steamapps/common/Eets}"
HERE="$(cd "$(dirname "$0")" && pwd)"

if [[ ! -x "$GAME/Eets" ]]; then
	echo "error: no Eets binary at '$GAME' (pass the game dir as arg 1)" >&2
	exit 1
fi

# back up an existing localexec.lua once
if [[ -f "$GAME/Data/localexec.lua" && ! -f "$GAME/Data/localexec.lua.bak" ]]; then
	cp "$GAME/Data/localexec.lua" "$GAME/Data/localexec.lua.bak"
	echo "backed up existing localexec.lua -> localexec.lua.bak"
fi

cp "$HERE/src/localexec.lua" "$GAME/Data/localexec.lua"
mkdir -p "$GAME/Data/Mods"
cp -r "$HERE/src/Mods/." "$GAME/Data/Mods/"

echo "installed to: $GAME/Data"
echo "launch the game, then check $GAME/Log/mods.log"
