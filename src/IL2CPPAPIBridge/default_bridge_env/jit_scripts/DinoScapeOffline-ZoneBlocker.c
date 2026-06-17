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

#define MY_PLUGIN "DinoScapeOffline-ZoneBlocker"

GLUMITYV2_GAME_HOOK_TYPE(void *, MainReferences_Update_t)(struct MainReferences *);
MainReferences_Update_t MainReferences_Update_o;
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
#pragma endregion

void Setup()
{
    GLUMITYV2_DUMPER_WAITFOR_INIT(dumperExports);
    const char *targetMod = "GameAssembly.dll";
    HANDLE hmod = ForceGetHmodule(targetMod, true);

    GlumityPlugin_printf("Force loaded module: %s at: %x\n", MY_PLUGIN, targetMod, hmod);
    MainReferences_Update_o = 
        (MainReferences_Update_t)dumperExports.GlumityV2Dumper_GetFunctionPointer("MainReferences", "Update");

    GlumityPlugin_printf("MainReferencesUpdatePtr: %x\n", MY_PLUGIN, MainReferences_Update_o);

    GLUMITYV2_INIT_HOOKING(MY_PLUGIN, return)
    {
        GlumityPlugin_printf("Initialized Minhook\n", MY_PLUGIN);
    }

    GLUMITYV2_GAME_HOOK_CREATE(
        "MainReferences_Update",
        MainReferences_Update_o,
        MainReferences_Update_hook);

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