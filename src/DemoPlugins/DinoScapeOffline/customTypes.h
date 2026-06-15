#pragma once

#include <stdint.h>
#include <stdlib.h>

#include <GlumityLib.h>
#include <assert.h>

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
    VirtualInvokeData Equals;
    VirtualInvokeData Finalize;
    VirtualInvokeData GetHashCode;
    VirtualInvokeData ToString;
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

static_assert(sizeof(struct MainReferences__Fields) == 0x04a0, "Struct size mismatch!");

struct MainReferences
{
    struct MainReferences__Class *klass;
    void *monitor;
    struct MainReferences__Fields fields;
};

static_assert(sizeof(struct MainReferences) == 0x04b0, "Struct size mismatch!");

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
    struct DinoInfo__Fields fields; // starts at 0x10
};

struct ZoneMove__VTable
{
    VirtualInvokeData Equals;
    VirtualInvokeData Finalize;
    VirtualInvokeData GetHashCode;
    VirtualInvokeData ToString;
};

struct ZoneMove__Class
{
    struct Il2CppGenericClass _0;
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
