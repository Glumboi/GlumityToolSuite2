#include <GlumityLib.h>
#include <stdbool.h>

#include <IL2CPP_Resolver.hpp>

#define MY_PLUGIN "PCBS2CheatsV2"

GlumityV2DumperExports exports = {0};

GLUMITYV2_GAME_HOOK_TYPE(void, CareerStatus_State_OnDeserialization_t)(void *, void *, const IL2CPP_MethodInfo *);
CareerStatus_State_OnDeserialization_t CareerStatus_State_OnDeserialization_o;
void __stdcall CareerStatus_State_OnDeserialization_hook(DWORD *__this, DWORD *sender, const IL2CPP_MethodInfo *method)
{
    CareerStatus_State_OnDeserialization_o(__this, sender, method);
    GlumityPlugin_printf("CareerStatus_State_OnDeserialization!\n", MY_PLUGIN);
}

void Setup()
{
    GLUMITYV2_DUMPER_WAITFOR_INIT(exports);

    // Needed if using il2cpp within hooks, can also do hooks without but limited functionality, il2cppresolver requires c++
    GLUMITYV2_PLUGIN_INIT_IL2CPP
    {
        GLUMITYV2_INIT_HOOKING(MY_PLUGIN, return)
        {
            GlumityPlugin_printf("Initialized Minhook\n", MY_PLUGIN);
        }

        CareerStatus_State_OnDeserialization_o = (CareerStatus_State_OnDeserialization_t)GLUMITYV2_DUMPER_GET_GAME_FUNCTION("CareerStatus", "OnDeserialization", exports);
        if (!CareerStatus_State_OnDeserialization_o)
        {
            GlumityPlugin_printf("Failed to get CareerStatus_State_OnDeserialization function!\n", MY_PLUGIN);
            return;
        }

        GLUMITYV2_GAME_HOOK_CREATE(
            "CareerStatus_State_OnDeserialization",
            CareerStatus_State_OnDeserialization_o,
            CareerStatus_State_OnDeserialization_hook);
    }
}

GLUMITYV2_PLUGIN_ENTRY
{
    GLUMITYV2_PLUGIN_THREADRUN(Setup, 0);
}

GLUMITYV2_PLUGIN_EXIT
{
}