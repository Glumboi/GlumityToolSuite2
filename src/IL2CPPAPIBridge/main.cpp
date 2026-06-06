#include <GlumityLib.h>
#include <string.h>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <filesystem>
#include <memoryapi.h>

#include "include/libtcc.h"

#include "globals.h"
#include "defines.h"
#include "il2cppHooks.h"

struct DeadMemoryBlock
{
    void *address;
    void *tccState;
    char *name;
};
std::vector<DeadMemoryBlock> g_zombieBlocks;

std::map<std::string, GlumityPlugin> g_activePlugins;
std::set<void *> g_trackedHookTargets;

extern "C" __declspec(dllexport) MH_STATUS __cdecl GlumityV2_TrackedCreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID *ppOriginal)
{
    MH_STATUS status = MH_CreateHook(pTarget, pDetour, ppOriginal);
    if (status == MH_OK || status == MH_ERROR_ALREADY_CREATED)
    {
        g_trackedHookTargets.insert(pTarget);
    }
    return status;
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

    std::string scriptPathString = scriptPath.string();
    std::string fullOptions = "-g -L" + std::string(libDir) + "\\lib -I" + std::string(includeDir) + "\\include ";
    tcc_set_options(tccState, fullOptions.c_str());

    tcc_set_output_type(tccState, TCC_OUTPUT_MEMORY);
    tcc_set_error_func(tccState, 0x0, TCC_Error_Handler);

    if (tcc_add_file(tccState, scriptPath.string().c_str()) < 0)
    {
        tcc_delete(tccState);
        return;
    }

    tcc_add_symbol(tccState, "MH_CreateHook", (const void *)GlumityV2_TrackedCreateHook);

    for (auto &apiFunc : hardApiFuncs)
        tcc_add_symbol(tccState, apiFunc.first.c_str(), (const void *)apiFunc.second);

    for (auto &lib : G_HARD_LIBS)
        tcc_add_library(tccState, lib.c_str());

    int size = tcc_relocate(tccState, NULL);
    if (size <= 0)
    {
        tcc_delete(tccState);
        return;
    }

    void *program = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!program)
    {
        tcc_delete(tccState);
        return;
    }

    if (tcc_relocate(tccState, program) < 0)
    {
        VirtualFree(program, 0, MEM_RELEASE);
        tcc_delete(tccState);
        return;
    }

    GlumityPlugin plugin = {};
    plugin.entryPoint = (GlumityPlugin_EntryPoint)tcc_get_symbol(tccState, "GlumityMain");
    plugin.exitPoint = (GlumityPlugin_ExitPoint)tcc_get_symbol(tccState, "GlumityExit");

    if (scriptPathString.length() >= sizeof(plugin.dllPath))
    {
        VirtualFree(program, 0, MEM_RELEASE);
        tcc_delete(tccState);
        return;
    }

    strcpy_s(plugin.dllPath, sizeof(plugin.dllPath), scriptPathString.c_str());
    plugin.tccState = tccState;
    plugin.hDll = NULL;
    plugin.name = _strdup(scriptPath.filename().string().c_str());
    plugin.compiledSz = size;
    plugin.programBase = program;

    if (plugin.entryPoint)
    {
        g_activePlugins[scriptPathString] = plugin;
        plugin.entryPoint();
    }
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

VOID HotReloadJITScripts()
{
    HMODULE hCurrentDll = GetModuleHandleA("IL2CPPAPIBridge.dll");

    char pathBuffer[MAX_PATH] = {0};
    GetPluginDirectory(hCurrentDll, pathBuffer, sizeof(pathBuffer));

    std::filesystem::path baseDir(pathBuffer);
    std::string libs = (baseDir / "tcc_libs").string();
    std::string incs = (baseDir / "tcc_include").string();

    g_isReloading = true;

    for (auto &[path, plugin] : g_activePlugins)
    {
        if (plugin.exitPoint)
        {
            plugin.exitPoint(); 
        }
    }

    for (void *targetAddress : g_trackedHookTargets)
    {
        if (targetAddress)
        {
            MH_DisableHook(targetAddress);
            MH_RemoveHook(targetAddress);
            GLUMITY_PRINT_COLOR(CON_YELLOW, "Safely restored game function bytes: %p\n", MY_PLUGIN, targetAddress);
        }
    }
    g_trackedHookTargets.clear();

    Sleep(30);

    std::vector<std::string> pathsToReload;
    for (auto &[path, plugin] : g_activePlugins)
    {
        pathsToReload.push_back(path);

        DeadMemoryBlock zombie = {plugin.programBase, plugin.tccState, plugin.name};
        g_zombieBlocks.push_back(zombie);
    }

    g_activePlugins.clear();

    for (const std::string &scriptPath : pathsToReload)
    {
        GLUMITY_PRINT_COLOR(CON_YELLOW, "Applying script updates: %s\n", MY_PLUGIN, scriptPath.c_str());
        LoadAndRunSingleScript(std::filesystem::path(scriptPath), libs.c_str(), incs.c_str());
    }

    g_isReloading = false;
}

VOID Setup()
{
    SetupIL2CPPHooks();

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

    strcat_s(libs, sizeof(libs), "tcc_libs");
    strcat_s(incs, sizeof(incs), "tcc_include");

    GetPluginDirectory(hCurrentDll, pathBuffer, sizeof(pathBuffer));
    ProcessJITScriptsFolder(pathBuffer, libs, incs);

    while (1)
    {
        if ((GetAsyncKeyState(VK_END) & 0x8000) != 0)
        {
            GLUMITY_PRINT_COLOR(CON_CYAN, "End pressed, reloading jit_scripts...\n", MY_PLUGIN);
            HotReloadJITScripts();

            while (GetAsyncKeyState(VK_END) & 0x8000)
            {
                Sleep(10);
            }

            Sleep(50);
        }
        Sleep(10);
    }
}

GLUMITYV2_PLUGIN_ENTRY
{
    GLUMITYV2_PLUGIN_THREADRUN(Setup, 0);
}

GLUMITYV2_PLUGIN_EXIT
{
    GLUMITYV2_GAME_HOOK_CLEAN_ALL();

    for (auto &zombie : g_zombieBlocks)
    {
        if (zombie.tccState)
            tcc_delete((TCCState *)zombie.tccState);
        if (zombie.address)
            VirtualFree(zombie.address, 0, MEM_RELEASE);
        if (zombie.name)
            free(zombie.name);
    }
    return 1;
}
