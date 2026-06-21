#!/usr/bin/env bash
# Make a custom Eets sound (ogg .snd + sample-rate-matched .ptch), playable by name.
#   into a mod's asset tree (for bundling):  ./add-sound.sh --into <dir> [--music] <name> <audiofile>
#   straight into a game (for quick testing): ./add-sound.sh [--music] <name> <audiofile> [game-dir]
# sfx:   Eets::PlaySound("<name>")     music: Sound_PlayMusic("<name>", loop)
# non-ogg input is converted with ffmpeg. the .ptch frequency tracks the .snd sample rate.
set -euo pipefail

INTO=""; MUSIC=0
while [[ "${1:-}" == --* ]]; do
	case "$1" in
		--into) INTO="$2"; shift 2;;
		--music) MUSIC=1; shift;;
		*) echo "unknown option $1" >&2; exit 1;;
	esac
done
NAME="${1:-}"; SRC="${2:-}"
[[ -n "$NAME" && -f "$SRC" ]] || { echo "usage: $0 [--into <dir>] [--music] <name> <audiofile> [game-dir]" >&2; exit 1; }

# SNDROOT is the dir that holds Sound/. for a mod asset tree that's <dir>; for a game it's <game>/Data.
if [[ -n "$INTO" ]]; then
	SNDROOT="$INTO"; WHERE="$INTO"
else
	GAME="${3:-$HOME/.local/share/Steam/steamapps/common/Eets}"
	[[ -x "$GAME/Eets" ]] || { echo "no Eets at $GAME (or use --into <mod>.assets)" >&2; exit 1; }
	SNDROOT="$GAME/Data"; WHERE="$GAME"
fi

to_ogg() {  # $1 src, $2 dst.ogg
	case "$(file -b --mime-type "$1")" in
		audio/ogg|application/ogg) cp "$1" "$2" ;;
		*) command -v ffmpeg >/dev/null || { echo "need ffmpeg to convert $1" >&2; exit 1; }
		   ffmpeg -v error -y -i "$1" -c:a libvorbis "$2" ;;
	esac
}

if [[ "$MUSIC" == 1 ]]; then
	mkdir -p "$SNDROOT/Sound/Music"
	to_ogg "$SRC" "$SNDROOT/Sound/Music/$NAME.ogg"
	echo "music '$NAME' -> $WHERE; play with Sound_PlayMusic(\"$NAME\", true)"
else
	mkdir -p "$SNDROOT/Sound/Wav" "$SNDROOT/Sound/Patch"
	SND="$SNDROOT/Sound/Wav/$NAME.snd"
	to_ogg "$SRC" "$SND"
	SR=$(ffprobe -v error -select_streams a:0 -show_entries stream=sample_rate -of csv=p=0 "$SND" 2>/dev/null || true)
	[[ "$SR" =~ ^[0-9]+$ ]] || SR=$(file "$SND" | grep -oE '[0-9]+ Hz' | grep -oE '[0-9]+' || echo 44100)
	printf 'Patch = \r\n{\r\n\tname = "%s",\r\n\tfilepath = "DATA:Sound\\\\Wav\\\\%s.snd",\r\n\tmax_polyphony = 5,\r\n\tfrequency = %s,\r\n}' \
		"$NAME" "$NAME" "$SR" > "$SNDROOT/Sound/Patch/$NAME.ptch"
	echo "sfx '$NAME' (rate $SR) -> $WHERE; play with Eets::PlaySound(\"$NAME\")"
fi
[[ -n "$INTO" ]] && echo "bundled into the mod; 'eetsmod pack' will include it." \
                 || echo "loaded at next launch (sound dirs are scanned at boot)."
