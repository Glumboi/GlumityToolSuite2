#ifndef GLUMITYLIB_H
#define GLUMITYLIB_H

#include <windows.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "../MainLoader/PluginLoader.h"
#include "../MainLoader/Plugin.h"

    /// @brief Printf for glumity plugin specific code, uses malloc and free for a temp char*, caller doesn't need to manage
    /// @param fmt
    /// @param
    void GlumityPlugin_printf(const char *fmt, const char *printHeaderInner, ...);

    /// @brief entry point of plugins, passes a handle of glumity's loaded main dll (the loader)
    typedef void (*GlumityPlugin_EntryPoint)(HMODULE glumityInternal);

    // Exports of MainLoader
    typedef GlumityPluginLoader *(*GetLoaderInstance_t)();
    typedef struct
    {
        GetLoaderInstance_t GetLoaderInstance;
    } GlumityV2Exports;

#define INIT_GLUMITYV2_EXPORT(mod, exportName, exportType) \
    (exportType) GetProcAddress(mod, exportName);

    void GlumityV2Exports_Init(GlumityV2Exports *exports, HMODULE mod);

#ifdef __cplusplus
}
#endif

#endif