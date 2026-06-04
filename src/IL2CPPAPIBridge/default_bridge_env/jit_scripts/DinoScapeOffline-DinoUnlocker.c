// This is an adaption of the original DinoScapeOffline mod but for it to be compiled at runtime using TCC!
#include <GlumityLib/GlumityLib.h>
#include <windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void GlumityPlugin_printf(const char *fmt, const char *head, ...);
GlumityV2DumperExports dumperExports;

HMODULE ForceGetHmodule(const char *moduleName, int isInApplicationFolder)
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

#pragma region customTypes

struct DinoInfo__Fields
{
    // 0x00 to 0x50 is exactly 0x50 bytes of padding
    PAD_TO(0x00, 0x50);

    bool isSelected; // Offset 0x50
    bool owned;      // Offset 0x51

    // Preloader is at 0x58. Current offset is 0x52.
    PAD_TO(0x52, 0x58);
    void *Preloader; // Offset 0x58
};

struct DinoInfo
{
    void *vtable;
    void *monitor;
    struct DinoInfo__Fields fields; // starts at 0x10 usually, but dump says the class is 0x70
};

#pragma endregion

#pragma region hooks

#define MY_PLUGIN "DinoScapeOffline-DinoUnlocker"

GLUMITYV2_GAME_HOOK_TYPE(BOOL, DinoInfo_CheckIfOwned_t)(struct DinoInfo *, struct IL2CPP_MethodInfo *);
DinoInfo_CheckIfOwned_t DinoInfo_CheckIfOwned_o;
BOOL DinoInfo_CheckIfOwned_hook(struct DinoInfo *_this, struct IL2CPP_MethodInfo *mi)
{
    _this->fields.owned = true;
    return true; // Force unlock all dinos!
}

#pragma endregion

void Setup()
{
    GLUMITYV2_DUMPER_WAITFOR_INIT(dumperExports);
    const char *targetMod = "GameAssembly.dll";
    HANDLE hmod = ForceGetHmodule(targetMod, true);

    DinoInfo_CheckIfOwned_o =
        (DinoInfo_CheckIfOwned_t)dumperExports.GlumityV2Dumper_GetFunctionPointer("DinoInfo", "CheckIfOwned");

    GlumityPlugin_printf("CheckIfOwnedPtr: %x\n", MY_PLUGIN, DinoInfo_CheckIfOwned_o);

    GLUMITYV2_INIT_HOOKING(MY_PLUGIN, return)
    {
        GlumityPlugin_printf("Initialized Minhook\n", MY_PLUGIN);
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
    MH_DisableHook(MH_ALL_HOOKS);
}