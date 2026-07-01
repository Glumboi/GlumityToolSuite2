#include "DinoScapeController.h"

bool g_bDeadZoneOn = true;

HRESULT UpdateControllerState()
{
    DWORD dwResult;
    for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
    {
        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState(i, &g_Controllers[i].state);

        if (dwResult == ERROR_SUCCESS)
            g_Controllers[i].bConnected = true;
        else
            g_Controllers[i].bConnected = false;
    }

    return S_OK;
}