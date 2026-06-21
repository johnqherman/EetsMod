# Windows port plan

The framework is Linux-only today. This is the concrete plan to support the
Windows build of Eets. (Not yet implemented — the dev/test machine is Linux.)

## What stays the same
- The architecture: a preloaded native module that interposes the renderer's
  per-frame present + the input pump, `dlopen`/`LoadLibrary`s mod binaries, and
  calls engine functions at fixed addresses.
- The mod API (`eetsmod.h`), UI (`eets_ui.h`), events, config, save data — all
  platform-independent C++. Mods recompile per platform; source is portable.

## What changes

### 1. Injection (no `LD_PRELOAD`)
Windows has no `LD_PRELOAD`. Options, easiest first:
- **DLL proxy / "winmm.dll" or "version.dll" shim** next to `Eets.exe`: a stub
  DLL the game auto-loads that forwards exports to the real system DLL and runs
  our loader from `DllMain`. This is the standard, launcher-free approach (works
  with the Steam launch with no options).
- **Steam launch wrapper** that injects (CreateRemoteThread) — more fragile.

### 2. Interposition -> inline hooks
PLT interposition is Linux-only. On Windows, install **IAT hooks** (patch the
import table for `FNA3D_SwapBuffers` / `SDL_PollEvent`) or reuse the inline
detour engine (`hook.h`) to hook them directly. The detour engine is already
cross-platform x86-64; only page-protection calls differ (`VirtualProtect`
instead of `mprotect`) — abstract those behind 2-3 `#ifdef _WIN32` helpers.

### 3. Addresses
The Windows build is a different binary -> different addresses, and PE images are
ASLR-relocated. So:
- Resolve a module base at runtime (`GetModuleHandle(NULL)`) and store engine
  addresses as **RVAs** (offsets), not absolute (the Linux build is non-PIE so we
  use absolutes; Windows needs base + RVA).
- Generate a Windows `eets_addr_win.h` from the PE with the same Ghidra script;
  select the header by platform + BuildID (the BuildID/version layer already
  exists for detection).

### 4. Compiler for runtime mod builds
End users lack a compiler. On Windows, ship **prebuilt `.dll` mods** in releases
rather than compiling `.cpp` at launch (the auto-compile stays a dev convenience
where a toolchain is present). The loader already supports loading prebuilt
binaries directly.

### 5. ABI
MSVC vs the engine's compiler: match calling convention. Most engine binding
statics are `__cdecl`/`__fastcall`; `Vector2`-by-value and `Colour`-by-value
need per-ABI verification on the Windows build (same method as Linux: decompile,
confirm register/stack placement).

## Effort
Roughly: proxy-DLL + VirtualProtect abstraction + Windows address header + IAT/
inline hook of the two functions. The mod-facing API is unchanged. A Mac port
would follow the same shape (`DYLD_INSERT_LIBRARIES`, Mach-O addresses).
