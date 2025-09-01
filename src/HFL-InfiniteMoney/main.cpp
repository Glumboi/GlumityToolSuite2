#include <GlumityLib.h>
#include <stdbool.h>

#include <IL2CPP_Resolver.hpp>

#define MY_PLUGIN "HFL-InfiniteMoney"

GlumityV2DumperExports exports = {0};

GLUMITYV2_GAME_HOOK_TYPE(void, Empyrean_HouseFlipper2_Budget_SetBudget_t)(DWORD *, int32_t, const DWORD *);
Empyrean_HouseFlipper2_Budget_SetBudget_t Empyrean_HouseFlipper2_Budget_SetBudget_o;
void Empyrean_HouseFlipper2_Budget_SetBudget_hook(DWORD *__this, int32_t newValue, const DWORD *method)
{
    newValue = 1.000000;
}

GLUMITYV2_GAME_HOOK_TYPE(void, Empyrean_HouseFlipper2_MoneyBalance_SetPlayerBudget_t)(DWORD *, int32_t, const DWORD *);
Empyrean_HouseFlipper2_MoneyBalance_SetPlayerBudget_t Empyrean_HouseFlipper2_MoneyBalance_SetPlayerBudget_o;
void Empyrean_HouseFlipper2_MoneyBalance_SetPlayerBudget_hook(DWORD *__this, int32_t newPlayerBudget, const DWORD *method)
{
    newPlayerBudget = 1.000000;
}

GLUMITYV2_GAME_HOOK_TYPE(void, Empyrean_HouseFlipper2_MoneyBalance_DecreaseBudget_t)(DWORD *, int32_t, const DWORD *);
Empyrean_HouseFlipper2_MoneyBalance_DecreaseBudget_t Empyrean_HouseFlipper2_MoneyBalance_DecreaseBudget_o;
void Empyrean_HouseFlipper2_MoneyBalance_DecreaseBudget_hook(DWORD *__this, int32_t moneyToSpend, const DWORD *method)
{
    moneyToSpend = 1.000000;
}

void MakeHooks()
{
    GLUMITYV2_DUMPER_WAITFOR_INIT(exports);

    // Needed if using il2cpp within hooks, can also do hooks without but limited functionality, il2cppresolver requires c++
    GLUMITYV2_PLUGIN_INIT_IL2CPP
    {
        GLUMITYV2_INIT_HOOKING(MY_PLUGIN, return)
        {
            GlumityPlugin_printf("Initialized Minhook\n", MY_PLUGIN);
        }

        Empyrean_HouseFlipper2_Budget_SetBudget_o = (Empyrean_HouseFlipper2_Budget_SetBudget_t)GLUMITYV2_DUMPER_GET_GAME_FUNCTION("Budget", "SetBudget", exports);
        if (!Empyrean_HouseFlipper2_Budget_SetBudget_o)
        {
            GlumityPlugin_printf("Failed to get SetBudget function!\n", MY_PLUGIN);
            return;
        }

        GLUMITYV2_GAME_HOOK_CREATE(
            "Empyrean_HouseFlipper2_Budget_SetBudget",
            Empyrean_HouseFlipper2_Budget_SetBudget_o,
            Empyrean_HouseFlipper2_Budget_SetBudget_hook);

        Empyrean_HouseFlipper2_MoneyBalance_SetPlayerBudget_o = (Empyrean_HouseFlipper2_MoneyBalance_SetPlayerBudget_t)GLUMITYV2_DUMPER_GET_GAME_FUNCTION("MoneyBalance", "SetPlayerBudget", exports);
        if (!Empyrean_HouseFlipper2_MoneyBalance_SetPlayerBudget_o)
        {
            GlumityPlugin_printf("Failed to get SetPlayerBudget function!\n", MY_PLUGIN);
            return;
        }

        GLUMITYV2_GAME_HOOK_CREATE(
            "Empyrean_HouseFlipper2_MoneyBalance_SetPlayerBudget",
            Empyrean_HouseFlipper2_MoneyBalance_SetPlayerBudget_o,
            Empyrean_HouseFlipper2_MoneyBalance_SetPlayerBudget_hook);

        Empyrean_HouseFlipper2_MoneyBalance_DecreaseBudget_o = (Empyrean_HouseFlipper2_MoneyBalance_DecreaseBudget_t)GLUMITYV2_DUMPER_GET_GAME_FUNCTION("MoneyBalance", "DecreaseBudget", exports);
        if (!Empyrean_HouseFlipper2_MoneyBalance_DecreaseBudget_o)
        {
            GlumityPlugin_printf("Failed to get SetPlayerBudget function!\n", MY_PLUGIN);
            return;
        }

        GLUMITYV2_GAME_HOOK_CREATE(
            "Empyrean_HouseFlipper2_MoneyBalance_DecreaseBudget",
            Empyrean_HouseFlipper2_MoneyBalance_DecreaseBudget_o,
            Empyrean_HouseFlipper2_MoneyBalance_DecreaseBudget_hook);

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