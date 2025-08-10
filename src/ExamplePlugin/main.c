#include <GlumityLib.h>
#include "hooks.h"

// Example plugin for PC Building Simulator 2

GlumityV2DumperExports dumperExports;

GLUMITYV2_PLUGIN_ENTRY
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

GLUMITYV2_PLUGIN_EXIT
{
    MH_DisableHook(MH_ALL_HOOKS);
}