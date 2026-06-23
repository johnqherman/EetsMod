#!/usr/bin/env bash
# Regenerate the "=== BEGIN GENERATED ===".."=== END GENERATED ===" block of include/eets_addr.h - the
# complete Linux engine map - in place, from nm on the binary (full demangled names). The curated entries
# ABOVE the marker are kept and excluded from the generated part. (Windows: eets_addr_win.h's generated
# block comes from Ghidra; the PE is stripped, so there is no nm equivalent.)
#   tools/gen-symbols.sh /path/to/Eets
set -euo pipefail
BIN="${1:?usage: gen-symbols.sh /path/to/Eets}"
HDR="$(cd "$(dirname "$0")/../include" && pwd)/eets_addr.h"
B='// === BEGIN GENERATED ==='; E='// === END GENERATED ==='
T=$(mktemp -d)
awk -v b="$B" 'index($0,b){exit} {print}' "$HDR" > "$T/top"
grep -oE 'uintptr_t +[A-Za-z0-9_]+' "$T/top" | awk '{print $NF}' | sort -u > "$T/exn"
grep -oE '= *0x[0-9a-f]+' "$T/top" | grep -oE '0x[0-9a-f]+' | tr 'A-F' 'a-f' | sort -u > "$T/exa"
nm -C --defined-only "$BIN" | awk 'toupper($2)~/^[TWI]$/&&$1~/^[0-9a-f]+$/{a=$1;$1=$2="";sub(/^ +/,"");print a"\t"$0}' \
| python3 - "$T/exn" "$T/exa" > "$T/dump" <<'PY'
import sys, re
KW=set("operator new delete int long void bool float double char short unsigned signed const volatile namespace template typename class struct union enum return if for while do switch case default this true false nullptr static inline virtual register auto dynamic_cast static_cast reinterpret_cast const_cast typeid sizeof goto try catch throw using friend and or not xor".split())
exn=set(open(sys.argv[1]).read().split()); exa=set(x.strip().lower() for x in open(sys.argv[2]))
seen={}; out=[]
for ln in sys.stdin:
    a,_,n=ln.partition('\t'); n=n.strip()
    if not n or ('0x'+a.lstrip('0')) in exa: continue
    i=re.sub(r'_+','_',re.sub(r'[^A-Za-z0-9_]','_',n)).strip('_')
    if not i or i[0].isdigit() or i in KW: i='f_'+i
    if i in exn: continue
    if i in seen: i='%s_%s'%(i,a)
    seen[i]=1; out.append((int(a,16),i))
out.sort()
for ad,i in out: print('constexpr uintptr_t %-46s = 0x%x;'%(i,ad))
PY
awk -v B="$B" -v E="$E" -v df="$T/dump" '$0==B{print; while((getline l<df)>0)print l; sk=1; next} $0==E{sk=0} !sk{print}' "$HDR" > "$HDR.new"
mv "$HDR.new" "$HDR"; rm -rf "$T"
echo "regenerated $(grep -c '^constexpr' "$HDR") constexpr entries in $HDR" >&2
