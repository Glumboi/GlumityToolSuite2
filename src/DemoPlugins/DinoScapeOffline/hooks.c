#include "hooks.h"

BOOL DinoInfo_CheckIfOwned_hook(struct DinoInfo *_this, struct IL2CPP_MethodInfo *mi)
{
    return true; // Force unlock all dinos!
}

void MainReferences_Update_hook(struct MainReferences *_this)
{
    // Demonstrati on of invoking a game function
    // struct IL2CPP_String *res = IL2CPP_InvokeMethod(_this->klass, _this->klass->vtable.ToString.method->name, _this, NULL);
    // char *resConverted = IL2CPP_String_ToCString(res);
    // if (resConverted)
    // {
    //     GlumityPlugin_printf("Invoked ToString [%s]!\n", MY_PLUGIN, resConverted);
    //     free(resConverted);
    // }

    // force off zone example
    struct ZoneMove *z = (struct ZoneMove *)_this->fields.zoneScript;
    static bool wasPressed = false;
    bool isCurrentlyDown = GetAsyncKeyState(VK_F2) & 0x8000;
    if (z && isCurrentlyDown && !wasPressed)
    {
        z->fields.DontMove = !z->fields.DontMove;
        GlumityPlugin_printf("Set zone DontMove to: %d!\n", MY_PLUGIN, z->fields.DontMove);
    }
    wasPressed = isCurrentlyDown;
    MainReferences_Update_o(_this);
}


struct IL2CPP_String *PlayFabApiSettings_GetFullUrl_hook(struct PlayFabApiSettings *_this, struct IL2CPP_String *apiCall, void **getParams)
{

#ifdef DEBUG
    GlumityPlugin_printf("PlayFabApiSettings_GetFullUrl_hook called!\n", MY_PLUGIN);
#endif

    struct IL2CPP_String *res = PlayFabApiSettings_GetFullUrl_o(_this, apiCall, getParams);
    char *chars = IL2CPP_String_ToCString(res);

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

    size_t size = sizeof(struct IL2CPP_String) + (len * sizeof(uint16_t));
    struct IL2CPP_String *str = malloc(size);
    if (!str)
        return res;

    str->klass = res->klass;
    str->monitor = NULL;
    str->fields.m_stringLength = (int32_t)len;

    uint16_t *buffer = &str->fields.m_firstChar;

    // copy base URL
    for (size_t i = 0; i < prefixLen; i++)
        buffer[i] = (uint16_t)urlReroute[i];
    // copy path
    for (size_t i = 0; i < pathLen; i++)
        buffer[prefixLen + i] = (uint16_t)path[i];

    free(chars);
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

    fscanf(f, "%255s", urlReroute); // adjust size
    fclose(f);
}

void Hooks_Install(void *PlayFabApiSettingsGetFullUrlPtr, void *MainReferencesUpdatePtr, void *DinoInfoCheckIfOwnedPtr)
{
    Hooks_PreInstall();

    GLUMITYV2_INIT_HOOKING(MY_PLUGIN, return)
    {
        GlumityPlugin_printf("Initialized Minhook\n", MY_PLUGIN);
    }

    PlayFabApiSettings_GetFullUrl_o = (PlayFabApiSettings_GetFullUrl_t)PlayFabApiSettingsGetFullUrlPtr;

    GLUMITYV2_GAME_HOOK_CREATE(
        "PlayFabApiSettings_GetFullUrl",
        PlayFabApiSettings_GetFullUrl_o,
        PlayFabApiSettings_GetFullUrl_hook);

    MainReferences_Update_o = (MainReferences_Update_t)MainReferencesUpdatePtr;

    GLUMITYV2_GAME_HOOK_CREATE(
        "MainReferences_Update",
        MainReferences_Update_o,
        MainReferences_Update_hook);

    DinoInfo_CheckIfOwned_o = (DinoInfo_CheckIfOwned_t)DinoInfoCheckIfOwnedPtr;

    GLUMITYV2_GAME_HOOK_CREATE(
        "DinoInfo_CheckIfOwned",
        DinoInfo_CheckIfOwned_o,
        DinoInfo_CheckIfOwned_hook);

    GLUMITYV2_GAME_HOOK_ENABLE_ALL(MY_PLUGIN);
}
