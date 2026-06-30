#!/usr/bin/env bash
# install the eetsmod loader + bundled mods into an Eets install. Run from the extracted release dir.
#   ./install.sh [GAME_DIR]
set -e
G="${1:-$HOME/.local/share/Steam/steamapps/common/Eets}"
os="$(uname -s 2>/dev/null || echo unknown)"
cd "$(dirname "$0")"

copy_payload() { # $1 = loader filename to install into the game dir
  install -m644 "$1" "$G/" 2>/dev/null || cp "$1" "$G/"
  mkdir -p "$G/eetsmod-include" "$G/mods"
  cp eetsmod-include/*.h "$G/eetsmod-include/"
  for m in mods/*.eetsmod; do [ -f "$m" ] && cp "$m" "$G/mods/"; done
  true
}

case "$os" in
  Linux)
    [ -x "$G/Eets" ] || { echo "no Eets binary at '$G' (pass the game dir as arg 1)"; exit 1; }
    copy_payload libeetsmod.so
    echo "installed (Linux)."
    echo "Set this Steam launch option for Eets (paste as-is): LD_PRELOAD=./libeetsmod.so %command%"
    ;;
  MINGW*|MSYS*|CYGWIN*)
    # Windows under a unix shell: the version.dll proxy auto-loads, no launch option
    [ -f "$G/Eets.exe" ] || { echo "no Eets.exe at '$G' (pass the game dir as arg 1)"; exit 1; }
    copy_payload version.dll
    echo "installed (Windows). No launch option needed - version.dll loads automatically. Launch Eets normally."
    ;;
  *)
    echo "unknown OS '$os'. See README.md for manual install."
    exit 1
    ;;
esac
