#ifndef GLUMITY_HOOKING_H
#define GLUMITY_HOOKING_H

#include <windows.h>
#include <stdio.h>
#include <MinHook.h>

void GlumityPlugin_printf(const char *fmt, const char *printHeaderInner, ...);

#define GLUMITYV2_INIT_HOOKING(pluginName, onFail)                           \
    if (MH_Initialize() != MH_OK)                                            \
    {                                                                        \
        GlumityPlugin_printf("Failed to initialize Minhook!\n", pluginName); \
        onFail;                                                              \
    }                                                                        \
    else

#define GLUMITYV2_GAME_HOOK_CREATE(name, original, hook)                                 \
    {                                                                                    \
        MH_STATUS stat = MH_CreateHook(                                                  \
            (LPVOID *)(original),                                                        \
            &hook,                                                                       \
            (LPVOID *)&original);                                                        \
        if (stat == MH_OK)                                                               \
            GlumityPlugin_printf("Created a hook: %s\n", GLUMITYLIB_PRINT_HEADER, name); \
    }

#define GLUMITYV2_GAME_HOOK_ENABLE_ALL(pluginName)                    \
    {                                                                 \
        MH_STATUS stat = MH_EnableHook(MH_ALL_HOOKS);                 \
        if (stat == MH_OK)                                            \
            GlumityPlugin_printf("Enabled all hooks!\n", pluginName); \
    }

#define GLUMITYV2_GAME_HOOK_ENABLE(name, original, pluginName)                           \
    {                                                                                    \
        MH_STATUS stat = MH_EnableHook(original);                                        \
        if (stat == MH_OK)                                                               \
            GlumityPlugin_printf("Enabled a hook: %s\n", GLUMITYLIB_PRINT_HEADER, name); \
    }

#define GLUMITYV2_GAME_HOOK_TYPE(retType, name) typedef retType(*name)

#endif