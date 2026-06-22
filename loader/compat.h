// compat.h - POSIX->Win32 shims so loader.cpp stays one body across platforms.
// Linux pulls the real POSIX headers; Windows gets thin inline equivalents.
#pragma once

#ifndef _WIN32
// ---- Linux: the genuine articles ----
#include <dlfcn.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <link.h>
#include <csetjmp>
#include <csignal>
#include <ctime>

#else
// ---- Windows: minimal shims over Win32 ----
#include <windows.h>
#include <direct.h>
#include <sys/stat.h>     // mingw provides struct stat / stat()
#include <ctime>          // mingw provides struct timespec / clock_gettime via winpthreads
#include <csetjmp>        // setjmp/longjmp for the crash guard (VEH-driven)
#include <string>
#include <cstring>

// --- dlfcn ---
#define RTLD_NOW    0x0
#define RTLD_GLOBAL 0x0
#define RTLD_LAZY   0x0
#define RTLD_NEXT   ((void*)-1)
namespace eets_compat {
    // the game's interposed exports live in these DLLs; RTLD_NEXT means "the real one".
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
inline int eets_mkdir(const char* p, int) { return _mkdir(p); }      // 2-arg POSIX shape
#define mkdir(p, mode) eets_mkdir((p), (mode))
inline int unsetenv(const char* n) { std::string s = std::string(n) + "="; return _putenv(s.c_str()); }
inline char* getcwd_compat(char* b, size_t n) { return _getcwd(b, (int)n); }
#define getcwd(b, n) getcwd_compat((b), (n))
inline FILE* popen_compat(const char* c, const char* m) { return _popen(c, m); }
inline int   pclose_compat(FILE* f) { return _pclose(f); }
#define popen(c, m) popen_compat((c), (m))
#define pclose(f)   pclose_compat((f))

#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 1
inline int clock_gettime(int, struct timespec* ts) {
    static LARGE_INTEGER freq; static BOOL have = QueryPerformanceFrequency(&freq);
    LARGE_INTEGER c; QueryPerformanceCounter(&c);
    ts->tv_sec  = (time_t)(c.QuadPart / freq.QuadPart);
    ts->tv_nsec = (long)((c.QuadPart % freq.QuadPart) * 1000000000ll / freq.QuadPart);
    return have ? 0 : 0;
}
#endif

// directory this DLL lives in (the game folder) - replaces dladdr on Linux.
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
