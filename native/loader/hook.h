// hook.h - minimal, safe inline x86-64 detour for hooking engine functions.
//
// Overwrites a function's prologue with a 14-byte absolute jump to a detour, and
// builds a trampoline (copied prologue + jump back) so the detour can still call
// the original. The length decoder ACCEPTS only instruction forms it can decode
// and relocate with certainty, and REJECTS everything else (returns 0) so a hook
// is simply refused rather than corrupting code. RIP-relative and relative
// call/jmp instructions in the prologue are rejected (cannot be relocated here).
#pragma once
#include <cstdint>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>

namespace eets_hook {

inline int modrm_len(const uint8_t* p);   // fwd

// Decode one instruction at p. Returns its length, or 0 if it cannot be safely
// decoded/relocated. Handles legacy prefixes, REX, and a prologue-common opcode
// subset with full ModRM/SIB/disp/imm sizing.
inline int insn_len(const uint8_t* p) {
	int len = 0;
	bool op66 = false;            // operand-size override
	// legacy prefixes
	for (;;) {
		uint8_t b = p[len];
		if (b == 0x66) { op66 = true; len++; continue; }
		if (b == 0x67 || b == 0xF0 || b == 0xF2 || b == 0xF3 ||
		    b == 0x2E || b == 0x36 || b == 0x3E || b == 0x26 || b == 0x64 || b == 0x65) { len++; continue; }
		break;
	}
	bool rexW = false;
	if ((p[len] & 0xF0) == 0x40) { rexW = (p[len] & 0x08) != 0; len++; }  // REX

	uint8_t op = p[len++];

	// two-byte opcode (0F ..) - accept a tiny known set (endbr64, common mov/movzx)
	if (op == 0x0F) {
		uint8_t op2 = p[len++];
		if (op2 == 0x1E) { len++; return len; }          // endbr64 / nop (modrm byte FA/FB)
		if (op2 == 0x1F) { /* multi-byte NOP: modrm follows */ }
		else if (op2 == 0xB6 || op2 == 0xB7 || op2 == 0xBE || op2 == 0xBF || op2 == 0x10 ||
		         op2 == 0x11 || op2 == 0x28 || op2 == 0x29 || op2 == 0x6E || op2 == 0x7E ||
		         op2 == 0xD6 || op2 == 0x57 || op2 == 0xEF) { /* has modrm */ }
		else return 0;                                    // unknown 2-byte op: refuse
		goto modrm;
	}

	// one-byte opcodes
	switch (op) {
		case 0x50: case 0x51: case 0x52: case 0x53:       // push r64
		case 0x54: case 0x55: case 0x56: case 0x57:
		case 0x58: case 0x59: case 0x5A: case 0x5B:       // pop r64
		case 0x5C: case 0x5D: case 0x5E: case 0x5F:
		case 0x90: case 0xC3: case 0xC9: case 0xCC:       // nop/ret/leave/int3
			return len;
		case 0x88: case 0x89: case 0x8A: case 0x8B:       // mov r/m<->r
		case 0x01: case 0x03: case 0x09: case 0x0B:       // add/or
		case 0x21: case 0x23: case 0x29: case 0x2B:       // and/sub
		case 0x31: case 0x33: case 0x39: case 0x3B:       // xor/cmp
		case 0x85: case 0x84:                             // test
		case 0x63:                                        // movsxd
			goto modrm;
		case 0x8D:                                        // lea (reject if RIP-relative)
			goto modrm;
		case 0x83:                                        // grp1 r/m, imm8
			{ int r = modrm_len(p + len); if (!r) return 0; len += r + 1; return len; }
		case 0x81:                                        // grp1 r/m, imm32
			{ int r = modrm_len(p + len); if (!r) return 0; len += r + (op66 ? 2 : 4); return len; }
		case 0xC6:                                        // mov r/m, imm8
			{ int r = modrm_len(p + len); if (!r) return 0; len += r + 1; return len; }
		case 0xC7:                                        // mov r/m, imm16/32
			{ int r = modrm_len(p + len); if (!r) return 0; len += r + (op66 ? 2 : 4); return len; }
		case 0xB8: case 0xB9: case 0xBA: case 0xBB:       // mov r, imm
		case 0xBC: case 0xBD: case 0xBE: case 0xBF:
			return len + (rexW ? 8 : (op66 ? 2 : 4));
		case 0x68:                                        // push imm32
			return len + 4;
		case 0x6A:                                        // push imm8
			return len + 1;
		default:
			return 0;                                     // unknown / relative jmp/call: refuse
	}

modrm:
	{
		int r = modrm_len(p + len);
		if (!r) return 0;
		len += r;
		return len;
	}
}

// length of ModRM(+SIB+disp). returns 0 if RIP-relative (cannot relocate here).
inline int modrm_len(const uint8_t* p) {
	uint8_t modrm = p[0];
	int mod = modrm >> 6, rm = modrm & 7;
	int n = 1;
	if (mod == 3) return n;                               // register direct
	if (rm == 4) {                                        // SIB
		uint8_t sib = p[1]; n++;
		int base = sib & 7;
		if (mod == 0 && base == 5) n += 4;               // disp32
	} else if (mod == 0 && rm == 5) {
		return 0;                                         // RIP-relative: refuse
	}
	if (mod == 1) n += 1;                                 // disp8
	else if (mod == 2) n += 4;                            // disp32
	return n;
}

constexpr int JMP_SIZE = 14;   // FF 25 00000000 + abs64

inline void write_abs_jmp(uint8_t* at, void* target) {
	at[0] = 0xFF; at[1] = 0x25;
	*(uint32_t*)(at + 2) = 0;
	*(uint64_t*)(at + 6) = (uint64_t)target;
}

inline bool unprotect(void* addr, size_t len) {
	long pg = sysconf(_SC_PAGESIZE);
	uintptr_t start = (uintptr_t)addr & ~(uintptr_t)(pg - 1);
	uintptr_t end = ((uintptr_t)addr + len + pg - 1) & ~(uintptr_t)(pg - 1);
	return mprotect((void*)start, end - start, PROT_READ | PROT_WRITE | PROT_EXEC) == 0;
}
inline void reprotect(void* addr, size_t len) {
	long pg = sysconf(_SC_PAGESIZE);
	uintptr_t start = (uintptr_t)addr & ~(uintptr_t)(pg - 1);
	uintptr_t end = ((uintptr_t)addr + len + pg - 1) & ~(uintptr_t)(pg - 1);
	mprotect((void*)start, end - start, PROT_READ | PROT_EXEC);
}

// Install a detour at `target`. On success *original receives a callable
// trampoline to the unhooked function. Returns false (no change) if the prologue
// can't be safely relocated.
inline bool install(void* target, void* detour, void** original) {
	uint8_t* t = (uint8_t*)target;
	int copied = 0;
	while (copied < JMP_SIZE) {
		int l = insn_len(t + copied);
		if (l <= 0) return false;                        // can't decode/relocate: refuse
		copied += l;
	}
	// trampoline: copied prologue + abs jmp back to (target + copied)
	uint8_t* tramp = (uint8_t*)mmap(nullptr, copied + JMP_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC,
	                                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (tramp == MAP_FAILED) return false;
	memcpy(tramp, t, copied);
	write_abs_jmp(tramp + copied, t + copied);

	// patch target prologue with jmp to detour
	if (!unprotect(t, JMP_SIZE)) { munmap(tramp, copied + JMP_SIZE); return false; }
	write_abs_jmp(t, detour);
	reprotect(t, JMP_SIZE);

	if (original) *original = tramp;
	return true;
}

} // namespace eets_hook
