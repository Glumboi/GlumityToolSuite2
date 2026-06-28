#pragma once

#include <map>
#include <GlumityLib.h>
#include "include/libtcc.h"
#include <vector>
#include <string>
#include <set>
#include <il2cppInternal.h>

inline GlumityV2DumperExports g_dumperExports;
inline bool g_verboseBridge = false;
inline bool g_isReloading = false;

extern std::set<void *> g_trackedHookTargets;

inline MH_STATUS GlumityTrackedCreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID *ppOriginal)
{
    if (pTarget)
    {
        g_trackedHookTargets.insert(pTarget);
    }
    return MH_CreateHook(pTarget, pDetour, ppOriginal);
}

inline std::map<std::string, GlumityGeneric_Func> hardApiFuncs = {
    {"GlumityPlugin_printf", reinterpret_cast<GlumityGeneric_Func>(&GlumityPlugin_printf)},
    {"IL2CPP_String_ToCString", reinterpret_cast<GlumityGeneric_Func>(&IL2CPP_String_ToCString)},
    {"Glumity_GetErrorMessage", reinterpret_cast<GlumityGeneric_Func>(&Glumity_GetErrorMessage)},
    {"LoadIL2CPP", reinterpret_cast<GlumityGeneric_Func>(&LoadIL2CPP)},
    {"MH_CreateHook", reinterpret_cast<GlumityGeneric_Func>(&GlumityTrackedCreateHook)},
    {"MH_EnableHook", reinterpret_cast<GlumityGeneric_Func>(&MH_EnableHook)},
    {"MH_Initialize", reinterpret_cast<GlumityGeneric_Func>(&MH_Initialize)},
    {"MH_DisableHook", reinterpret_cast<GlumityGeneric_Func>(&MH_DisableHook)},
    {"MH_RemoveHook", reinterpret_cast<GlumityGeneric_Func>(&MH_RemoveHook)},
    {"GlumityV2DumperExports_Init", reinterpret_cast<GlumityGeneric_Func>(&GlumityV2DumperExports_Init)},
};

// hard coded libraries to link to tcc
inline const std::vector<std::string> G_HARD_LIBS = {"msvcrt", "kernel32", "user32"};
// ^
// |
// TODO: make both not hard coded and provide some sort of config

// ptr to program and the state
inline std::map<void *, GlumityPlugin> g_compiledScripts = {};