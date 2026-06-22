#!/usr/bin/env bash
# Generate docs/NATIVE_API.md from the public header signatures, so the native
# API is discoverable without grepping headers. Run after changing the headers.
set -euo pipefail
export LC_ALL=C   # deterministic sort order across machines (CI runs C locale)
SELF="$(cd "$(dirname "$0")/.." && pwd)"
INC="$SELF/include"
OUT="$SELF/docs/NATIVE_API.md"

inlines() {  # eets_engine.h / eets_ui.h: inline RET Name(args) { ... }
	grep -hE '^[[:space:]]*inline ' "$1" 2>/dev/null \
		| sed -E 's/[[:space:]]*\{.*$//; s/^[[:space:]]*inline /  /; s/[[:space:]]+$//' \
		| sort -u || true
}
decls() {    # eetsmod.h: indented function declarations ending in );
	grep -hE '^[[:space:]]+[A-Za-z].*\(.*\);[[:space:]]*$' "$1" 2>/dev/null \
		| sed -E 's/^[[:space:]]+/  /; s/[[:space:]]+$//' | sort -u || true
}

{
	echo "# Native API reference"
	echo
	echo "Auto-generated from \`include/*.h\` by \`tools/gen-api-ref.sh\`. Do not edit by hand."
	echo "All symbols live in namespace \`Eets\`. Signatures only; see the headers for offset/ABI notes."
	echo
	echo "## Engine wrappers (\`eets_engine.h\`)"
	echo '```cpp'; inlines "$INC/eets_engine.h"; echo '```'
	echo
	echo "## UI toolkit (\`eets_ui.h\`)"
	echo '```cpp'; inlines "$INC/eets_ui.h"; echo '```'
	echo
	echo "## Loader services (\`eetsmod.h\`)"
	echo '```cpp'; decls "$INC/eetsmod.h"; echo '```'
	echo
	echo "## Mod entry points"
	echo
	echo "Define any subset as \`extern \"C\"\`; the loader resolves each by name:"
	echo '```cpp'
	grep -hoE 'EetsMod_[A-Za-z]+' "$INC/eetsmod.h" "$SELF/loader/loader.cpp" | sort -u | sed 's/^/  /'
	echo '```'
} > "$OUT"
echo "wrote $OUT ($(wc -l < "$OUT") lines)"
