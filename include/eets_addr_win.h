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
