#pragma once

#include <stdint.h>
#include <GlumityLib.h>
#include <Minhook.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "customTypes.h"

#define MY_PLUGIN "DinoScapeOffline"

GLUMITYV2_GAME_HOOK_TYPE(BOOL, DinoInfo_CheckIfOwned_t)(struct DinoInfo *, struct IL2CPP_MethodInfo *);
DinoInfo_CheckIfOwned_t DinoInfo_CheckIfOwned_o;
BOOL DinoInfo_CheckIfOwned_hook(struct DinoInfo *_this, struct IL2CPP_MethodInfo *mi);

GLUMITYV2_GAME_HOOK_TYPE(void *, MainReferences_Update_t)(struct MainReferences *);
MainReferences_Update_t MainReferences_Update_o;
void MainReferences_Update_hook(struct MainReferences *_this);

GLUMITYV2_GAME_HOOK_TYPE(struct IL2CPP_String *, PlayFabApiSettings_GetFullUrl_t)(struct PlayFabApiSettings *, struct IL2CPP_String *, void **);
PlayFabApiSettings_GetFullUrl_t PlayFabApiSettings_GetFullUrl_o;
struct IL2CPP_String *PlayFabApiSettings_GetFullUrl_hook(struct PlayFabApiSettings *_this, struct IL2CPP_String *apiCall, void **getParams);

static char urlReroute[256] = "localhost:8000";

void Hooks_PreInstall();
void Hooks_Install(void *PlayFabApiSettingsGetFullUrlPtr, void *MainReferencesUpdatePtr, void *DinoInfoCheckIfOwnedPtr);
