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
    if (MH_Initialize() != MH_OK)
    {
        GlumityPlugin_printf("Failed to init minhook!\n", "Example Plugin");
        return;
    }

    BiosConfig_ChangeCPUMultiplier_o = (BiosConfig_ChangeCPUMultiplier_t)BiosConfig_ChangeCPUMultiplier_ptr;
    MH_STATUS stat = MH_CreateHook(
        (LPVOID *)(BiosConfig_ChangeCPUMultiplier_o),
        &BiosConfig_ChangeCPUMultiplier_hook,
        (LPVOID *)&BiosConfig_ChangeCPUMultiplier_o);

    if (stat != MH_OK)
    {
        GlumityPlugin_printf("Failed to init BiosConfig_ChangeCPUMultiplier_hook!\n", "Example Plugin");
        return;
    }

    stat = MH_EnableHook(MH_ALL_HOOKS);
    if (stat != MH_OK)
    {
        GlumityPlugin_printf("Failed to enable BiosConfig_ChangeCPUMultiplier_hook!\n", "Example Plugin");
    }

        GlumityPlugin_printf("Created and enabled BiosConfig_ChangeCPUMultiplier_hook!\n", "Example Plugin");

}
