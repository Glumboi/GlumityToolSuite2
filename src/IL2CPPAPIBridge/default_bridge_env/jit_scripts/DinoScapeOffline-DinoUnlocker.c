#include <GlumityLib/GlumityLib.h>

#define MY_PLUGIN "DinoScapeOffline-DinoUnlocker"

GlumityV2DumperExports dumperExports;
struct DinoInfo;

GLUMITYV2_GAME_HOOK_TYPE(void*, DinoInfo_CheckIfOwned_t)(struct DinoInfo*);
DinoInfo_CheckIfOwned_t DinoInfo_CheckIfOwned_o;

void* DinoInfo_CheckIfOwned_hook(struct DinoInfo* _this)
{
	char* owned = (char*)_this;
	owned =  owned+0x51;
	*owned = 1;
	return 1;
}

// Installation:
// Current pointer address found: 0x00007FF91887ACF0

VOID OnHookInitFail()
{
    GLUMITY_PRINT_COLOR(CON_RED, "Failed to initialize Minhook!\n", MY_PLUGIN); 
}

VOID Setup()
{
    GLUMITYV2_VERIFY_DEPENDENCY("GlumityV2IL2CPPDumper");
    GLUMITYV2_DUMPER_WAITFOR_INIT(dumperExports);

    DinoInfo_CheckIfOwned_o = (DinoInfo_CheckIfOwned_t)dumperExports.GlumityV2Dumper_GetFunctionPointer("Assembly-CSharp", "DinoInfo", "CheckIfOwned");

    GLUMITYV2_INIT_HOOKING(MY_PLUGIN, OnHookInitFail)
    {
        GLUMITY_PRINT_COLOR(CON_GREEN, "Initialized Minhook\n", MY_PLUGIN);
    }

    GLUMITYV2_GAME_HOOK_CREATE(
        "DinoInfo_CheckIfOwned",
        DinoInfo_CheckIfOwned_o,
        DinoInfo_CheckIfOwned_hook);

    GLUMITYV2_GAME_HOOK_ENABLE_ALL(MY_PLUGIN);
}

GLUMITYV2_PLUGIN_ENTRY
{
    GLUMITYV2_PLUGIN_THREADRUN(Setup, 0);
}

GLUMITYV2_PLUGIN_EXIT
{
    GLUMITYV2_GAME_HOOK_CLEAN_ALL();
} 
