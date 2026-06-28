#include "il2cppHooks.h"
#include "il2cppInternal.h"

il2cpp_object_new_t o_il2cpp_object_new = nullptr;
il2cpp_runtime_invoke_t o_il2cpp_runtime_invoke = nullptr;

void *il2cpp_object_new_hook_v(Il2CppClass *klass)
{
    if (klass && klass->name && g_verboseBridge)
        GLUMITY_PRINT_COLOR(CON_CYAN, "An object is being allocated! (%s)\n", MY_PLUGIN, klass->name);

    return o_il2cpp_object_new(klass);
}

void *il2cpp_runtime_invoke_hook_v(const MethodInfo *method, void *obj, void **params, Il2CppObject **exc)
{
    if (method && method->name && method->klass && method->klass->name && g_verboseBridge)
    {
        bool isBanned = false;

        for (int i = 0; i < totalIgnored; i++)
        {
            if (strcmp(method->name, ignoredMethods[i]) == 0)
            {
                isBanned = true;
                break;
            }
        }
        
        if (!isBanned)
        {
            if (prntCntr % 2 == 0)
            {
                GlumityPlugin_printf("A method is being executed! (%s - %s | %p)\n",
                                     MY_PLUGIN, method->klass->name, method->name, method->methodPointer);
            }
            else
            {
                GLUMITY_PRINT_COLOR(CON_RESET, "A method is being executed! (%s - %s | %p)\n",
                                    MY_PLUGIN, method->klass->name, method->name, method->methodPointer);
            }
            prntCntr++;
        }
    }

    return o_il2cpp_runtime_invoke(method, obj, params, exc);
}

void *il2cpp_object_new_hook(Il2CppClass *klass)
{
    return o_il2cpp_object_new(klass);
}

bool lastBlocked = true;
void *il2cpp_runtime_invoke_hook(const MethodInfo *method, void *obj, void **params, Il2CppObject **exc)
{
    return o_il2cpp_runtime_invoke(method, obj, params, exc);
}

void SetupIL2CPPHooks()
{
    GLUMITYV2_VERIFY_DEPENDENCY("libtcc");
    GLUMITYV2_VERIFY_DEPENDENCY("GlumityV2IL2CPPDumper");
    GLUMITYV2_DUMPER_WAITFOR_INIT(g_dumperExports);

    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    std::filesystem::path gameDir = std::filesystem::path(exePath).parent_path();
    std::filesystem::path verboseCheckPath = gameDir / VERBOSE_DEFINITION;
    g_verboseBridge = std::filesystem::exists(verboseCheckPath);
    GLUMITY_PRINT_COLOR(CON_YELLOW, "Verbose: %d (Checked path: %s)\n",
                        MY_PLUGIN,
                        g_verboseBridge,
                        verboseCheckPath.string().c_str());

    GLUMITYV2_INIT_HOOKING(MY_PLUGIN, return)
    {
        GLUMITY_PRINT_COLOR(CON_GREEN, "Initialized Minhook!\n", MY_PLUGIN);
    }

    o_il2cpp_object_new = (il2cpp_object_new_t)il2cpp_object_new;
    o_il2cpp_runtime_invoke = (il2cpp_runtime_invoke_t)il2cpp_runtime_invoke;

    if (g_verboseBridge) 
    {
        GLUMITYV2_GAME_HOOK_CREATE("il2cpp_object_new", o_il2cpp_object_new, il2cpp_object_new_hook_v);
        GLUMITYV2_GAME_HOOK_CREATE("il2cpp_runtime_invoke", o_il2cpp_runtime_invoke, il2cpp_runtime_invoke_hook_v);
    }
    else
    {
        GLUMITYV2_GAME_HOOK_CREATE("il2cpp_object_new", o_il2cpp_object_new, il2cpp_object_new_hook);
        GLUMITYV2_GAME_HOOK_CREATE("il2cpp_runtime_invoke", o_il2cpp_runtime_invoke, il2cpp_runtime_invoke_hook);
    }

    GLUMITYV2_GAME_HOOK_ENABLE_ALL(MY_PLUGIN);
}