#pragma once
#include <cstdint>
#include "loader/platform.h"

namespace Eets { namespace addr {

// Windows is not wired up yet - HELP WANTED. To port: regenerate these RVAs (offsets
// from the PE image base) for a Windows Eets build using the same symbol set as
// tools/gen_engine_header.sh, fill out the full mirror of eets_addr.h below, and finish
// the Win32 branch in loader/platform.h. A Windows Eets binary is needed to extract the
// offsets. PRs welcome.
namespace rva {
	constexpr uintptr_t World_GetGravity   = 0x0;  // TODO
	constexpr uintptr_t World_SetGravity   = 0x0;  // TODO
	constexpr uintptr_t World_CreateObject = 0x0;  // TODO
	// ... full set mirrors eets_addr.h ...
}

// resolved at runtime: base + rva
inline uintptr_t resolve(uintptr_t r) { return r ? plat::module_base() + r : 0; }

}} // namespace Eets::addr
