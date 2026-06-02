#ifndef PLUGIN_H
#define PLUGIN_H

#include <Windows.h>

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define DLL_PATH_MAX 260

/// @brief entry point of plugins, passes a handle of glumity's loaded main dll (the loader)
typedef void (*GlumityPlugin_EntryPoint)();
typedef void (*GlumityPlugin_ExitPoint)();
typedef void (*GlumityGeneric_Func)();

#define GLUMITYV2_PLUGIN_ENTRY \
    EXPORT void GlumityMain()

#define GLUMITYV2_PLUGIN_EXIT \
    EXPORT void GlumityExit()

// Recommended to use this to run most code,
// specially function hooking since you have to rely on the dumper being loaded
#define GLUMITYV2_PLUGIN_THREADRUN(function, pParam)                                     \
    freopen("CONOUT$", "w", stdout); /* enable plugin printing */                        \
    HANDLE hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)function, pParam, 0, 0); \
    if (hThread)                                                                         \
        CloseHandle(hThread);

/// @brief Printf for glumity plugin specific code, uses malloc and free for a temp char*, caller doesn't need to manage
/// @param fmt
/// @param
void GlumityPlugin_printf(const char *fmt, const char *printHeaderInner, ...);

typedef struct
{
    char dllPath[DLL_PATH_MAX];
    char *name;
    GlumityPlugin_EntryPoint entryPoint;
    GlumityPlugin_ExitPoint exitPoint;

    HMODULE hDll;
} GlumityPlugin;

#endif