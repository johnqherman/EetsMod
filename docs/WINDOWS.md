# Windows port plan

The framework is Linux-only today. This is the concrete plan to support the
Windows build of Eets. (Not yet implemented - the dev/test machine is Linux.)

## What stays the same
- The architecture: a preloaded native module that interposes the renderer's
  per-frame present + the input pump, `dlopen`/`LoadLibrary`s mod binaries, and
  calls engine functions at fixed addresses.
- The mod API (`eetsmod.h`), UI (`eets_ui.h`), events, config, save data - all
  platform-independent C++. Mods recompile per platform; source is portable.

## What changes

### 1. Injection (no `LD_PRELOAD`)
Windows has no `LD_PRELOAD`. Options, easiest first:
- **DLL proxy / "winmm.dll" or "version.dll" shim** next to `Eets.exe`: a stub
  DLL the game auto-loads that forwards exports to the real system DLL and runs
  our loader from `DllMain`. This is the standard, launcher-free approach (works
  with the Steam launch with no options).
- **Steam launch wrapper** that injects (CreateRemoteThread) - more fragile.

### 2. Interposition -> inline hooks
PLT interposition is Linux-only. On Windows, install **IAT hooks** (patch the
import table for `FNA3D_SwapBuffers` / `SDL_PollEvent`) or reuse the inline
detour engine (`hook.h`) to hook them directly. The detour engine is already
cross-platform x86-64; only page-protection calls differ (`VirtualProtect`
instead of `mprotect`) - abstract those behind 2-3 `#ifdef _WIN32` helpers.

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

## Status (what's scaffolded)
Done / portable:
- `loader/hook.h` is **cross-platform** - page protection + executable alloc
  abstracted (`mprotect`/`mmap` vs `VirtualProtect`/`VirtualAlloc`); the x86-64
  length decoder + detour logic are shared. (Validated on Linux.)
- `loader/platform.h` - the OS seam: `load_library`/`get_symbol`/`real_symbol`/
  `module_base` + `EETS_EXPORT`, with Windows and POSIX implementations.
- `include/eets_addr_win.h` - RVA-based address skeleton + `resolve(rva)` =
  `module_base() + rva`.

TODO (needs a Windows box + the Windows `Eets.exe`):
1. Generate Windows addresses: run `gen_engine_header.sh` on `Eets.exe` in
   Ghidra, convert to RVAs, fill `eets_addr_win.h`.
2. Route `loader.cpp`'s `dlopen`/`dlsym`/`RTLD_NEXT` through `platform.h`, and the
   `FNA3D_SwapBuffers`/`SDL_PollEvent` interposition through an IAT or `hook.h`
   inline hook (no `LD_PRELOAD` on Windows).
3. Ship as a proxy DLL (`version.dll`/`winmm.dll` next to `Eets.exe`) whose
   `DllMain` starts the loader; mods ship as prebuilt `.dll` (no runtime compile).
4. Build with MSVC or mingw-w64; verify the `Vector2`/`Colour` by-value ABI on
   the Windows build.

Cannot be done on this machine: no Windows, no mingw-w64 cross-compiler, and no
Windows `Eets.exe` to reverse. The above makes the codebase Windows-ready; the
remaining work is mechanical once those are available.
