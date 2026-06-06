#pragma once

#include <map>
#include <GlumityLib.h>
#include "include/libtcc.h"
#include <vector>
#include <string>

inline GlumityV2DumperExports g_dumperExports;
inline bool gb_verboseBridge = false;

// hard coded api functions exposed by various modules of the glumityv2 suite
inline std::map<std::string, GlumityGeneric_Func> hardApiFuncs = {
    {"GlumityPlugin_printf", reinterpret_cast<GlumityGeneric_Func>(&GlumityPlugin_printf)},
    {"MH_DisableHook", reinterpret_cast<GlumityGeneric_Func>(&MH_DisableHook)},
    {"GlumityV2DumperExports_Init", reinterpret_cast<GlumityGeneric_Func>(&GlumityV2DumperExports_Init)},
    {"IL2CPP_ResolveFunctions", reinterpret_cast<GlumityGeneric_Func>(&IL2CPP_ResolveFunctions)},
    {"IL2CPP_String_ToCString", reinterpret_cast<GlumityGeneric_Func>(&IL2CPP_String_ToCString)},
    {"MH_Initialize", reinterpret_cast<GlumityGeneric_Func>(&MH_Initialize)},
    {"MH_CreateHook", reinterpret_cast<GlumityGeneric_Func>(&MH_CreateHook)},
    {"MH_EnableHook", reinterpret_cast<GlumityGeneric_Func>(&MH_EnableHook)},
    {"Glumity_GetErrorMessage", reinterpret_cast<GlumityGeneric_Func>(&Glumity_GetErrorMessage)},
    {"MH_RemoveHook", reinterpret_cast<GlumityGeneric_Func>(&MH_RemoveHook)},
};

// hard coded libraries to link to tcc
inline const std::vector<std::string> G_HARD_LIBS = {"msvcrt", "kernel32", "user32"};
// ^
// |
// TODO: make both not hard coded and provide some sort of config

// ptr to program and the state
inline std::map<void *, GlumityPlugin> g_compiledScripts = {};