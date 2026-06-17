
#include <windows.h>
#include <GlumityLib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <vector>
#include <string>

#include "il2cppInternal.h"

#define MY_PLUGIN "IL2CPPGUILookup"

GlumityV2DumperExports dumperExports;

const wchar_t g_szClassName[] = L"GlumityGuiClass";

int g_showWindow = 0;

#define IDC_INPUT_TEXT 101
#define IDC_BUTTON_LOOKUP 102
#define IDC_OUTPUT_TEXT 103
#define IDC_BYTES_TEXT 104
#define IDC_HOTKEY_SHOW_HIDE 105
#define IDC_ADDRESS_LIST 106
#define IDC_FILTER_TEXT 107
#define IDC_IMAGE_COMBO 108 

HWND g_hInputEdit = NULL;
HWND g_hLookupBtn = NULL;
HWND g_hOutputEdit = NULL;
HWND g_hBytesEdit = NULL;
HWND g_hListBox = NULL;
HWND g_hFilterEdit = NULL;
HWND g_hImageCombo = NULL; 

DWORD g_toggleKey = VK_INSERT;

// Structured cache tracking raw pointer arrays found inside the game runtime domain
struct CachedClassInfo
{
    std::wstring className;
    std::wstring fullName; // Format: "ClassName MethodName"
};

std::vector<const Il2CppImage *> g_DiscoveredImages;
std::vector<CachedClassInfo> g_ActiveImageClasses;

// UTF-8 conversion helpers 
std::wstring UTF8ToWide(const char *utf8Str)
{
    if (!utf8Str)
        return L"";
    int size = MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, NULL, 0);
    if (size <= 0)
        return L"";
    std::wstring wide(size, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, &wide[0], size);
    wide.resize(size - 1); 
    return wide;
}

std::string WideToUTF8(const wchar_t *wideStr)
{
    if (!wideStr)
        return "";
    int size = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, NULL, 0, NULL, NULL);
    if (size <= 0)
        return "";
    std::string utf8(size, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, &utf8[0], size, NULL, NULL);
    utf8.resize(size - 1);
    return utf8;
}

// Scans active domain memory to identify all assemblies loaded by the exe
void PopulateImageDropdown()
{
    if (!il2cpp_domain_get || !il2cpp_domain_get_assemblies || !il2cpp_assembly_get_image || !il2cpp_image_get_name)
    {
        GlumityPlugin_printf("Error: Missing internal IL2CPP API bindings required for image discovery.\n", MY_PLUGIN);
        return;
    }

    Il2CppDomain *domain = il2cpp_domain_get();
    if (!domain)
        return;

    size_t count = 0;
    const Il2CppAssembly **assemblies = il2cpp_domain_get_assemblies(domain, &count);
    if (!assemblies || count == 0)
        return;

    g_DiscoveredImages.clear();
    SendMessageW(g_hImageCombo, CB_RESETCONTENT, 0, 0);

    for (size_t i = 0; i < count; i++)
    {
        if (!assemblies[i])
            continue;
        const Il2CppImage *image = il2cpp_assembly_get_image(assemblies[i]);
        if (!image)
            continue;

        const char *imgName = il2cpp_image_get_name(image);
        if (imgName)
        {
            g_DiscoveredImages.push_back(image);
            std::wstring wideName = UTF8ToWide(imgName);
            SendMessageW(g_hImageCombo, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
        }
    }

    if (!g_DiscoveredImages.empty())
    {
        SendMessageW(g_hImageCombo, CB_SETCURSEL, 0, 0);
    }
}

void CacheClassesFromSelectedImage(int imageIndex)
{
    g_ActiveImageClasses.clear();
    if (imageIndex < 0 || imageIndex >= (int)g_DiscoveredImages.size())
        return;

    if (!il2cpp_image_get_class_count || !il2cpp_image_get_class || !il2cpp_class_get_name || !il2cpp_class_get_methods)
    {
        return;
    }

    const Il2CppImage *image = g_DiscoveredImages[imageIndex];
    size_t classCount = il2cpp_image_get_class_count((Il2CppImage *)image);

    for (size_t i = 0; i < classCount; i++)
    {
        const Il2CppClass *klass = il2cpp_image_get_class((Il2CppImage *)image, i);
        if (!klass)
            continue;

        const char *className = il2cpp_class_get_name((Il2CppClass *)klass);
        if (!className)
            continue;

        std::wstring wideClassName = UTF8ToWide(className);

        void *iter = NULL;
        while (const MethodInfo *method = il2cpp_class_get_methods((Il2CppClass *)klass, &iter))
        {
            if (!method || !method->name)
                continue;

            std::wstring wideMethodName = UTF8ToWide(method->name);
            CachedClassInfo info;
            info.className = wideClassName;
            info.fullName = wideClassName + L" " + wideMethodName;
            g_ActiveImageClasses.push_back(info);
        }
    }
}

void ApplyListFilter(const wchar_t *filterText)
{
    if (g_hListBox == NULL)
        return;

    SendMessageW(g_hListBox, LB_RESETCONTENT, 0, 0);
    size_t filterLen = wcslen(filterText);

    std::wstring lowerFilter = filterText;
    for (size_t i = 0; i < lowerFilter.length(); i++)
    {
        lowerFilter[i] = towlower(lowerFilter[i]);
    }

    for (const auto &item : g_ActiveImageClasses)
    {
        if (filterLen == 0)
        {
            SendMessageW(g_hListBox, LB_ADDSTRING, 0, (LPARAM)item.fullName.c_str());
        }
        else
        {
            std::wstring lowerItem = item.fullName;
            for (size_t j = 0; j < lowerItem.length(); j++)
            {
                lowerItem[j] = towlower(lowerItem[j]);
            }

            if (lowerItem.find(lowerFilter) != std::wstring::npos)
            {
                SendMessageW(g_hListBox, LB_ADDSTRING, 0, (LPARAM)item.fullName.c_str());
            }
        }
    }
}

void PerformAddressLookup(const wchar_t *query)
{
    wchar_t className[128] = {0};
    wchar_t methodName[128] = {0};
    size_t start = 0;
    size_t classLen = 0;
    size_t methodLen = 0;
    void *funcPtr = NULL;

    while (query[start] != L'\0' && iswspace(query[start]))
    {
        start++;
    }

    if (query[start] == L'\0')
    {
        SetWindowTextW(g_hOutputEdit, L"Error: Please enter a string to look up.");
        SetWindowTextW(g_hBytesEdit, L"");
        return;
    }

    while (query[start + classLen] != L'\0' && !iswspace(query[start + classLen]))
    {
        classLen++;
    }

    if (classLen == 0 || classLen >= (sizeof(className) / sizeof(wchar_t)))
    {
        SetWindowTextW(g_hOutputEdit, L"Error: Invalid class name format.");
        SetWindowTextW(g_hBytesEdit, L"");
        return;
    }

    wcsncpy_s(className, &query[start], classLen);
    start += classLen;

    while (query[start] != L'\0' && iswspace(query[start]))
    {
        start++;
    }

    while (query[start + methodLen] != L'\0' && !iswspace(query[start + methodLen]))
    {
        methodLen++;
    }

    if (methodLen == 0 || methodLen >= (sizeof(methodName) / sizeof(wchar_t)))
    {
        SetWindowTextW(g_hOutputEdit, L"Error: Format must be 'ClassName MethodName'.");
        SetWindowTextW(g_hBytesEdit, L"");
        return;
    }

    wcsncpy_s(methodName, &query[start], methodLen);

    if (dumperExports.GlumityV2Dumper_GetFunctionPointer == NULL)
    {
        SetWindowTextW(g_hOutputEdit, L"Error: Glumity Dumper export table not loaded/linked.");
        SetWindowTextW(g_hBytesEdit, L"");
        return;
    }

    // Convert parameters to broad standard UTF-8 string boundaries to trace hidden runtime table links
    std::string utf8Class = WideToUTF8(className);
    std::string utf8Method = WideToUTF8(methodName);

    funcPtr = dumperExports.GlumityV2Dumper_GetFunctionPointer_Global(utf8Class.c_str(), utf8Method.c_str());

    wchar_t bytesRes[512] = {0};
    if (funcPtr != NULL)
    {
        unsigned char *codeBytes = (unsigned char *)funcPtr;
        if (!IsBadReadPtr(funcPtr, 16))
        {
            swprintf_s(bytesRes, sizeof(bytesRes) / sizeof(wchar_t),
                       L"Raw Bytes: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
                       codeBytes[0], codeBytes[1], codeBytes[2], codeBytes[3],
                       codeBytes[4], codeBytes[5], codeBytes[6], codeBytes[7],
                       codeBytes[8], codeBytes[9], codeBytes[10], codeBytes[11],
                       codeBytes[12], codeBytes[13], codeBytes[14], codeBytes[15]);
        }
        else
        {
            swprintf_s(bytesRes, sizeof(bytesRes) / sizeof(wchar_t), L"Raw Bytes: [Memory Unreadable]");
        }
    }
    else
    {
        swprintf_s(bytesRes, sizeof(bytesRes) / sizeof(wchar_t), L"Raw Bytes: [Function Not Found]");
    }
    SetWindowTextW(g_hBytesEdit, bytesRes);

    size_t resBufferSize = 16384;
    wchar_t *res = (wchar_t *)malloc(resBufferSize * sizeof(wchar_t));
    if (res != NULL)
    {
        swprintf_s(res, resBufferSize,
                   L"Lookup Result:\r\n"
                   L"----------------------------------------\r\n"
                   L"Class: %s\r\n"
                   L"Method: %s\r\n"
                   L"Pointer: 0x%p\r\n\r\n"
                   L"JIT Template:\r\n"
                   L"----------------------------------------\r\n"
                   L"#include <GlumityLib/GlumityLib.h>\r\n\r\n"
                   L"#define MY_PLUGIN \"MY_PLUGIN\"\r\n\r\n"
                   L"GlumityV2DumperExports dumperExports;\r\n"
                   L"struct %s;\r\n\r\n"
                   L"GLUMITYV2_GAME_HOOK_TYPE(void*, %s_%s_t)(struct %s*);\r\n"
                   L"%s_%s_t %s_%s_o;\r\n\r\n"
                   L"void* %s_%s_hook(struct %s* _this)\r\n"
                   L"{\r\n"
                   L"    return %s_%s_o(_this);\r\n"
                   L"}\r\n\r\n"
                   L"// Installation:\r\n"
                   L"// Current pointer address found: 0x%p\r\n\r\n"
                   L"VOID OnHookInitFail()\r\n"
                   L"{\r\n"
                   L"    GLUMITY_PRINT_COLOR(CON_RED, \"Failed to initialize Minhook!\\n\", MY_PLUGIN); \r\n"
                   L"}\r\n\r\n"
                   L"VOID Setup()\r\n"
                   L"{\r\n"
                   L"    GLUMITYV2_VERIFY_DEPENDENCY(\"GlumityV2IL2CPPDumper\");\r\n"
                   L"    GLUMITYV2_DUMPER_WAITFOR_INIT(dumperExports);\r\n\r\n"
                   L"    %s_%s_o = (%s_%s_t)dumperExports.GlumityV2Dumper_GetFunctionPointer(\"%S\", \"%S\");\r\n\r\n"
                   L"    GLUMITYV2_INIT_HOOKING(MY_PLUGIN, OnHookInitFail)\r\n"
                   L"    {\r\n"
                   L"        GLUMITY_PRINT_COLOR(CON_GREEN, \"Initialized Minhook\\n\", MY_PLUGIN);\r\n"
                   L"    }\r\n\r\n"
                   L"    GLUMITYV2_GAME_HOOK_CREATE(\r\n"
                   L"        \"%s_%s\",\r\n"
                   L"        %s_%s_o,\r\n"
                   L"        %s_%s_hook);\r\n\r\n"
                   L"    GLUMITYV2_GAME_HOOK_ENABLE_ALL(MY_PLUGIN);\r\n"
                   L"}\r\n",
                   className, methodName, funcPtr,
                   className,
                   className, methodName, className,
                   className, methodName, className, methodName,
                   className, methodName, className,
                   className, methodName,
                   funcPtr,
                   className, methodName, className, methodName, utf8Class.c_str(), utf8Method.c_str(),
                   className, methodName,
                   className, methodName,
                   className, methodName);

        SetWindowTextW(g_hOutputEdit, res);
        free(res);
    }
}

VOID ToggleWindowVisibility(HWND hwnd)
{
    g_showWindow = (g_showWindow == 1) ? 0 : 1;
    ShowWindow(hwnd, g_showWindow ? SW_SHOW : SW_HIDE);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        g_hInputEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 20, 20, 240, 25, hwnd, (HMENU)IDC_INPUT_TEXT, GetModuleHandle(NULL), NULL);
        SetWindowTextW(g_hInputEdit, L"Enter search term...");

        g_hLookupBtn = CreateWindowW(L"BUTTON", L"Lookup Address", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 270, 18, 110, 28, hwnd, (HMENU)IDC_BUTTON_LOOKUP, GetModuleHandle(NULL), NULL);
        g_hOutputEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_VSCROLL, 20, 60, 360, 245, hwnd, (HMENU)IDC_OUTPUT_TEXT, GetModuleHandle(NULL), NULL);
        SetWindowTextW(g_hOutputEdit, L"Results will appear here...");

        g_hBytesEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 20, 315, 360, 50, hwnd, (HMENU)IDC_BYTES_TEXT, GetModuleHandle(NULL), NULL);
        SetWindowTextW(g_hBytesEdit, L"Hex Bytes will appear here...");

        g_hImageCombo = CreateWindowExW(WS_EX_CLIENTEDGE, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL, 395, 20, 230, 200, hwnd, (HMENU)IDC_IMAGE_COMBO, GetModuleHandle(NULL), NULL);

        g_hFilterEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 395, 55, 230, 25, hwnd, (HMENU)IDC_FILTER_TEXT, GetModuleHandle(NULL), NULL);
        g_hListBox = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", L"", WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL | WS_BORDER, 395, 90, 230, 275, hwnd, (HMENU)IDC_ADDRESS_LIST, GetModuleHandle(NULL), NULL);

        PopulateImageDropdown();
        CacheClassesFromSelectedImage(0);
        ApplyListFilter(L"");
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
        if (g_hListBox != NULL && g_hFilterEdit != NULL && g_hImageCombo != NULL)
        {
            int listLeft = 395;
            int listPaddingRight = 20;
            int newWidth = newClientWidth - listLeft - listPaddingRight;
            if (newWidth < 50)
                newWidth = 50;

            MoveWindow(g_hImageCombo, listLeft, 20, newWidth, 200, TRUE);
            MoveWindow(g_hFilterEdit, listLeft, 55, newWidth, 25, TRUE);
            MoveWindow(g_hListBox, listLeft, 90, newWidth, 275, TRUE);
        }
        break;
    }
    case WM_COMMAND:
    {
        if (LOWORD(wParam) == IDC_IMAGE_COMBO && HIWORD(wParam) == CBN_SELCHANGE)
        {
            int idx = (int)SendMessageW(g_hImageCombo, CB_GETCURSEL, 0, 0);
            if (idx != CB_ERR)
            {
                CacheClassesFromSelectedImage(idx);

                SetWindowTextW(g_hFilterEdit, L"");
                ApplyListFilter(L"");
            }
        }
        else if (LOWORD(wParam) == IDC_BUTTON_LOOKUP)
        {
            int len = GetWindowTextLengthW(g_hInputEdit);
            if (len > 0)
            {
                wchar_t *buff = (wchar_t *)malloc(((size_t)len + 1) * sizeof(wchar_t));
                if (!buff)
                    return 0;
                GetWindowTextW(g_hInputEdit, buff, len + 1);
                PerformAddressLookup(buff);
                free(buff);
            }
            else
            {
                PerformAddressLookup(L"");
            }
        }
        else if (LOWORD(wParam) == IDC_FILTER_TEXT && HIWORD(wParam) == EN_CHANGE)
        {
            int len = GetWindowTextLengthW(g_hFilterEdit);
            wchar_t *filterBuff = (wchar_t *)malloc(((size_t)len + 1) * sizeof(wchar_t));
            if (filterBuff != NULL)
            {
                GetWindowTextW(g_hFilterEdit, filterBuff, len + 1);
                ApplyListFilter(filterBuff);
                free(filterBuff);
            }
        }
        else if (LOWORD(wParam) == IDC_ADDRESS_LIST && HIWORD(wParam) == LBN_SELCHANGE)
        {
            int idx = (int)SendMessageW(g_hListBox, LB_GETCURSEL, 0, 0);
            if (idx != LB_ERR)
            {
                int textLen = (int)SendMessageW(g_hListBox, LB_GETTEXTLEN, idx, 0);
                if (textLen > 0)
                {
                    wchar_t *listBuffer = (wchar_t *)malloc(((size_t)textLen + 1) * sizeof(wchar_t));
                    if (listBuffer)
                    {
                        SendMessageW(g_hListBox, LB_GETTEXT, idx, (LPARAM)listBuffer);
                        SetWindowTextW(g_hInputEdit, listBuffer);
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
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
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

    WNDCLASSEXW wc = {0};
    HWND hwnd;
    MSG Msg;
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = g_szClassName;

    if (!RegisterClassExW(&wc))
        return 0;

    hwnd = CreateWindowExW(WS_EX_TOPMOST, g_szClassName, L"Glumity Address Lookup", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 660, 425, NULL, NULL, hInstance, NULL);
    if (hwnd == NULL)
        return 0;

    g_showWindow = 1;
    ShowWindow(hwnd, SW_HIDE);
    UpdateWindow(hwnd);
    GlumityPlugin_printf("Initialized Unicode GUI (press insert to toggle tracking)!\n", MY_PLUGIN);

    BOOL wasKeyPressed = FALSE;
    while (TRUE)
    {
        if (PeekMessageW(&Msg, NULL, 0, 0, PM_REMOVE))
        {
            if (Msg.message == WM_QUIT)
                break;
            TranslateMessage(&Msg);
            DispatchMessageW(&Msg);
        }
        else
        {
            Sleep(10);
        }

        BOOL isKeyDown = (GetAsyncKeyState(g_toggleKey) & 0x8000) != 0;
        if (isKeyDown && !wasKeyPressed)
        {
            ToggleWindowVisibility(hwnd);
        }
        wasKeyPressed = isKeyDown;
    }

    UnregisterClassW(g_szClassName, hInstance);
    return Msg.wParam;
}

GLUMITYV2_PLUGIN_ENTRY { GLUMITYV2_PLUGIN_THREADRUN(WindowThread, 0); }
GLUMITYV2_PLUGIN_EXIT { return 0; }
