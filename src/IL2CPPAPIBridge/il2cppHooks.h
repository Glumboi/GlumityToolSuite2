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

void *il2cpp_object_new_hook_v(Il2CppClass *klass);
void *il2cpp_runtime_invoke_hook_v(const MethodInfo *method, void *obj, void **params, Il2CppObject **exc);
void *il2cpp_object_new_hook(Il2CppClass *klass);
void *il2cpp_runtime_invoke_hook(const MethodInfo *method, void *obj, void **params, Il2CppObject **exc);

void LoadIL2CPPApi(HMODULE hModule);
void SetupIL2CPPHooks();