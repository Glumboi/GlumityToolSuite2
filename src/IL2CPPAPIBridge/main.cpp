#include <GlumityLib.h>
#include <string.h>
#include <map>
#include <string>
#include <vector>
#include <filesystem>
#include <memoryapi.h>

#include "include/libtcc.h"

#include "globals.h"
#include "defines.h"
#include "il2cppHooks.h"

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

    for (auto &lib : G_HARD_LIBS)
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

    GlumityPlugin plugin;
    plugin.entryPoint = (GlumityPlugin_EntryPoint)tcc_get_symbol(tccState, "GlumityMain");
    plugin.exitPoint = (GlumityPlugin_EntryPoint)tcc_get_symbol(tccState, "GlumityExit");
    plugin.tccState = tccState;
    if (plugin.entryPoint != nullptr)
    {
        GLUMITY_PRINT_COLOR(CON_GREEN, "Executing Script Instance -> %s [%p]\n", TCC_HEADER, scriptPath.filename().string().c_str(), plugin.entryPoint);
        g_compiledScripts[program] = plugin;
        plugin.entryPoint();
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

VOID HotReloadJITScripts()
{
    for (auto &plugin : g_compiledScripts)
    {
        plugin.second.exitPoint();
        tcc_delete((TCCState *)plugin.second.tccState);
        free(plugin.first);
    }
    g_compiledScripts.clear();

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
        GLUMITY_PRINT_COLOR(CON_RED, "Failed to manually resolve sibling dependency: %s\n", MY_PLUGIN, tccDllPath);
        return;
    }
    GLUMITY_PRINT_COLOR(CON_GREEN, "Successfully resolved dependency internally: %s\n", MY_PLUGIN, tccDllPath);

    strcat_s(libs, sizeof(libs), "tcc_libs");
    strcat_s(incs, sizeof(incs), "tcc_include");

    GetPluginDirectory(hCurrentDll, pathBuffer, sizeof(pathBuffer));
    ProcessJITScriptsFolder(pathBuffer, libs, incs);
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
}

VOID KeyboardLoop()
{
    while (1)
    {
        if ((GetAsyncKeyState(VK_END) & 0x8000) != 0)
        {
            HotReloadJITScripts();

            // Wait here until the user physically lets go of the key
            while (GetAsyncKeyState(VK_END) & 0x8000)
            {
                Sleep(10);
            }
        }
        Sleep(10);
    }
}

GLUMITYV2_PLUGIN_ENTRY
{
    Setup();
    GLUMITYV2_PLUGIN_THREADRUN(KeyboardLoop, 0);
}

GLUMITYV2_PLUGIN_EXIT
{
    MH_DisableHook(MH_ALL_HOOKS);
}