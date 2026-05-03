#include <GlumityLib.h>
#include "hooks.h"
#include <string.h>

GlumityV2DumperExports dumperExports;

HMODULE ForceGetHmodule(const char *moduleName, bool isInApplicationFolder)
{
    HMODULE returnModule = GetModuleHandleA(moduleName);

    if (!returnModule)
    {
        char buff[MAX_PATH];

        if (isInApplicationFolder)
        {
            GetCurrentDirectoryA(MAX_PATH, buff);
            strcat_s(buff, MAX_PATH, "\\");
        }
        else
        {
            GetWindowsDirectoryA(buff, MAX_PATH);
            strcat_s(buff, MAX_PATH, "\\system32\\");
        }

        strcat_s(buff, MAX_PATH, moduleName);
        returnModule = LoadLibraryA(buff);
    }

    return returnModule;
}

void MakeHooks()
{
    // Wait for dumper to finish initializing, crucial to avoid crashes
    GLUMITYV2_DUMPER_WAITFOR_INIT(dumperExports);
    const char *targetMod = "GameAssembly.dll";

    HANDLE hmod = ForceGetHmodule(targetMod, true);
    GlumityPlugin_printf("Force loaded module: %s at: %x\n", MY_PLUGIN, targetMod, hmod);

    void *GetFullUrlPtr =
        dumperExports.GlumityV2Dumper_GetFunctionPointerWithPattern(hmod, "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 80 3D 1D DF F0 00 00");

    GlumityPlugin_printf("GetFullUrlPtr: %x\n", MY_PLUGIN, GetFullUrlPtr);

    void *MainReferencesUpdatePtr =
        dumperExports.GlumityV2Dumper_GetFunctionPointer("MainReferences", "Update");
    GlumityPlugin_printf("MainReferencesUpdatePtr: %x\n", MY_PLUGIN, MainReferencesUpdatePtr);

    void *CheckIfOwnedPtr =
        dumperExports.GlumityV2Dumper_GetFunctionPointer("DinoInfo", "CheckIfOwned");

    GlumityPlugin_printf("CheckIfOwnedPtr: %x\n", MY_PLUGIN, CheckIfOwnedPtr);

    if (GetFullUrlPtr)
    {
        // Make a hook
        Hooks_Install(GetFullUrlPtr, MainReferencesUpdatePtr, CheckIfOwnedPtr);
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