#!/usr/bin/env bash
# Regenerate docs/decompiled/ - full Ghidra decompilation of the Eets binary.
# Requires a local Ghidra install (set GHIDRA_DIR or have analyzeHeadless on PATH).
#   ./tools/gen_decompiled.sh /path/to/Eets
set -euo pipefail

BIN="${1:-}"
[[ -x "$BIN" ]] || { echo "usage: $0 /path/to/Eets" >&2; exit 1; }

HEADLESS="$(command -v analyzeHeadless || echo "${GHIDRA_DIR:-/opt/ghidra}/support/analyzeHeadless")"
[[ -x "$HEADLESS" ]] || { echo "analyzeHeadless not found (set GHIDRA_DIR)" >&2; exit 1; }

HERE="$(cd "$(dirname "$0")" && pwd)"
REPO="$(dirname "$HERE")"
PROJ="$(mktemp -d)"
RAW="$(mktemp).c"

echo "[1/3] headless analyze + decompile (~minutes)..."
"$HEADLESS" "$PROJ" EetsDecomp \
	-import "$BIN" \
	-scriptPath "$HERE" \
	-postScript DecompAll.java "$RAW" \
	-deleteProject

echo "[2/3] splitting into docs/decompiled/..."
mkdir -p "$REPO/docs/decompiled"
rm -f "$REPO/docs/decompiled/"*.c "$REPO/docs/decompiled/INDEX.txt"
RAW="$RAW" OUT="$REPO/docs/decompiled" python3 - <<'PY'
import re, os
src=os.environ["RAW"]; outdir=os.environ["OUT"]
hdr=re.compile(r'^// ======== (.+?) @ ([0-9a-fA-F]+) ========')
lines=open(src).readlines(); blocks=[]; cur=None; buf=[]; i=0
while i<len(lines) and not hdr.match(lines[i]): i+=1
while i<len(lines):
    m=hdr.match(lines[i])
    if m:
        if cur: blocks.append((cur[0],cur[1],"".join(buf)))
        cur=(m.group(1),m.group(2)); buf=[lines[i]]
    else: buf.append(lines[i])
    i+=1
if cur: blocks.append((cur[0],cur[1],"".join(buf)))
PER=1000; n=(len(blocks)+PER-1)//PER
idx=open(os.path.join(outdir,"INDEX.txt"),"w")
for p in range(n):
    ch=blocks[p*PER:(p+1)*PER]; fn="part_%03d.c"%(p+1)
    with open(os.path.join(outdir,fn),"w") as o:
        o.write("// Eets decompilation part %d/%d\n\n"%(p+1,n))
        for nm,ad,bl in ch:
            o.write(bl); o.write("\n" if bl.endswith("\n") else "\n\n")
    for nm,ad,bl in ch: idx.write("%s\t%s\t%s\n"%(ad,nm,fn))
idx.close(); print("blocks=%d parts=%d"%(len(blocks),n))
PY

echo "[3/3] done -> docs/decompiled/"
rm -f "$RAW"
