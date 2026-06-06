#pragma once

#include <GlumityLib.h>

#include "defines.h"
#include "globals.h"

#include <filesystem>

inline uint8_t prntCntr = 0; // for color alternating

inline const char *ignoredMethods[] = {
    "Update",
    "FixedUpdate",
    "GetRenderFrameInterval",
    "NotifyBeforeUpdate",
    "InvokeMoveNext",
    "get_Current",
    "DoRenderLoop_Internal"};
const inline int totalIgnored = sizeof(ignoredMethods) / sizeof(ignoredMethods[0]);

inline void *Global_PreHook()
{
}

GLUMITYV2_GAME_HOOK_TYPE(void *, il2cpp_object_new_t)(IL2CPP_Class *klass);
inline il2cpp_object_new_t target_il2cpp_object_new = nullptr;

GLUMITYV2_GAME_HOOK_TYPE(void *, il2cpp_runtime_invoke_t2)(IL2CPP_MethodInfo *method, void *obj, void **params, void **exc);
inline il2cpp_runtime_invoke_t2 target_il2cpp_runtime_invoke = nullptr; // t2 has no specific meaning, just to avoid redefinition and keep this as contained as possible

void *il2cpp_object_new_hook_v(IL2CPP_Class *klass);
void *il2cpp_runtime_invoke_hook_v(IL2CPP_MethodInfo *method, void *obj, void **params, void **exc);
void *il2cpp_object_new_hook(IL2CPP_Class *klass);
void *il2cpp_runtime_invoke_hook(IL2CPP_MethodInfo *method, void *obj, void **params, void **exc);

void LoadIL2CPPApi(HMODULE hModule);
void SetupIL2CPPHooks();