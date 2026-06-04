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

#pragma once

struct Color
{
    float r;
    float g;
    float b;
    float a;
};

struct Vector3
{
    float x;
    float y;
    float z;
};

struct Vector2
{
    float x;
    float y;
};

struct MainReferences__VTable
{
    struct IL2CPP_VirtualInvokeData Equals;
    struct IL2CPP_VirtualInvokeData Finalize;
    struct IL2CPP_VirtualInvokeData GetHashCode;
    struct IL2CPP_VirtualInvokeData ToString;
};

struct MainReferences__Class
{
    PAD_TO(0x00, 0x0138);
    struct MainReferences__VTable vtable;
};

struct MainReferences__Fields
{
    PAD_TO(0x00, 0x08);
    void *LocalPlayerS;
    bool IsMobile;
    float BonusMultiplier;
    bool IsTest;
    bool UseEnglishAnyway;
    void *OutfitsN;
    void *AllEggs;
    int32_t EmeraldToAmberRatio;
    void *AllPlayerReferences;
    void *playerProfiles;
    void *AllPlayerActorIDs;
    void *AllPlayerObjectsByIDs;
    void *AllPlayerColors;
    float ShakeF;
    void *StartLands;
    void *StageBiomeBanner;
    void *StageBiomeImage;
    void *AllMetersRun;
    int32_t emeraldTotal;
    void *emeraldTotalText;
    void *emeraldTotalTextOutline;
    void *emeraldCurrentText;
    void *emeraldCurrentTextOutline;
    void *SpawnPos;
    void *Impulse;
    void *Vignette;
    float CameraZoomedOut;
    float CameraZoomedIn;
    bool MusicEnabled;
    bool ShowFPS;
    bool SoundEffectsEnabled;
    bool JoystickIsSlider;
    void *LoadingMultiplayerScreen;
    void *LoadingMultiplayerInfoText;
    void *LoadingMultiplayerDownText;
    struct MainReferences_EggInventory__Array *AllEggSlots;
    void *EggFoundPanel;
    void *EggFoundNoSpacePanel;
    void *EggAttention;
    void *UpgradeAttention;
    void *ShopHeaderr;
    void *PlayerDeathPrefab;
    void *WorldCanvas;
    void *CurrentlyUnavailablePanel;
    void *NoInternetPanel;
    void *NotLoggedInPanel;
    void *NewBiomeText;
    void *NewStageReachedUI;
    void *leaderboardMenu;
    void *EggMenu;
    struct Color SelectedColor;
    struct Color UnselectedColor;
    void *playerReferences;
    void *NP;
    void *LevelGenerator;
    struct GameObject *zoneScript;
    void *PlayerLavaParticles;
    bool IsInLava;
    void *rigid;
    struct Vector3 PlayerRotatedLeft;
    struct Vector3 PlayerRotatedRight;
    bool IsRotated;
    void *AnmPlayer;
    void *AnmDino;
    void *x2GemsButton;
    void *x2GemsPanel;
    void *scoreManager;
    void *multiplayerUI;
    void *AllIAPDisabledPanels;
    void *Amber180Price;
    void *Amber500Price;
    void *Amber1100Price;
    void *Amber3000Price;
    void *Amber6500Price;
    void *Amber14000Price;
    void *NetworkMsgText;
    void *inventoryMenu;
    void *selectItemMenu;
    void *itemUpgradeMenu;
    void *tandardSpear;
    void *IG;
    void *MobileJumpScript;
    void *LeaveButton;
    void *SkinRemoved;
    void *OnlineText;
    void *OnlineStatusRenderer;
    void *PlayerName;
    void *backgroundLoop;
    void *AdCompletedAmberPanel;
    void *EmeraldsBought;
    void *EmeraldsBought2500;
    void *MenuAmberParentObject;
    void *EmeraldInGameTextParent;
    void *NotEnoughAmber;
    void *AmberText;
    void *audioManager;
    void *setStatusImage;
    void *Slidetext;
    void *Taptext;
    void *JumpHoldText;
    void *TutorialText;
    void *ArrowsParent;
    void *LeftArrow;
    void *RightArrow;
    void *Music;
    void *FPSText;
    void *JoystickToggleButton;
    void *MusicToggleButton;
    void *FPSToggleButton;
    void *SoundEffectToggleButton;
    void *MenuScript;
    void *TapToPlayPanel;
    void *TapToPlayText;
    void *PauseButton;
    void *Joystick;
    void *MeterScore;
    void *JumpButton;
    void *StartMenu;
    void *LocalPlayer;
    void *Canvas;
    void *Zone;
    void *camera;
    void *CMCamera;
    void *CMCameraComposer;
    void *playerMovement;
    void *adStart;
    void *AdFinishedx2Gems;
    void *BonusEmeraldAmount;
    void *deathGUI;
    void *pauseButton;
    struct Vector2 StandardArrowPos;
    struct Vector2 StandardSliderPos;
    float XChangeArrows;
    float XChangeSlider;
    float ScaleChangeArrows;
    float ScaleChangeSlider;
    struct IL2CPP_String *StoreIDAndroid;
    struct IL2CPP_String *StoreIDIOS;
    struct IL2CPP_String *rewardedVideo;
    int32_t LastRequestedEmeralds;
    void *ThisOutfits;
    bool GameStarted;
    int32_t StateCurrency;
    bool LastEmeraldPlayfabChangeRequested;
    bool LastAmberPlayfabChangeRequested;
    int32_t LastEggID;
    void *LastFoundEgg;
    void *AddSpriteAll;
    bool RahmenAusgewu00E4hlt;
    float TransparencyDistance;
    float ZoomSpeed;
    struct IL2CPP_String *MainScene;
    int32_t ConvertAmberComplete;
    int32_t ActiveOutfitIDPlayerHead;
    int32_t ActiveOutfitIDPlayerHeadSkin;
    int32_t ActiveOutfitIDDinoHead;
    int32_t ActiveOutfitIDDinoHeadSkin;
    int32_t ActiveOutfitIDWeapon;
    int32_t ActiveOutfitIDWeaponSkin;
};

struct MainReferences
{
    struct MainReferences__Class *klass;
    void *monitor;
    struct MainReferences__Fields fields;
};

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

struct DinoInfo__Fields
{
    // 0x00 to 0x50 is exactly 0x50 bytes of padding
    PAD_TO(0x00, 0x50);

    bool isSelected; // Offset 0x50
    bool owned;      // Offset 0x51

    // Preloader is at 0x58. Current offset is 0x52.
    PAD_TO(0x52, 0x58);
    void *Preloader; // Offset 0x58
};

struct DinoInfo
{
    void *vtable;
    void *monitor;
    struct DinoInfo__Fields fields; // starts at 0x10 usually, but dump says the class is 0x70
};

struct ZoneMove__VTable
{
    struct IL2CPP_VirtualInvokeData Equals;
    struct IL2CPP_VirtualInvokeData Finalize;
    struct IL2CPP_VirtualInvokeData GetHashCode;
    struct IL2CPP_VirtualInvokeData ToString;
};

struct ZoneMove__Class
{
    struct IL2CPP_Class_0 _0;
    struct Il2CppRuntimeInterfaceOffsetPair *interfaceOffsets;
    struct ZoneMove__StaticFields *static_fields;
    union Il2CppRGCTXData const *rgctx_data;
    PAD_TO(0x0c8, 0x0138);
    struct ZoneMove__VTable vtable;
};

struct ZoneMove__Fields
{
    PAD_TO(0x00, 0x018);

    float MoveSpeed;
    struct Vector3 StartPos;
    bool DontMove;
    float TimeToLast;
    float StartX;
    float elapsedTime;
};

struct ZoneMove
{
    struct ZoneMove__Class *klass;
    struct MonitorData *monitor;
    struct ZoneMove__Fields fields;
};

#pragma endregion

#pragma region hooks

#define MY_PLUGIN "DinoScapeOffline"

GLUMITYV2_GAME_HOOK_TYPE(BOOL, DinoInfo_CheckIfOwned_t)(struct DinoInfo *, struct IL2CPP_MethodInfo *);
DinoInfo_CheckIfOwned_t DinoInfo_CheckIfOwned_o;

GLUMITYV2_GAME_HOOK_TYPE(void *, MainReferences_Update_t)(struct MainReferences *);
MainReferences_Update_t MainReferences_Update_o;

GLUMITYV2_GAME_HOOK_TYPE(struct IL2CPP_String *, PlayFabApiSettings_GetFullUrl_t)(struct PlayFabApiSettings *, struct IL2CPP_String *, void **);
PlayFabApiSettings_GetFullUrl_t PlayFabApiSettings_GetFullUrl_o;

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

#define DEBUG

struct IL2CPP_String *PlayFabApiSettings_GetFullUrl_hook(struct PlayFabApiSettings *_this, struct IL2CPP_String *apiCall, void **getParams)
{
#ifdef DEBUG
    GlumityPlugin_printf("PlayFabApiSettings_GetFullUrl_hook called!\n", MY_PLUGIN);
#endif

    struct IL2CPP_String *res = PlayFabApiSettings_GetFullUrl_o(_this, apiCall, getParams);
    if (!res) return NULL;

    char *chars = IL2CPP_String_ToCString(res);
    if (!chars) return res;

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
    if (!str) {
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
    GLUMITYV2_DUMPER_WAITFOR_INIT(dumperExports);
    const char *targetMod = "GameAssembly.dll";
    HANDLE hmod = ForceGetHmodule(targetMod, true);

    GlumityPlugin_printf("Force loaded module: %s at: %x\n", MY_PLUGIN, targetMod, hmod);
    void *GetFullUrlPtr =
        dumperExports.GlumityV2Dumper_GetFunctionPointerWithPattern(hmod, "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 80 3D 1D DF F0 00 00");

    GlumityPlugin_printf("GetFullUrlPtr: %x\n", MY_PLUGIN, GetFullUrlPtr);

    void *MainReferencesUpdatePtr =
        dumperExports.GlumityV2Dumper_GetFunctionPointer("MainReferences", "Update");
    GlumityPlugin_printf("MainReferencesUpdatePtr: %x\n", MY_PLUGIN, MainReferencesUpdatePtr);

    void *CheckIfOwnedPtr =
        dumperExports.GlumityV2Dumper_GetFunctionPointer("DinoInfo", "CheckIfOwned");

    GlumityPlugin_printf("CheckIfOwnedPtr: %x\n", MY_PLUGIN, CheckIfOwnedPtr);

    if (GetFullUrlPtr)
    {
        // Make a hook
        Hooks_Install(GetFullUrlPtr, MainReferencesUpdatePtr, CheckIfOwnedPtr);
    }

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