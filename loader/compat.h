// compat.h - POSIX->Win32 shims so loader.cpp stays one body across platforms
#pragma once

#ifndef _WIN32
#include <dlfcn.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <link.h>
#include <csetjmp>
#include <csignal>
#include <ctime>

// --- glibc 2.38 C23 symbol shim (Steam Linux Runtime compatibility) ---
// GCC 13+/glibc 2.38 redirect strtol/strtoul/sscanf to __isoc23_* variants the Steam Linux Runtime's
// older glibc does NOT provide - so a loader (or mod) built on a modern toolchain fails to preload/
// dlopen inside the runtime container (silent "symbol not found"). Define those symbols here, exported
// from the loader so dlopen'd mods resolve them too, routed to the long-standing glibc versions. Only
// compiled when built against glibc >= 2.38 (older toolchains already emit the legacy symbols).
#if defined(__GLIBC__) && defined(__GLIBC_PREREQ)
#if __GLIBC_PREREQ(2, 38)
#include <cstdarg>
__asm__(".symver eets_strtol,   strtol@GLIBC_2.2.5");
__asm__(".symver eets_strtoul,  strtoul@GLIBC_2.2.5");
__asm__(".symver eets_strtoll,  strtoll@GLIBC_2.2.5");
__asm__(".symver eets_strtoull, strtoull@GLIBC_2.2.5");
__asm__(".symver eets_strtod,   strtod@GLIBC_2.2.5");
__asm__(".symver eets_vsscanf,  __isoc99_vsscanf@GLIBC_2.7");
extern "C" long               eets_strtol(const char*, char**, int);
extern "C" unsigned long      eets_strtoul(const char*, char**, int);
extern "C" long long          eets_strtoll(const char*, char**, int);
extern "C" unsigned long long eets_strtoull(const char*, char**, int);
extern "C" double             eets_strtod(const char*, char**);
extern "C" int                eets_vsscanf(const char*, const char*, va_list);
#define EETS_C23SHIM extern "C" __attribute__((visibility("default")))
EETS_C23SHIM long               __isoc23_strtol(const char* p, char** e, int b)   { return eets_strtol(p, e, b); }
EETS_C23SHIM unsigned long      __isoc23_strtoul(const char* p, char** e, int b)  { return eets_strtoul(p, e, b); }
EETS_C23SHIM long long          __isoc23_strtoll(const char* p, char** e, int b)  { return eets_strtoll(p, e, b); }
EETS_C23SHIM unsigned long long __isoc23_strtoull(const char* p, char** e, int b) { return eets_strtoull(p, e, b); }
EETS_C23SHIM double             __isoc23_strtod(const char* p, char** e)          { return eets_strtod(p, e); }
EETS_C23SHIM int __isoc23_sscanf(const char* s, const char* f, ...) { va_list a; va_start(a, f); int r = eets_vsscanf(s, f, a); va_end(a); return r; }
#endif
#endif

#else
#include <windows.h>
#include <direct.h>
#include <sys/stat.h>
#include <ctime>
#include <csetjmp>        // setjmp/longjmp for VEH-driven crash guard
#include <string>
#include <cstring>

// --- dlfcn ---
#define RTLD_NOW    0x0
#define RTLD_GLOBAL 0x0
#define RTLD_LAZY   0x0
#define RTLD_NEXT   ((void*)-1)
namespace eets_compat {
    // RTLD_NEXT resolves the real (un-hooked) export from these interposed DLLs
    static const char* const kNextMods[] = { "FNA3D.dll", "SDL2.dll", "FAudio.dll" };
}
inline void* dlopen(const char* path, int) { return (void*)LoadLibraryA(path); }
inline void* dlsym(void* h, const char* name) {
    if (h == RTLD_NEXT) {
        for (const char* m : eets_compat::kNextMods) {
            HMODULE mod = GetModuleHandleA(m);
            if (mod) { void* f = (void*)GetProcAddress(mod, name); if (f) return f; }
        }
        return nullptr;
    }
    return (void*)GetProcAddress((HMODULE)h, name);
}
inline int dlclose(void* h) { return FreeLibrary((HMODULE)h) ? 0 : -1; }
inline const char* dlerror() {
    static char buf[256];
    DWORD e = GetLastError();
    if (!e) return nullptr;
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   nullptr, e, 0, buf, sizeof(buf), nullptr);
    return buf;
}

// --- dirent ---
struct dirent { char d_name[MAX_PATH]; };
struct DIR { HANDLE h; WIN32_FIND_DATAA fd; bool first; dirent ent; };
inline DIR* opendir(const char* path) {
    DIR* d = new DIR();
    std::string spec = std::string(path) + "\\*";
    d->h = FindFirstFileA(spec.c_str(), &d->fd);
    if (d->h == INVALID_HANDLE_VALUE) { delete d; return nullptr; }
    d->first = true;
    return d;
}
inline dirent* readdir(DIR* d) {
    if (!d->first) { if (!FindNextFileA(d->h, &d->fd)) return nullptr; }
    d->first = false;
    strncpy(d->ent.d_name, d->fd.cFileName, MAX_PATH - 1);
    d->ent.d_name[MAX_PATH - 1] = 0;
    return &d->ent;
}
inline int closedir(DIR* d) { if (d) { FindClose(d->h); delete d; } return 0; }

// --- misc POSIX ---
inline int eets_mkdir(const char* p, int) { return _mkdir(p); }      // drop POSIX mode arg
#define mkdir(p, mode) eets_mkdir((p), (mode))
inline int unsetenv(const char* n) { std::string s = std::string(n) + "="; return _putenv(s.c_str()); }
inline char* getcwd_compat(char* b, size_t n) { return _getcwd(b, (int)n); }
#define getcwd(b, n) getcwd_compat((b), (n))
inline FILE* popen_compat(const char* c, const char* m) { return _popen(c, m); }
inline int   pclose_compat(FILE* f) { return _pclose(f); }
#define popen(c, m) popen_compat((c), (m))
#define pclose(f)   pclose_compat((f))

// macro-route clock_gettime through QPC: never link the real symbol (libwinpthread, declared by
// newer mingw but not auto-linked -> "undefined reference to clock_gettime")
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 1
#endif
inline int clock_gettime_compat(int, struct timespec* ts) {
    static LARGE_INTEGER freq; static BOOL have = QueryPerformanceFrequency(&freq);
    LARGE_INTEGER c; QueryPerformanceCounter(&c);
    ts->tv_sec  = (time_t)(c.QuadPart / freq.QuadPart);
    ts->tv_nsec = (long)((c.QuadPart % freq.QuadPart) * 1000000000ll / freq.QuadPart);
    (void)have; return 0;
}
#define clock_gettime(c, t) clock_gettime_compat((c), (t))

// directory this DLL lives in (game folder); replaces dladdr
inline std::string eets_self_dir() {
    HMODULE m = nullptr;
    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                       (LPCSTR)&eets_self_dir, &m);
    char buf[MAX_PATH] = {0};
    GetModuleFileNameA(m, buf, MAX_PATH);
    std::string p = buf; size_t s = p.find_last_of("/\\");
    return s != std::string::npos ? p.substr(0, s) : std::string(".");
}

#endif // _WIN32
