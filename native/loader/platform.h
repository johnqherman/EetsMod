// platform.h - OS abstraction seam for the loader (Linux done; Windows scaffold).
//
// The loader's core (mod discovery, compile/dlopen, dispatch, events, config,
// UI) is platform-independent. Only these primitives differ. loader.cpp uses the
// POSIX calls directly today; a Windows build routes them here.
#pragma once

#ifdef _WIN32
  #include <windows.h>
  #define EETS_EXPORT extern "C" __declspec(dllexport)
  namespace plat {
    // injection: ship as a proxy DLL (e.g. version.dll/winmm.dll next to
    // Eets.exe) whose DllMain spawns the loader; or a launcher that injects.
    inline void* load_library(const char* p) { return (void*)LoadLibraryA(p); }
    inline void* get_symbol(void* h, const char* n) { return (void*)GetProcAddress((HMODULE)h, n); }
    // "next" symbol: resolve the real FNA3D/SDL2 export from their module
    // (we IAT- or inline-hook the imports rather than LD_PRELOAD-interpose).
    inline void* real_symbol(const char* mod, const char* n) {
      HMODULE m = GetModuleHandleA(mod); return m ? (void*)GetProcAddress(m, n) : nullptr;
    }
    inline uintptr_t module_base() { return (uintptr_t)GetModuleHandleA(nullptr); }  // PE base for RVA
  }
#else
  #include <dlfcn.h>
  #define EETS_EXPORT extern "C" __attribute__((visibility("default")))
  namespace plat {
    inline void* load_library(const char* p) { return dlopen(p, RTLD_NOW | RTLD_GLOBAL); }
    inline void* get_symbol(void* h, const char* n) { return dlsym(h, n); }
    inline void* real_symbol(const char*, const char* n) { return dlsym(RTLD_NEXT, n); }
    inline uintptr_t module_base() { return 0; }   // non-PIE: addresses are absolute
  }
#endif
