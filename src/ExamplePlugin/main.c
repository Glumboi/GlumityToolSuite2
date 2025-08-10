#include <GlumityLib.h>
#include "hooks.h"

// Example plugin for PC Building Simulator 2

GlumityV2Exports exports;
GlumityV2DumperExports dumperExports;

__declspec(dllexport) void GlumityMain(HMODULE glumityInternal)
{
    GlumityV2DumperExports_Init(&dumperExports);
    if (dumperExports.GlumityV2Dumper_GetFunctionPointer)
    {
        GlumityPlugin_printf("Found GlumityV2Dumper_GetFunctionPointer!\n", "Example Plugin");
    }
    void *ChangeCPUMultiplierPtr = dumperExports.GlumityV2Dumper_GetFunctionPointer("BiosConfig", "ChangeCPUMultiplier");
    if (ChangeCPUMultiplierPtr)
    {
        // Make a hook
        Hooks_Install(ChangeCPUMultiplierPtr);
    }
}