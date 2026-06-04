#include <GlumityLib.h>
#include <string.h>
#include <map>
#include <string>
#include <vector>
#include <filesystem>
#include <memoryapi.h>

#include "include/libtcc.h"

GlumityV2DumperExports dumperExports;
#define MY_PLUGIN "IL2CPPAPIBridge"
#define GAME_ASSEMBLY "GameAssembly.dll"
#define VERBOSE_DEFINITION "Plugins\\VERBOSE_BRIDGE"
#define TCC_HEADER "TCC"
bool gb_verboseBridge = false;

// hard coded api functions exposed by various modules of the glumityv2 suite
std::map<std::string, GlumityGeneric_Func> hardApiFuncs = {
    {"GlumityPlugin_printf", reinterpret_cast<GlumityGeneric_Func>(&GlumityPlugin_printf)},
    {"MH_DisableHook", reinterpret_cast<GlumityGeneric_Func>(&MH_DisableHook)},
    {"GlumityV2DumperExports_Init", reinterpret_cast<GlumityGeneric_Func>(&GlumityV2DumperExports_Init)},
    {"IL2CPP_ResolveFunctions", reinterpret_cast<GlumityGeneric_Func>(&IL2CPP_ResolveFunctions)},
    {"IL2CPP_String_ToCString", reinterpret_cast<GlumityGeneric_Func>(&IL2CPP_String_ToCString)},
    {"MH_Initialize", reinterpret_cast<GlumityGeneric_Func>(&MH_Initialize)},
    {"MH_CreateHook", reinterpret_cast<GlumityGeneric_Func>(&MH_CreateHook)},
    {"MH_EnableHook", reinterpret_cast<GlumityGeneric_Func>(&MH_EnableHook)},
    {"Glumity_GetErrorMessage", reinterpret_cast<GlumityGeneric_Func>(&Glumity_GetErrorMessage)},
};

// hard coded libraries to link to tcc
const std::vector<std::string> hardLibs = {"msvcrt", "kernel32", "user32"};
// ^
// |
// TODO: make both not hard coded and provide some sort of config

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
        hardApiFuncs[functionName] = (GlumityGeneric_Func)functionAddress;
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

uint8_t prntCntr = 0; // for color alternating

const char *ignoredMethods[] = {
    "Update",
    "FixedUpdate",
    "GetRenderFrameInterval",
    "NotifyBeforeUpdate",
    "InvokeMoveNext",
    "get_Current",
    "DoRenderLoop_Internal"};
const int totalIgnored = sizeof(ignoredMethods) / sizeof(ignoredMethods[0]);

void *il2cpp_runtime_invoke_hook_v(IL2CPP_MethodInfo *method, void *obj, void **params, void **exc)
{
    if (method && method->name && method->klass && method->klass->name && gb_verboseBridge)
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
                GLUMITY_PRINT_COLOR(CON_CYAN, "A method is being executed! (%s - %s | %p)\n",
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

VOID TCC_Error_Handler(void *opaque, const char *msg)
{
    GLUMITY_PRINT_COLOR(CON_RED, "%s\n", TCC_HEADER, msg);
}

void LoadAndRunSingleScript(const std::filesystem::path &scriptPath, const char *libDir, const char *includeDir)
{
    TCCState *tccState = tcc_new();
    if (!tccState)
        return;

    std::string libOption = "-L" + std::string(libDir) + "\\lib";
    std::string includeOption = "-I" + std::string(includeDir) + "\\include";
    std::string fullOptions = "-g " + libOption + " " + includeOption;
    tcc_set_options(tccState, fullOptions.c_str());

    tcc_set_output_type(tccState, TCC_OUTPUT_MEMORY);
    tcc_set_error_func(tccState, 0x0, TCC_Error_Handler);

    // Load only this specific self-contained .c file
    std::string filePathStr = scriptPath.string();
    int stat = tcc_add_file(tccState, filePathStr.c_str());
    if (stat < 0)
    {
        GLUMITY_PRINT_COLOR(CON_RED, "Failed to compile file: %s\n", TCC_HEADER, scriptPath.filename().string().c_str());
        tcc_delete(tccState);
        return;
    }

    for (auto &apiFunc : hardApiFuncs)
    {
        tcc_add_symbol(tccState, apiFunc.first.c_str(), (const void *)apiFunc.second);
        if (gb_verboseBridge)
            GLUMITY_PRINT_COLOR(CON_CYAN, "Added a library function: %s\n", TCC_HEADER, apiFunc.first.c_str());
    }

    for (auto &lib : hardLibs)
    {
        tcc_add_library(tccState, lib.c_str());
        if (gb_verboseBridge)
            GLUMITY_PRINT_COLOR(CON_CYAN, "Added a lib: %s\n", TCC_HEADER, lib.c_str());
    }

    int size = tcc_relocate(tccState, NULL);
    if (size <= 0)
    {
        GLUMITY_PRINT_COLOR(CON_RED, "Failed memory layout calculation for: %s\n", TCC_HEADER, scriptPath.filename().string().c_str());
        tcc_delete(tccState);
        return;
    }

    void *program = calloc(1, size);
    if (!program)
    {
        tcc_delete(tccState);
        return;
    }

    stat = tcc_relocate(tccState, program);
    if (stat < 0)
    {
        GLUMITY_PRINT_COLOR(CON_RED, "Failed to relocate script into memory: %s\n", TCC_HEADER, scriptPath.filename().string().c_str());
        free(program);
        tcc_delete(tccState);
        return;
    }

    GlumityPlugin_EntryPoint entryPoint = (GlumityPlugin_EntryPoint)tcc_get_symbol(tccState, "GlumityMain");
    if (entryPoint != nullptr)
    {
        GLUMITY_PRINT_COLOR(CON_GREEN, "Executing Script Instance -> %s [%p]\n", TCC_HEADER, scriptPath.filename().string().c_str(), entryPoint);
        entryPoint();
    }
    else
    {
        GLUMITY_PRINT_COLOR(CON_RED, "Missing 'GlumityMain' entry point in script: %s\n", TCC_HEADER, scriptPath.filename().string().c_str());
    }

    // Note: If scripts register permanent hooks, do NOT delete/free the context here
    // as it will unmap the generated machine code from the game's address space.
    // tcc_delete(tccState);
    // free(program);
}

VOID ProcessJITScriptsFolder(const char *targetProgramFolder, const char *libDir, const char *includeDir)
{
    std::filesystem::path scriptDir = std::filesystem::path(targetProgramFolder) / "jit_scripts";
    if (!std::filesystem::exists(scriptDir) || !std::filesystem::is_directory(scriptDir))
    {
        GLUMITY_PRINT_COLOR(CON_RED, "Scripts directory not found: %s\n", TCC_HEADER, scriptDir.string().c_str());
        return;
    }

    GLUMITY_PRINT_COLOR(CON_BLUE, "Scanning for isolated script instances...\n", TCC_HEADER);

    for (const auto &entry : std::filesystem::directory_iterator(scriptDir))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".c")
        {
            GLUMITY_PRINT_COLOR(CON_YELLOW, "Found script: %s\n", TCC_HEADER, entry.path().filename().string().c_str());
            LoadAndRunSingleScript(entry.path(), libDir, includeDir);
        }
    }
}

VOID Setup()
{
    GLUMITYV2_DUMPER_WAITFOR_INIT(dumperExports);
    GLUMITYV2_VERIFY_DEPENDENCY("GlumityV2IL2CPPDumper");
    GLUMITYV2_VERIFY_DEPENDENCY("libtcc");

    HMODULE hMod = GetModuleHandleA(GAME_ASSEMBLY);
    if (!hMod)
    {
        GLUMITY_PRINT_COLOR(CON_RED, "Could not find unity IL2CPP GameAssembly.dll\n", MY_PLUGIN);
        return;
    }

    GLUMITY_PRINT_COLOR(CON_BLUE, "Found unity IL2CPP GameAssembly.dll (starting export parsing...)\n", MY_PLUGIN);
    LoadIl2CPP_API(hMod);

    if (hardApiFuncs.find("il2cpp_object_new") == hardApiFuncs.end() ||
        hardApiFuncs.find("il2cpp_runtime_invoke") == hardApiFuncs.end())
    {
        GLUMITY_PRINT_COLOR(CON_YELLOW, "Could not locate required internal API endpoints inside map!\n", MY_PLUGIN);
        return;
    }

    target_il2cpp_object_new = (il2cpp_object_new_t)hardApiFuncs["il2cpp_object_new"];
    target_il2cpp_runtime_invoke = (il2cpp_runtime_invoke_t2)hardApiFuncs["il2cpp_runtime_invoke"];

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

    HMODULE hCurrentDll = GetModuleHandleA("IL2CPPAPIBridge.dll");
    char pathBuffer[MAX_PATH] = {0};
    GetPluginDirectory(hCurrentDll, pathBuffer, sizeof(pathBuffer));

    size_t len = strlen(pathBuffer);
    if (len > 0 && pathBuffer[len - 1] != '\\')
    {
        strcat_s(pathBuffer, sizeof(pathBuffer), "\\");
    }

    char libs[MAX_PATH];
    strcpy_s(libs, sizeof(libs), pathBuffer);

    char incs[MAX_PATH];
    strcpy_s(incs, sizeof(incs), pathBuffer);

    char tccDllPath[MAX_PATH];
    strcpy_s(tccDllPath, sizeof(tccDllPath), pathBuffer);
    strcat_s(tccDllPath, sizeof(tccDllPath), "libtcc.dll");

    HMODULE hTcc = LoadLibraryA(tccDllPath);
    if (!hTcc)
    {
        GlumityPlugin_printf("Failed to manually resolve sibling dependency: %s\n", MY_PLUGIN, tccDllPath);
        return;
    }
    GlumityPlugin_printf("Successfully resolved dependency internally: %s\n", MY_PLUGIN, tccDllPath);

    strcat_s(libs, sizeof(libs), "tcc_libs");
    strcat_s(incs, sizeof(incs), "tcc_include");

    GetPluginDirectory(hCurrentDll, pathBuffer, sizeof(pathBuffer));
    ProcessJITScriptsFolder(pathBuffer, libs, incs);
}

GLUMITYV2_PLUGIN_ENTRY
{
    GLUMITYV2_PLUGIN_THREADRUN(Setup, 0);
}

GLUMITYV2_PLUGIN_EXIT
{
    MH_DisableHook(MH_ALL_HOOKS);
}