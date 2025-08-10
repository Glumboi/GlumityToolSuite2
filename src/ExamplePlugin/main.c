#include <GlumityLib.h>
#include "hooks.h"

// Example plugin for PC Building Simulator 2

GlumityV2Exports exports;
GlumityV2DumperExports dumperExports;

__declspec(dllexport) void GlumityMain(HMODULE glumityInternal)
{
    GlumityV2Exports_Init(&exports, glumityInternal);
    GlumityPluginLoader *loader = exports.GetLoaderInstance();
    if (loader)
    {
        GlumityPlugin_printf("Found loader\nTotal plugins loaded: %d\n", "Example Plugin", loader->pluginsCount);
    }

    GlumityV2DumperExports_Init(&dumperExports);
    if (dumperExports.GlumityV2Dumper_GetFunctionPointer)
    {
        GlumityPlugin_printf("Found GlumityV2Dumper_GetFunctionPointer!\n", "Example Plugin");
    }

    void *ptr = dumperExports.GlumityV2Dumper_GetFunctionPointer("BiosConfig", "ChangeCPUMultiplier");
    if (ptr)
    {

        // Make a hook
        GlumityPlugin_printf("ChangeCPUMultiplier!\n", "Example Plugin");
        Hooks_Install(ptr);
    }
}