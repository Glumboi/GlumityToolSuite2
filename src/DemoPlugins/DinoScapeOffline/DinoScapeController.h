#pragma once
#include <GlumityLib.h>
#include <stdbool.h>

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
#include <XInput.h>
#pragma comment(lib, "xinput.lib")
#else
#include <XInput.h>
#pragma comment(lib, "xinput9_1_0.lib")
#endif

// TODO: Finish, or not finish. Might be too much for me to work on
// Basic movement may be quite easy to do with just hooking into the players update code
// but I would like to make menus navigateable as well

#define DINOSCAPE_DEFAULT_CONTROLLER_INDEX 0
#define MAX_CONTROLLERS 1 // XInput *can* use up to 4 controllers, but we only want one
#define CONTROLLER_PLAYER1 0
#define INPUT_DEADZONE (0.24f * FLOAT(0x7FFF)) // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.

typedef struct
{
    XINPUT_STATE state;
    bool bConnected;
} CONTROLLER_STATE;

CONTROLLER_STATE g_Controllers[MAX_CONTROLLERS];
bool g_bDeadZoneOn;

HRESULT UpdateControllerState();