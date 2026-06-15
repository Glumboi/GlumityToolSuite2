// TODOS:
// -- add new il2cppinternal api to the template

#include <windows.h>
#include <GlumityLib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "il2cppInternal.h"

#define MY_PLUGIN "IL2CPPGUILookup"

GlumityV2DumperExports dumperExports;

const char g_szClassName[] = "GlumityGuiClass";

int g_showWindow = 0;

#define IDC_INPUT_TEXT 101
#define IDC_BUTTON_LOOKUP 102
#define IDC_OUTPUT_TEXT 103
#define IDC_BYTES_TEXT 104
#define IDC_HOTKEY_SHOW_HIDE 105
#define IDC_ADDRESS_LIST 106
#define IDC_FILTER_TEXT 107

HWND g_hInputEdit = NULL;
HWND g_hLookupBtn = NULL;
HWND g_hOutputEdit = NULL;
HWND g_hBytesEdit = NULL;
HWND g_hListBox = NULL;
HWND g_hFilterEdit = NULL;

DWORD g_toggleKey = VK_INSERT;

char **g_CachedFunctionList = NULL;
int g_CachedFunctionCount = 0;

void CacheAllFunctions()
{
    if (dumperExports.GlumityV2Dumper_GetEverything != NULL)
    {
        char **functionList = dumperExports.GlumityV2Dumper_GetEverything();
        if (functionList != NULL)
        {
            int count = 0;
            while (functionList[count] != NULL)
            {
                count++;
            }

            g_CachedFunctionList = (char **)malloc((count + 1) * sizeof(char *));
            if (g_CachedFunctionList != NULL)
            {
                for (int i = 0; i < count; i++)
                {
                    g_CachedFunctionList[i] = _strdup(functionList[i]);
                    free(functionList[i]);
                }
                g_CachedFunctionList[count] = NULL;
                g_CachedFunctionCount = count;
            }
            free(functionList);
        }
    }
}

void ApplyListFilter(const char *filterText)
{
    if (g_hListBox == NULL || g_CachedFunctionList == NULL)
        return;

    SendMessageA(g_hListBox, LB_RESETCONTENT, 0, 0);

    size_t filterLen = strlen(filterText);

    char *lowerFilter = _strdup(filterText);
    if (lowerFilter != NULL)
    {
        for (size_t i = 0; i < filterLen; i++)
        {
            lowerFilter[i] = (char)tolower((unsigned char)lowerFilter[i]);
        }

        for (int i = 0; i < g_CachedFunctionCount; i++)
        {
            if (g_CachedFunctionList[i] == NULL)
                continue;

            if (filterLen == 0)
            {
                SendMessageA(g_hListBox, LB_ADDSTRING, 0, (LPARAM)g_CachedFunctionList[i]);
            }
            else
            {
                char *lowerItem = _strdup(g_CachedFunctionList[i]);
                if (lowerItem != NULL)
                {
                    size_t itemLen = strlen(lowerItem);
                    for (size_t j = 0; j < itemLen; j++)
                    {
                        lowerItem[j] = (char)tolower((unsigned char)lowerItem[j]);
                    }

                    if (strstr(lowerItem, lowerFilter) != NULL)
                    {
                        SendMessageA(g_hListBox, LB_ADDSTRING, 0, (LPARAM)g_CachedFunctionList[i]);
                    }
                    free(lowerItem);
                }
            }
        }
        free(lowerFilter);
    }
}

void FreeCachedFunctions()
{
    if (g_CachedFunctionList != NULL)
    {
        for (int i = 0; i < g_CachedFunctionCount; i++)
        {
            if (g_CachedFunctionList[i] != NULL)
            {
                free(g_CachedFunctionList[i]);
            }
        }
        free(g_CachedFunctionList);
        g_CachedFunctionList = NULL;
    }
}

void PerformAddressLookup(const char *query)
{
    char className[64] = {0};
    char methodName[64] = {0};
    size_t start = 0;
    size_t classLen = 0;
    size_t methodLen = 0;
    void *funcPtr = NULL;

    while (query[start] != '\0' && isspace((unsigned char)query[start]))
    {
        start++;
    }

    if (query[start] == '\0')
    {
        SetWindowTextA(g_hOutputEdit, "Error: Please enter a string to look up.");
        SetWindowTextA(g_hBytesEdit, "");
        return;
    }

    GlumityPlugin_printf("Searching for query: %s\n", MY_PLUGIN, query);

    while (query[start + classLen] != '\0' && !isspace((unsigned char)query[start + classLen]))
    {
        classLen++;
    }

    if (classLen == 0 || classLen >= sizeof(className))
    {
        SetWindowTextA(g_hOutputEdit, "Error: Invalid class name format.");
        SetWindowTextA(g_hBytesEdit, "");
        return;
    }

    memcpy(className, &query[start], classLen);
    className[classLen] = '\0';
    start += classLen;

    while (query[start] != '\0' && isspace((unsigned char)query[start]))
    {
        start++;
    }

    while (query[start + methodLen] != '\0' && !isspace((unsigned char)query[start + methodLen]))
    {
        methodLen++;
    }

    if (methodLen == 0 || methodLen >= sizeof(methodName))
    {
        SetWindowTextA(g_hOutputEdit, "Error: Format must be 'ClassName MethodName'.");
        SetWindowTextA(g_hBytesEdit, "");
        return;
    }

    memcpy(methodName, &query[start], methodLen);
    methodName[methodLen] = '\0';

    if (dumperExports.GlumityV2Dumper_GetFunctionPointer == NULL)
    {
        SetWindowTextA(g_hOutputEdit, "Error: Glumity Dumper export table not loaded/linked.");
        SetWindowTextA(g_hBytesEdit, "");
        return;
    }

    funcPtr = dumperExports.GlumityV2Dumper_GetFunctionPointer(className, methodName);

    char bytesRes[512] = {0};
    if (funcPtr != NULL)
    {
        unsigned char *codeBytes = (unsigned char *)funcPtr;
        if (!IsBadReadPtr(funcPtr, 16))
        {
            sprintf_s(bytesRes, sizeof(bytesRes),
                      "Raw Bytes: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
                      codeBytes[0], codeBytes[1], codeBytes[2], codeBytes[3],
                      codeBytes[4], codeBytes[5], codeBytes[6], codeBytes[7],
                      codeBytes[8], codeBytes[9], codeBytes[10], codeBytes[11],
                      codeBytes[12], codeBytes[13], codeBytes[14], codeBytes[15]);
        }
        else
        {
            sprintf_s(bytesRes, sizeof(bytesRes), "Raw Bytes: [Memory Unreadable]");
        }
    }
    else
    {
        sprintf_s(bytesRes, sizeof(bytesRes), "Raw Bytes: [Function Not Found]");
    }
    SetWindowTextA(g_hBytesEdit, bytesRes);

    size_t resBufferSize = 8192;
    char *res = (char *)malloc(resBufferSize);
    if (res != NULL)
    {
        sprintf_s(res, resBufferSize,
                  "Lookup Result:\r\n"
                  "----------------------------------------\r\n"
                  "Class: %s\r\n"
                  "Method: %s\r\n"
                  "Pointer: 0x%p\r\n\r\n"
                  "JIT Template:\r\n"
                  "----------------------------------------\r\n"
                  "#include <GlumityLib/GlumityLib.h>\r\n\r\n"
                  "#define MY_PLUGIN \"MY_PLUGIN\"\r\n\r\n"
                  "GlumityV2DumperExports dumperExports;\r\n"
                  "struct %s;\r\n\r\n"
                  "GLUMITYV2_GAME_HOOK_TYPE(void*, %s_%s_t)(struct %s*);\r\n"
                  "%s_%s_t %s_%s_o;\r\n\r\n"
                  "void* %s_%s_hook(struct %s* _this)\r\n"
                  "{\r\n"
                  "    return %s_%s_o(_this);\r\n"
                  "}\r\n\r\n"
                  "// Installation:\r\n"
                  "// Current pointer address found: 0x%p\r\n"
                  "// (Assigned dynamically from dumper structure table below)\r\n\r\n"
                  "VOID OnHookInitFail()\r\n"
                  "{\r\n"
                  "    GLUMITY_PRINT_COLOR(CON_RED, \"Failed to initialize Minhook!\\n\", MY_PLUGIN); \r\n"
                  "}\r\n\r\n"
                  "VOID Setup()\r\n"
                  "{\r\n"
                  "    GLUMITYV2_VERIFY_DEPENDENCY(\"GlumityV2IL2CPPDumper\");\r\n"
                  "    GLUMITYV2_DUMPER_WAITFOR_INIT(dumperExports);\r\n\r\n"
                  "    %s_%s_o = (%s_%s_t)dumperExports.GlumityV2Dumper_GetFunctionPointer(\"%s\", \"%s\");\r\n\r\n"
                  "    GLUMITYV2_INIT_HOOKING(MY_PLUGIN, OnHookInitFail)\r\n"
                  "    {\r\n"
                  "        GLUMITY_PRINT_COLOR(CON_GREEN, \"Initialized Minhook\\n\", MY_PLUGIN);\r\n"
                  "    }\r\n\r\n"
                  "    GLUMITYV2_GAME_HOOK_CREATE(\r\n"
                  "        \"%s_%s\",\r\n"
                  "        %s_%s_o,\r\n"
                  "        %s_%s_hook);\r\n\r\n"
                  "    GLUMITYV2_GAME_HOOK_ENABLE_ALL(MY_PLUGIN);\r\n"
                  "}\r\n\r\n"
                  "GLUMITYV2_PLUGIN_ENTRY\r\n"
                  "{\r\n"
                  "    GLUMITYV2_PLUGIN_THREADRUN(Setup, 0);\r\n"
                  "}\r\n\r\n"
                  "GLUMITYV2_PLUGIN_EXIT\r\n"
                  "{\r\n"
                  "    GLUMITYV2_GAME_HOOK_CLEAN_ALL();\r\n"
                  "}",
                  className, methodName, funcPtr,                                      // Header outputs
                  className,                                                           // struct %s;
                  className, methodName, className,                                    // GLUMITYV2_GAME_HOOK_TYPE
                  className, methodName, className, methodName,                        // %s_%s_t %s_%s_o;
                  className, methodName, className,                                    // void* %s_%s_hook(struct %s* _this)
                  className, methodName,                                               // return %s_%s_o(_this);
                  funcPtr,                                                             // Current pointer address found: 0x%p
                  className, methodName, className, methodName, className, methodName, // Setup pointer assignment lines
                  className, methodName,                                               // GLUMITYV2_GAME_HOOK_CREATE label
                  className, methodName,                                               // original pointer target
                  className, methodName);                                              // hook pointer target

        SetWindowTextA(g_hOutputEdit, res);
        free(res);
    }
}

VOID ToggleWindowVisibility(HWND hwnd)
{
    if (g_showWindow == 1)
        g_showWindow = 0;
    else
        g_showWindow = 1;
    ShowWindow(hwnd, g_showWindow);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        g_hInputEdit = CreateWindowExA(
            WS_EX_CLIENTEDGE, "EDIT", "",
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
            20, 20, 240, 25,
            hwnd, (HMENU)IDC_INPUT_TEXT, GetModuleHandle(NULL), NULL);
        SendMessage(g_hInputEdit, WM_SETTEXT, 0, (LPARAM) "Enter search term...");

        g_hLookupBtn = CreateWindowA(
            "BUTTON", "Lookup Address",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 270, 18, 110, 28, hwnd, (HMENU)IDC_BUTTON_LOOKUP, GetModuleHandle(NULL), NULL);
        g_hOutputEdit = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_VSCROLL, 20, 60, 360, 245, hwnd, (HMENU)IDC_OUTPUT_TEXT, GetModuleHandle(NULL), NULL);
        SetWindowTextA(g_hOutputEdit, "Results will appear here...");
        g_hBytesEdit = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 20, 315, 360, 50, hwnd, (HMENU)IDC_BYTES_TEXT, GetModuleHandle(NULL), NULL);
        SetWindowTextA(g_hBytesEdit, "Hex Bytes will appear here...");
        g_hFilterEdit = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 395, 20, 230, 25, hwnd, (HMENU)IDC_FILTER_TEXT, GetModuleHandle(NULL), NULL);
        SendMessage(g_hFilterEdit, WM_SETTEXT, 0, (LPARAM) "");
        g_hListBox = CreateWindowExA(WS_EX_CLIENTEDGE, "LISTBOX", "", WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL | WS_BORDER, 395, 55, 230, 310, hwnd, (HMENU)IDC_ADDRESS_LIST, GetModuleHandle(NULL), NULL);
        CacheAllFunctions();
        ApplyListFilter("");
        break;
    }
    case WM_SIZING:
    {
        RECT *rect = (RECT *)lParam;
        int currentWidth = rect->right - rect->left;
        if (currentWidth < 450)
        {
            if (wParam == WMSZ_LEFT || wParam == WMSZ_TOPLEFT || wParam == WMSZ_BOTTOMLEFT)
                rect->left = rect->right - 450;
            else
                rect->right = rect->left + 450;
        }
        rect->bottom = rect->top + 425;
        return TRUE;
    }
    case WM_SIZE:
    {
        int newClientWidth = LOWORD(lParam);
        if (g_hListBox != NULL && g_hFilterEdit != NULL)
        {
            int listLeft = 395;
            int listPaddingRight = 20;
            int filterTop = 20;
            int filterHeight = 25;
            int listTop = 55;
            int listHeight = 310;
            int newWidth = newClientWidth - listLeft - listPaddingRight;
            if (newWidth < 50)
                newWidth = 50;
            MoveWindow(g_hFilterEdit, listLeft, filterTop, newWidth, filterHeight, TRUE);
            MoveWindow(g_hListBox, listLeft, listTop, newWidth, listHeight, TRUE);
        }
        break;
    }
    case WM_COMMAND:
    {
        if (LOWORD(wParam) == IDC_BUTTON_LOOKUP)
        {
            int len = GetWindowTextLengthA(g_hInputEdit);
            if (len > 0)
            {
                char *buff = (char *)malloc((size_t)len + 1);
                if (!buff)
                    return 0;
                GetWindowTextA(g_hInputEdit, buff, len + 1);
                PerformAddressLookup(buff);
                free(buff);
            }
            else
            {
                PerformAddressLookup("");
            }
        }
        else if (LOWORD(wParam) == IDC_FILTER_TEXT && HIWORD(wParam) == EN_CHANGE)
        {
            int len = GetWindowTextLengthA(g_hFilterEdit);
            char *filterBuff = (char *)malloc((size_t)len + 1);
            if (filterBuff != NULL)
            {
                GetWindowTextA(g_hFilterEdit, filterBuff, len + 1);
                ApplyListFilter(filterBuff);
                free(filterBuff);
            }
        }
        else if (LOWORD(wParam) == IDC_ADDRESS_LIST && HIWORD(wParam) == LBN_SELCHANGE)
        {
            int idx = (int)SendMessageA(g_hListBox, LB_GETCURSEL, 0, 0);
            if (idx != LB_ERR)
            {
                int textLen = (int)SendMessageA(g_hListBox, LB_GETTEXTLEN, idx, 0);
                if (textLen > 0)
                {
                    char *listBuffer = (char *)malloc((size_t)textLen + 1);
                    if (listBuffer)
                    {
                        SendMessageA(g_hListBox, LB_GETTEXT, idx, (LPARAM)listBuffer);
                        SetWindowTextA(g_hInputEdit, listBuffer);
                        PerformAddressLookup(listBuffer);
                        free(listBuffer);
                    }
                }
            }
        }
        break;
    }
    case WM_CLOSE:
        ToggleWindowVisibility(hwnd);
        break;
    case WM_DESTROY:
        UnregisterHotKey(hwnd, IDC_HOTKEY_SHOW_HIDE);
        FreeCachedFunctions();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    return 0;
}

DWORD WINAPI WindowThread(LPVOID lpParam)
{
    GLUMITYV2_VERIFY_DEPENDENCY("GlumityV2IL2CPPDumper");
    GLUMITYV2_DUMPER_WAITFOR_INIT(dumperExports);

    HINSTANCE hInstance = (HINSTANCE)lpParam;
    if (hInstance == NULL)
    {
        hInstance = GetModuleHandle(NULL);
    }
    WNDCLASSEXA wc = {0};
    HWND hwnd;
    MSG Msg;
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = g_szClassName;
    if (!RegisterClassExA(&wc))
        return 0;
    hwnd = CreateWindowExA(WS_EX_TOPMOST, g_szClassName, "Glumity Address Lookup", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 660, 425, NULL, NULL, hInstance, NULL);
    if (hwnd == NULL)
        return 0;
    g_showWindow = 1;
    ShowWindow(hwnd, SW_HIDE);
    UpdateWindow(hwnd);
    GlumityPlugin_printf("Initialized the window (press insert to show)!\n", MY_PLUGIN);

    BOOL wasKeyPressed = FALSE; // Asynchronous loop checking state updates safely alongside standard window scheduling messages
    while (TRUE)
    {
        // Check if there are messages waiting for the UI elements
        if (PeekMessageA(&Msg, NULL, 0, 0, PM_REMOVE))
        {
            if (Msg.message == WM_QUIT)
                break;
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        else
        {
            // Thread yield interval to keep CPU cycles low when idling
            Sleep(10);
        }
        //  key detection out of the game focus locks
        BOOL isKeyDown = (GetAsyncKeyState(g_toggleKey) & 0x8000) != 0;
        if (isKeyDown && !wasKeyPressed)
        {
            ToggleWindowVisibility(hwnd);
        }
        wasKeyPressed = isKeyDown;
    }
    UnregisterClassA(g_szClassName, hInstance);
    return Msg.wParam;
}

GLUMITYV2_PLUGIN_ENTRY { GLUMITYV2_PLUGIN_THREADRUN(WindowThread, 0); }
GLUMITYV2_PLUGIN_EXIT {}