// eets_addr_win.h - Windows address skeleton (NOT yet filled).
//
// The Windows build of Eets is a different binary with different addresses, and
// PE images are ASLR-relocated, so Windows uses base + RVA (unlike the Linux
// non-PIE absolutes in eets_addr.h).
//
// TO FILL: run gen_engine_header.sh against the Windows Eets.exe in Ghidra,
// convert each absolute to an RVA (addr - image_base), and paste below. Then the
// loader resolves real addresses as plat::module_base() + rva.
#pragma once
#include <cstdint>
#include "loader/platform.h"

namespace Eets { namespace addr {

// RVAs (offsets from the PE image base) - PLACEHOLDERS, regenerate per build.
namespace rva {
	constexpr uintptr_t World_GetGravity   = 0x0;  // TODO
	constexpr uintptr_t World_SetGravity   = 0x0;  // TODO
	constexpr uintptr_t World_CreateObject = 0x0;  // TODO
	// ... full set mirrors eets_addr.h ...
}

// resolved at runtime: base + rva
inline uintptr_t resolve(uintptr_t r) { return r ? plat::module_base() + r : 0; }

}} // namespace Eets::addr
