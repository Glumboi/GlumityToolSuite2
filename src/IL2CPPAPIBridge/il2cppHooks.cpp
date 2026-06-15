#include "il2cppHooks.h"
#include "il2cppInternal.h"

void *il2cpp_object_new_hook_v(Il2CppClass *klass)
{
    if (klass && klass->name && g_verboseBridge)
        GLUMITY_PRINT_COLOR(CON_CYAN, "An object is being allocated! (%s)\n", MY_PLUGIN, klass->name);

    return il2cpp_object_new(klass);
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

    return il2cpp_runtime_invoke(method, obj, params, exc);
}

void *il2cpp_object_new_hook(Il2CppClass *klass)
{
    return il2cpp_object_new(klass);
}

bool lastBlocked = true;
void *il2cpp_runtime_invoke_hook(const MethodInfo *method, void *obj, void **params, Il2CppObject **exc)
{
    return il2cpp_runtime_invoke(method, obj, params, exc);
}

void LoadIL2CPPApi(HMODULE hModule)
{
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
        return;

    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((BYTE *)hModule + dosHeader->e_lfanew);
    if (ntHeaders->Signature != IMAGE_NT_SIGNATURE)
        return;

    IMAGE_DATA_DIRECTORY exportDataDir = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    if (exportDataDir.VirtualAddress == 0)
    {
        GlumityPlugin_printf("No exported functions found in this module.\n", MY_PLUGIN);
        return;
    }

    PIMAGE_EXPORT_DIRECTORY exportDir = (PIMAGE_EXPORT_DIRECTORY)((BYTE *)hModule + exportDataDir.VirtualAddress);
    DWORD *nameRvas = (DWORD *)((BYTE *)hModule + exportDir->AddressOfNames);
    WORD *nameOrdinals = (WORD *)((BYTE *)hModule + exportDir->AddressOfNameOrdinals);
    DWORD *functionAddresses = (DWORD *)((BYTE *)hModule + exportDir->AddressOfFunctions);

    GLUMITY_PRINT_COLOR(CON_YELLOW, "Exported Functions (%d named functions):\n", MY_PLUGIN, exportDir->NumberOfNames);

    for (DWORD i = 0; i < exportDir->NumberOfNames; i++)
    {
        char *functionName = (char *)((BYTE *)hModule + nameRvas[i]);
        WORD ordinal = nameOrdinals[i];
        void *functionAddress = (BYTE *)hModule + functionAddresses[ordinal];
        hardApiFuncs[functionName] = (GlumityGeneric_Func)functionAddress;
        if (i % 2 == 0)
        {
            GlumityPlugin_printf(" - %s [At: %p]\n", MY_PLUGIN, functionName, functionAddress);
        }
        else
            GLUMITY_PRINT_COLOR(CON_CYAN, " - %s [At: %p]\n", MY_PLUGIN, functionName, functionAddress);
    }
}

void SetupIL2CPPHooks()
{
    GLUMITYV2_VERIFY_DEPENDENCY("GlumityV2IL2CPPDumper");
    GLUMITYV2_VERIFY_DEPENDENCY("libtcc");
    GLUMITYV2_DUMPER_WAITFOR_INIT(g_dumperExports);

    HMODULE hMod = GetModuleHandleA(GAME_ASSEMBLY);
    if (!hMod)
    {
        GLUMITY_PRINT_COLOR(CON_RED, "Could not find unity IL2CPP GameAssembly.dll\n", MY_PLUGIN);
        return;
    }

    GLUMITY_PRINT_COLOR(CON_BLUE, "Found unity IL2CPP GameAssembly.dll (starting export parsing...)\n", MY_PLUGIN);

    LoadIL2CPP(hMod);
    if (!il2cpp_object_new)
    {
        GLUMITY_PRINT_COLOR(CON_YELLOW, "Could not locate required internal API endpoints inside map!\n", MY_PLUGIN);
        return;
    }

    // check verbosity
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

    if (g_verboseBridge) // Hook to different hooks, to avoid performance cost if verbosity is not desired
    {
        GLUMITYV2_GAME_HOOK_CREATE("il2cpp_object_new", il2cpp_object_new, il2cpp_object_new_hook_v);
        GLUMITYV2_GAME_HOOK_CREATE("il2cpp_runtime_invoke", il2cpp_runtime_invoke, il2cpp_runtime_invoke_hook_v);
    }
    else
    {
        GLUMITYV2_GAME_HOOK_CREATE("il2cpp_object_new", il2cpp_object_new, il2cpp_object_new_hook);
        GLUMITYV2_GAME_HOOK_CREATE("il2cpp_runtime_invoke", il2cpp_runtime_invoke, il2cpp_runtime_invoke_hook);
    }

    GLUMITYV2_GAME_HOOK_ENABLE_ALL(MY_PLUGIN);
}
