#pragma once

#include <stdint.h>
#include <GlumityLib.h>
#include <Minhook.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "customTypes.h"

#define MY_PLUGIN "DinoScapeOffline"

typedef struct String *(*PlayFabApiSettings_GetFullUrl_t)(void *, struct String *, void **);
PlayFabApiSettings_GetFullUrl_t PlayFabApiSettings_GetFullUrl_o;

static char *urlReroute = "http://localhost:8000";

struct String *PlayFabApiSettings_GetFullUrl_hook(void *__this, struct String *apiCall, void **getParams)
{
#ifdef DEBUG

    GlumityPlugin_printf("PlayFabApiSettings_GetFullUrl called!\n", MY_PLUGIN);
#endif

    struct String *res = PlayFabApiSettings_GetFullUrl_o(__this, apiCall, getParams);

    char *chars = ToCString(res);
    char *path = strstr(chars, ".com");
    if (path)
        path += 4; // skip .com
    else
        path = "";

    size_t prefixLen = strlen(urlReroute);
    size_t pathLen = strlen(path);
    size_t len = prefixLen + pathLen;

    size_t size = sizeof(struct String) + (len * sizeof(uint16_t));
    struct String *str = malloc(size);
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

#ifdef DEBUG
    GlumityPlugin_printf("original: %s\n", MY_PLUGIN, chars);
    GlumityPlugin_printf("spoofed: %s\n", MY_PLUGIN, ToCString(str));
#endif

    free(chars);
    return str;
}

// Config stuff
void Hooks_PreInstall()
{
    char *file[MAX_PATH];
    GetCurrentDirectoryA(sizeof(file), file);
    strcat(file, "\\Plugins\\newUrl.txt");
    FILE *f = fopen(file, "r");
    GlumityPlugin_printf("Trying to read new url from: %s\n", MY_PLUGIN, file);
    if (!f)
    {
        FILE *f = fopen(file, "w");
        if (!f)
        {
            GlumityPlugin_printf("Failed to create newUrl.txt, aborting hooking!\n", MY_PLUGIN);
            return;
        }

        fprintf(f, "%s", urlReroute);
        fclose(f);
        return;
    }
    fscanf(f, "%s", urlReroute);
    fclose(f);
}

void Hooks_Install(void *PlayFabApiSettings_GetFullUrl_ptr)
{
    Hooks_PreInstall();

    GLUMITYV2_INIT_HOOKING(MY_PLUGIN, return)
    {
        GlumityPlugin_printf("Initialized Minhook\n", MY_PLUGIN);
    }

    PlayFabApiSettings_GetFullUrl_o = (PlayFabApiSettings_GetFullUrl_t)PlayFabApiSettings_GetFullUrl_ptr;

    GLUMITYV2_GAME_HOOK_CREATE(
        "PlayFabApiSettings_GetFullUrl",
        PlayFabApiSettings_GetFullUrl_o,
        PlayFabApiSettings_GetFullUrl_hook);

    GLUMITYV2_GAME_HOOK_ENABLE_ALL(MY_PLUGIN);
}
