#include <GlumityLib.h>
#include "hooks.h"

// Example plugin for PC Building Simulator 2

GlumityV2DumperExports dumperExports;

void MakeHooks()
{
    // Wait for dumper to finish initializing, crucial to avoid crashes
    GLUMITYV2_DUMPER_WAITFOR_INIT(dumperExports);
    void *ChangeCPUMultiplierPtr =
        GLUMITYV2_DUMPER_GET_GAME_FUNCTION("BiosConfig", "ChangeCPUMultiplier", dumperExports);

    if (ChangeCPUMultiplierPtr)
    {
        // Make a hook
        Hooks_Install(ChangeCPUMultiplierPtr);
    }
}

GLUMITYV2_PLUGIN_ENTRY
{
    GLUMITYV2_PLUGIN_THREADRUN(MakeHooks, 0);
}

GLUMITYV2_PLUGIN_EXIT
{
    MH_DisableHook(MH_ALL_HOOKS);
}