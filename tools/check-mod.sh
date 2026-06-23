#!/usr/bin/env bash
# compile a native mod the way the loader does, without launching the game
#   ./tools/check-mod.sh mods/foo.cpp [bar.cpp ...]
# honors loader env: CXX, EETS_INCLUDE, EETSMOD_DEBUG, EETSMOD_CXXFLAGS
set -euo pipefail

[[ $# -ge 1 ]] || { echo "usage: $0 <mod.cpp> [more.cpp ...]" >&2; exit 1; }

SELF="$(cd "$(dirname "$0")/.." && pwd)"
INC="${EETS_INCLUDE:-$SELF/include}"
[[ -f "$INC/eetsmod.h" ]] || { echo "no eetsmod.h under $INC (set EETS_INCLUDE)" >&2; exit 1; }
CXX="${CXX:-g++}"

OPT="-O2"
[[ "${EETSMOD_DEBUG:-0}" != "0" ]] && OPT="-g -O0"
FLAGS=( $OPT -fPIC -std=c++17 -Wall -shared "-I$INC" ${EETSMOD_CXXFLAGS:-} )

rc=0
for f in "$@"; do
	out="$(mktemp --suffix=.so)"
	if "$CXX" "${FLAGS[@]}" -o "$out" "$f"; then echo "ok:   $f"; else echo "FAIL: $f"; rc=1; fi
	rm -f "$out"
done
exit $rc
