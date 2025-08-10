#include <Windows.h>
#include <stdio.h>

#include "Exports.h" // Contains globalLoader

#include "PluginLoader.h"

int main()
{
    if (AllocConsole())
        freopen("CONOUT$", "w", stdout);

    SetConsoleOutputCP(CP_UTF8);
    Glumity_printf("Console initialized, loading plugins...\n");

    globalLoader = GlumityPluginLoader_Create();
    GlumityPluginLoader_LoadAllPlugins(&globalLoader);
    return 0;
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL, // handle to DLL module
    DWORD fdwReason,    // reason for calling function
    LPVOID lpvReserved) // reserved
{
    // Perform actions based on the reason for calling.
    HANDLE hThread = NULL;
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // main();
        hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
        CloseHandle(hThread);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        GlumityPluginLoader_Destroy(&globalLoader);
        break;
    }
    return TRUE;
}