#!/usr/bin/env bash
# install a custom sound into Eets, playable by name from a mod.
#   sfx:   ./add-sound.sh <name> <audiofile> [game-dir]
#   music: ./add-sound.sh --music <name> <audiofile> [game-dir]
# sfx:   Sound_CreateSound("<name>", ...)   music: Sound_PlayMusic("<name>", loop)
# .snd / music files are ogg vorbis; non-ogg input is converted with ffmpeg.
set -euo pipefail

MUSIC=0
if [[ "${1:-}" == "--music" ]]; then MUSIC=1; shift; fi
NAME="${1:-}"; SRC="${2:-}"
GAME="${3:-$HOME/.local/share/Steam/steamapps/common/Eets}"
[[ -n "$NAME" && -f "$SRC" ]] || { echo "usage: $0 [--music] <name> <audiofile> [game-dir]" >&2; exit 1; }
[[ -x "$GAME/Eets" ]] || { echo "no Eets at $GAME" >&2; exit 1; }

to_ogg() {  # $1 src, $2 dst.ogg
	case "$(file -b --mime-type "$1")" in
		audio/ogg|application/ogg) cp "$1" "$2" ;;
		*) command -v ffmpeg >/dev/null || { echo "need ffmpeg to convert $1" >&2; exit 1; }
		   ffmpeg -v error -y -i "$1" -c:a libvorbis "$2" ;;
	esac
}

if [[ "$MUSIC" == 1 ]]; then
	mkdir -p "$GAME/Data/Sound/Music"
	to_ogg "$SRC" "$GAME/Data/Sound/Music/$NAME.ogg"
	echo "music '$NAME' installed; play with Sound_PlayMusic(\"$NAME\", true)"
else
	mkdir -p "$GAME/Data/Sound/Wav" "$GAME/Data/Sound/Patch"
	SND="$GAME/Data/Sound/Wav/$NAME.snd"
	to_ogg "$SRC" "$SND"
	# frequency must match the .snd sample rate or it plays at the wrong pitch/speed
	SR=$(ffprobe -v error -select_streams a:0 -show_entries stream=sample_rate -of csv=p=0 "$SND" 2>/dev/null || true)
	[[ "$SR" =~ ^[0-9]+$ ]] || SR=$(file "$SND" | grep -oE '[0-9]+ Hz' | grep -oE '[0-9]+' || echo 44100)
	printf 'Patch = \r\n{\r\n\tname = "%s",\r\n\tfilepath = "DATA:Sound\\\\Wav\\\\%s.snd",\r\n\tmax_polyphony = 5,\r\n\tfrequency = %s,\r\n}' \
		"$NAME" "$NAME" "$SR" > "$GAME/Data/Sound/Patch/$NAME.ptch"
	echo "sfx '$NAME' installed (rate $SR); play with Eets::PlaySound(\"$NAME\")"
fi
echo "loaded at next launch (sound dirs are scanned at boot)."
