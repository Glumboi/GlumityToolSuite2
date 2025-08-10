#ifndef PLUGIN_H
#define PLUGIN_H

#include <Windows.h>

#include <string.h>
#include <stdint.h>

#define DLL_PATH_MAX 260

/// @brief entry point of plugins, passes a handle of glumity's loaded main dll (the loader)
typedef void(*GlumityPlugin_EntryPoint)(HMODULE glumityInternal);

typedef struct{
    char dllPath[DLL_PATH_MAX];
    GlumityPlugin_EntryPoint entryPoint;

    HMODULE hDll;
} GlumityPlugin;

GlumityPlugin GlumityPlugin_LoadPlugin(const char* dllPath);
void GlumityPlugin_FreePlugin(GlumityPlugin* plugin);

#endif