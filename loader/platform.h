// platform.h
#pragma once

#ifdef _WIN32
  #include <windows.h>
  #define EETS_EXPORT extern "C" __declspec(dllexport)
  namespace plat {
    // injection via proxy DLL (version.dll/winmm.dll) or launcher
    inline void* load_library(const char* p) { return (void*)LoadLibraryA(p); }
    inline void* get_symbol(void* h, const char* n) { return (void*)GetProcAddress((HMODULE)h, n); }
    // real export: we IAT/inline-hook, not LD_PRELOAD, so resolve it directly
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
