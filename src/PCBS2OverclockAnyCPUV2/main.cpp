#include <GlumityLib.h>
#include <stdbool.h>

#include <IL2CPPResolver/IL2CPP_Resolver.hpp>

#define MY_PLUGIN "PCBS2OverclockAnyCPUV2"

GlumityV2DumperExports exports = {0};

typedef bool (*BiosConfig_ChangeCPUMultiplier_t)(DWORD *, int32_t, const DWORD *);
BiosConfig_ChangeCPUMultiplier_t BiosConfig_ChangeCPUMultiplier_o;

bool BiosConfig_ChangeCPUMultiplier_hook(DWORD *__this, int32_t dir, const DWORD *method)
{
    Unity::CComponent *caller = (Unity::CComponent *)__this;
    float cpuMultiplier = caller->GetMemberValue<float>("m_cpuMultiplier");
#ifdef __DEBUG
    GlumityPlugin_printf("cpu multiplier: %d dir: %d\n", MY_PLUGIN, cpuMultiplier, dir);
#endif
    if (dir == 1) // Incresase
    {
        cpuMultiplier += 0.25f;
    }
    else if (dir == -1) // Decrease
    {
        cpuMultiplier -= 0.25f;
    }

    caller->SetMemberValue<float>("m_cpuMultiplier", cpuMultiplier);
    return true;
}

void MakeHooks()
{
    GLUMITYV2_DUMPER_WAITFOR_INIT(exports);
    GLUMITYV2_PLUGIN_INIT_IL2CPP
    {
        GLUMITYV2_INIT_HOOKING(MY_PLUGIN, return)
        {
            GlumityPlugin_printf("Initialized Minhook\n", MY_PLUGIN);
        }

        BiosConfig_ChangeCPUMultiplier_o = (BiosConfig_ChangeCPUMultiplier_t)GLUMITYV2_DUMPER_GET_GAME_FUNCTION("BiosConfig", "ChangeCPUMultiplier", exports);

        if (!BiosConfig_ChangeCPUMultiplier_o)
        {
            GlumityPlugin_printf("Failed to get ChangeCPUMultiplier function!\n", MY_PLUGIN);
            return;
        }

        GLUMITYV2_GAME_HOOK_CREATE(
            "BiosConfig_ChangeCPUMultiplier",
            BiosConfig_ChangeCPUMultiplier_o,
            BiosConfig_ChangeCPUMultiplier_hook);

        GLUMITYV2_GAME_HOOK_ENABLE_ALL(MY_PLUGIN);
    }
}

GLUMITYV2_PLUGIN_ENTRY
{
    GLUMITYV2_PLUGIN_THREADRUN(MakeHooks, 0);
}

GLUMITYV2_PLUGIN_EXIT
{
}