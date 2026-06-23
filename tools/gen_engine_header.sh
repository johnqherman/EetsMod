#!/usr/bin/env bash
# regenerate address constants in include/eets_addr.h (stable per build: binary is non-PIE)
#   ./gen_engine_header.sh /path/to/Eets
set -euo pipefail

BIN="${1:-}"
[[ -x "$BIN" ]] || { echo "usage: $0 /path/to/Eets" >&2; exit 1; }

# mangled symbol -> constant name in the header
declare -A SYMS=(
	[_ZL15World_GetGravityv]=World_GetGravity
	[_ZL15World_SetGravity7Vector2i]=World_SetGravity
	[_ZL16World_CreateObjectPKc7Vector2]=World_CreateObject
	[_ZL12World_GetEetsv]=World_GetEets
	[_ZL20World_CreateExplosionRK7Vector2f]=World_CreateExplosion
	[_ZL27World_CreateExplosionSpecialv]=World_CreateExplosionSpecial
	[_ZL19Object_ApplyImpulseP6ObjectRK7Vector2]=Object_ApplyImpulse
	[_ZL20Object_EnablePhysicsP6Objectb]=Object_EnablePhysics
)

BUILDID=$(readelf -n "$BIN" 2>/dev/null | awk '/Build ID/{print $3}')
echo "// addresses for BuildID ${BUILDID:-unknown}"
for mangled in "${!SYMS[@]}"; do
	addr=$(nm "$BIN" 2>/dev/null | awk -v s="$mangled" '$3==s{print $1}')
	name=${SYMS[$mangled]}
	if [[ -n "$addr" ]]; then
		printf 'constexpr uintptr_t %-28s = 0x%s;\n' "$name" "$(echo "$addr" | sed 's/^0*//')"
	else
		echo "// MISSING: $name ($mangled)" >&2
	fi
done
echo
echo "Paste the constexpr lines into include/eets_addr.h (namespace Eets::addr)." >&2
