#include <GlumityLib.h>
#include <string.h>
#include <map>
#include <string>
#include <filesystem>

#define MY_PLUGIN "IL2CPPAPIBridge"
#define GAME_ASSEMBLY "GameAssembly.dll"
#define VERBOSE_DEFINITION "Plugins\\VERBOSE_BRIDGE"
bool gb_verboseBridge = false;

std::map<std::string, GlumityGeneric_Func> apiFuncs;

GLUMITYV2_GAME_HOOK_TYPE(void *, il2cpp_object_new_t)(IL2CPP_Class *klass);
il2cpp_object_new_t target_il2cpp_object_new = nullptr;

GLUMITYV2_GAME_HOOK_TYPE(void *, il2cpp_runtime_invoke_t2)(IL2CPP_MethodInfo *method, void *obj, void **params, void **exc);
il2cpp_runtime_invoke_t2 target_il2cpp_runtime_invoke = nullptr; // t2 has no specific meaning, just to avoid redefinition and keep this as contained as possible

void LoadIl2CPP_API(HMODULE hModule)
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
        apiFuncs[functionName] = (GlumityGeneric_Func)functionAddress;
        if (i % 2 == 0)
        {
            GlumityPlugin_printf(" - %s [At: %p]\n", MY_PLUGIN, functionName, functionAddress);
        }
        else
            GLUMITY_PRINT_COLOR(CON_CYAN, " - %s [At: %p]\n", MY_PLUGIN, functionName, functionAddress);
    }
}

void *il2cpp_object_new_hook_v(IL2CPP_Class *klass)
{
    if (klass && klass->name && gb_verboseBridge)
        GLUMITY_PRINT_COLOR(CON_CYAN, "An object is being allocated! (%s)\n", MY_PLUGIN, klass->name);

    return target_il2cpp_object_new(klass);
}

void *il2cpp_runtime_invoke_hook_v(IL2CPP_MethodInfo *method, void *obj, void **params, void **exc)
{
    if (method && method->name && gb_verboseBridge)
        GLUMITY_PRINT_COLOR(CON_CYAN, "A method is being executed! (%s)\n", MY_PLUGIN, method->name);

    return target_il2cpp_runtime_invoke(method, obj, params, exc);
}

void *il2cpp_object_new_hook(IL2CPP_Class *klass)
{
    return target_il2cpp_object_new(klass);
}

void *il2cpp_runtime_invoke_hook(IL2CPP_MethodInfo *method, void *obj, void **params, void **exc)
{
    return target_il2cpp_runtime_invoke(method, obj, params, exc);
}

VOID InitVerbosity()
{
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    std::filesystem::path gameDir = std::filesystem::path(exePath).parent_path();
    std::filesystem::path verboseCheckPath = gameDir / VERBOSE_DEFINITION;
    gb_verboseBridge = std::filesystem::exists(verboseCheckPath);
    GLUMITY_PRINT_COLOR(CON_YELLOW, "Verbose: %d (Checked path: %s)\n",
                        MY_PLUGIN,
                        gb_verboseBridge,
                        verboseCheckPath.string().c_str());
}

VOID Setup()
{
    GLUMITYV2_VERIFY_DEPENDENCY("GlumityV2IL2CPPDumper");
    HMODULE hMod = GetModuleHandleA(GAME_ASSEMBLY);
    if (!hMod)
    {
        GLUMITY_PRINT_COLOR(CON_RED, "Could not find unity IL2CPP GameAssembly.dll\n", MY_PLUGIN);
        return;
    }

    GLUMITY_PRINT_COLOR(CON_BLUE, "Found unity IL2CPP GameAssembly.dll (starting export parsing...)\n", MY_PLUGIN);
    LoadIl2CPP_API(hMod);

    if (apiFuncs.find("il2cpp_object_new") == apiFuncs.end() ||
        apiFuncs.find("il2cpp_runtime_invoke") == apiFuncs.end())
    {
        GLUMITY_PRINT_COLOR(CON_YELLOW, "Could not locate required internal API endpoints inside map!\n", MY_PLUGIN);
        return;
    }

    target_il2cpp_object_new = (il2cpp_object_new_t)apiFuncs["il2cpp_object_new"];
    target_il2cpp_runtime_invoke = (il2cpp_runtime_invoke_t2)apiFuncs["il2cpp_runtime_invoke"];

    InitVerbosity();

    GLUMITYV2_INIT_HOOKING(MY_PLUGIN, return)
    {
        GLUMITY_PRINT_COLOR(CON_GREEN, "Initialized Minhook!\n", MY_PLUGIN);
    }

    if (gb_verboseBridge) // Hook to different hooks, to avoid performance cost if verbosity is not desired
    {
        GLUMITYV2_GAME_HOOK_CREATE("il2cpp_object_new", target_il2cpp_object_new, il2cpp_object_new_hook_v);
        GLUMITYV2_GAME_HOOK_CREATE("il2cpp_runtime_invoke", target_il2cpp_runtime_invoke, il2cpp_runtime_invoke_hook_v);
    }
    else
    {
        GLUMITYV2_GAME_HOOK_CREATE("il2cpp_object_new", target_il2cpp_object_new, il2cpp_object_new_hook);
        GLUMITYV2_GAME_HOOK_CREATE("il2cpp_runtime_invoke", target_il2cpp_runtime_invoke, il2cpp_runtime_invoke_hook);
    }

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