// This is an adaption of the original DinoScapeOffline mod but for it to be compiled at runtime using TCC!
#include <GlumityLib/GlumityLib.h>
#include <windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void GlumityPlugin_printf(const char *fmt, const char *head, ...);

GlumityV2DumperExports dumperExports;
static char urlReroute[256] = "localhost:8000";

HMODULE ForceGetHmodule(const char *moduleName, int isInApplicationFolder)
{
    HMODULE returnModule = GetModuleHandleA(moduleName);

    if (!returnModule)
    {
        char buff[MAX_PATH];

        if (isInApplicationFolder)
        {
            GetCurrentDirectoryA(MAX_PATH, buff);
            strcat_s(buff, MAX_PATH, "\\");
        }
        else
        {
            GetWindowsDirectoryA(buff, MAX_PATH);
            strcat_s(buff, MAX_PATH, "\\system32\\");
        }

        strcat_s(buff, MAX_PATH, moduleName);
        returnModule = LoadLibraryA(buff);
    }

    return returnModule;
}

#pragma region customTypes

struct PlayFabApiSettings__Fields
{
    struct IL2CPP_String *_ProductionEnvironmentUrl;
    PAD_TO(0x08, 0x09);
    struct IL2CPP_String *_TitleId_k__BackingField;
    struct IL2CPP_String *_VerticalName_k__BackingField;
    struct IL2CPP_String *_DeveloperSecretKey_k__BackingField;
    struct IL2CPP_String *_AdvertisingIdType_k__BackingField;
    struct IL2CPP_String *_AdvertisingIdValue_k__BackingField;
    bool _DisableAdvertising_k__BackingField;
    bool _DisableDeviceInfo_k__BackingField;
    bool _DisableFocusTimeCollection_k__BackingField;
};

struct PlayFabApiSettings
{
    void *klass;
    void *monitor;
    struct PlayFabApiSettings__Fields fields;
};

#pragma endregion

#pragma region hooks

#define MY_PLUGIN "DinoScapeOffline-OfflineServer"

GLUMITYV2_GAME_HOOK_TYPE(struct IL2CPP_String *, PlayFabApiSettings_GetFullUrl_t)(struct PlayFabApiSettings *, struct IL2CPP_String *, void **);
PlayFabApiSettings_GetFullUrl_t PlayFabApiSettings_GetFullUrl_o;
struct IL2CPP_String *PlayFabApiSettings_GetFullUrl_hook(struct PlayFabApiSettings *_this, struct IL2CPP_String *apiCall, void **getParams)
{
#ifdef DEBUG
    GlumityPlugin_printf("PlayFabApiSettings_GetFullUrl_hook called!\n", MY_PLUGIN);
#endif

    struct IL2CPP_String *res = PlayFabApiSettings_GetFullUrl_o(_this, apiCall, getParams);
    if (!res)
        return NULL;

    char *chars = IL2CPP_String_ToCString(res);
    if (!chars)
        return res;

#ifdef DEBUG
    GlumityPlugin_printf("Original result: %s\n", MY_PLUGIN, chars);
#endif

    char *path = strstr(chars, ".com");
    if (path)
        path += 4; // skip .com
    else
        path = "";

    size_t prefixLen = strlen(urlReroute);
    size_t pathLen = strlen(path);
    size_t len = prefixLen + pathLen;

    // Allocate space for header data + the UTF-16 character string (+1 for null terminator)
    size_t size = sizeof(struct IL2CPP_String) + ((len + 1) * sizeof(uint16_t));
    struct IL2CPP_String *str = (struct IL2CPP_String *)malloc(size);
    if (!str)
    {
        free(chars);
        return res;
    }

    str->klass = res->klass;
    str->monitor = NULL;
    str->fields.m_stringLength = (int32_t)len;

    // FIX: Let the compiler safely grab the exact location of the character field
    uint16_t *buffer = &str->fields.m_firstChar;

    // copy base URL
    for (size_t i = 0; i < prefixLen; i++)
        buffer[i] = (uint16_t)urlReroute[i];

    // copy path
    for (size_t i = 0; i < pathLen; i++)
        buffer[prefixLen + i] = (uint16_t)path[i];

    buffer[len] = 0; // Null-terminate UTF-16 string

    free(chars); // Free the temporary C-string wrapper
    return str;
}

void Hooks_PreInstall()
{
    char file[MAX_PATH];

    GetCurrentDirectoryA(sizeof(file), file);
    strcat(file, "\\Plugins\\newUrl.txt");

    FILE *f = fopen(file, "r");
    GlumityPlugin_printf("Trying to read new url from: %s\n", MY_PLUGIN, file);

    if (!f)
    {
        f = fopen(file, "w");
        if (!f)
        {
            GlumityPlugin_printf("Failed to create newUrl.txt, aborting hooking!\n", MY_PLUGIN);
            return;
        }

        fprintf(f, "%s", urlReroute);
        fclose(f);
        return;
    }

    fscanf(f, "%255s", urlReroute);
    fclose(f);

    GlumityPlugin_printf("urlReroute: %s\n", MY_PLUGIN, urlReroute);
}

#pragma endregion

void LaunchCustomServer()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char file[MAX_PATH];
    GetCurrentDirectoryA(sizeof(file), file);
    strcat(file, "\\Plugins\\DinoScape-Server.exe");

    // Start the child process.
    if (!CreateProcess(NULL,  // No module name (use command line)
                       file,  // Command line
                       NULL,  // Process handle not inheritable
                       NULL,  // Thread handle not inheritable
                       FALSE, // Set handle inheritance to FALSE
                       0,     // No creation flags
                       NULL,  // Use parent's environment block
                       NULL,  // Use parent's starting directory
                       &si,   // Pointer to STARTUPINFO structure
                       &pi)   // Pointer to PROCESS_INFORMATION structure
    )
    {
        char errBuff[250];
        if (Glumity_GetErrorMessage(GetLastError(), errBuff, sizeof(errBuff)))
            GlumityPlugin_printf("(%s) CreateProcess failed: %s\n", MY_PLUGIN, file, errBuff);
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void Setup()
{
    GLUMITYV2_VERIFY_DEPENDENCY("GlumityV2IL2CPPDumper");
    GLUMITYV2_DUMPER_WAITFOR_INIT(dumperExports);
    const char *targetMod = "GameAssembly.dll";
    HANDLE hmod = ForceGetHmodule(targetMod, true);

    GlumityPlugin_printf("Force loaded module: %s at: %x\n", MY_PLUGIN, targetMod, hmod);
    PlayFabApiSettings_GetFullUrl_o =
        (PlayFabApiSettings_GetFullUrl_t)dumperExports.GlumityV2Dumper_GetFunctionPointerWithPattern(hmod, "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 80 3D 1D DF F0 00 00");

    GlumityPlugin_printf("PlayFabApiSettings_GetFullUrlPtr: %x\n", MY_PLUGIN, PlayFabApiSettings_GetFullUrl_o);
    Hooks_PreInstall();

    GLUMITYV2_INIT_HOOKING(MY_PLUGIN, return)
    {
        GlumityPlugin_printf("Initialized Minhook\n", MY_PLUGIN);
    }

    GLUMITYV2_GAME_HOOK_CREATE(
        "PlayFabApiSettings_GetFullUrl",
        PlayFabApiSettings_GetFullUrl_o,
        PlayFabApiSettings_GetFullUrl_hook);

    GLUMITYV2_GAME_HOOK_ENABLE_ALL(MY_PLUGIN);

    // Launch the custom server
    LaunchCustomServer();
}

GLUMITYV2_PLUGIN_ENTRY
{
    GLUMITYV2_PLUGIN_THREADRUN(Setup, 0);
}

GLUMITYV2_PLUGIN_EXIT
{
    MH_DisableHook(MH_ALL_HOOKS);
}