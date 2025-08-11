#pragma once

#include <stdint.h>
#include <GlumityLib.h>
#include <Minhook.h>
#include <stdbool.h>

typedef bool (*BiosConfig_ChangeCPUMultiplier_t)(void *, int32_t, const void *);
BiosConfig_ChangeCPUMultiplier_t BiosConfig_ChangeCPUMultiplier_o;

bool BiosConfig_ChangeCPUMultiplier_hook(void *__this, int32_t dir, const void *method)
{
    GlumityPlugin_printf("BiosConfig_ChangeCPUMultiplier_hook called!\n", "Example Plugin");
    return BiosConfig_ChangeCPUMultiplier_o(__this, dir, method);
}

void Hooks_Install(void *BiosConfig_ChangeCPUMultiplier_ptr)
{
    GLUMITYV2_INIT_HOOKING("Example Plugin", return)
    {
        GlumityPlugin_printf("Initialized Minhook\n", "Example Plugin");
    }

    BiosConfig_ChangeCPUMultiplier_o = (BiosConfig_ChangeCPUMultiplier_t)BiosConfig_ChangeCPUMultiplier_ptr;

    GLUMITYV2_GAME_HOOK_CREATE(
        "BiosConfig_ChangeCPUMultiplier",
        BiosConfig_ChangeCPUMultiplier_o,
        BiosConfig_ChangeCPUMultiplier_hook);

    GLUMITYV2_GAME_HOOK_ENABLE_ALL("Example Plugin");
}
