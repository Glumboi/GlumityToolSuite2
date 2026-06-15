#ifndef GLUMITYLIB_H
#define GLUMITYLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <MinHook.h>
#include <windef.h>

#include "il2cppInternal.h"

#define GLUMITYLIB_PRINT_HEADER "GlumityV2Lib"

#define GLUMITYV2_DUMPER_MODULE "GlumityV2IL2CPPDumper"
#define GLUMITYV2_DUMPER_DLL "GlumityV2IL2CPPDumper.dll"

#define GLUMITYV2_MODULE "GlumityToolSuite2"
#define GLUMITYV2_DLL "GlumityToolSuite2.dll"

#ifdef __cplusplus
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif
#ifdef __cplusplus
extern "C"
{
#endif

#include "GlumityPluginLoader.h"
#include "GlumityPlugin.h"
#include "GlumityFilesystem.h"
#include "GlumityDumper.h"
#include "GlumityHooking.h"

    BOOL Glumity_GetErrorMessage(DWORD dwErrorCode, LPTSTR pBuffer, DWORD cchBufferLength);

#define INIT_GLUMITYV2_EXPORT(mod, exportName, exportType) \
    (exportType) GetProcAddress(mod, exportName);

#ifdef __cplusplus
}
#endif

// Some il2cpp type helpers
// e.g:
/*
struct DinoInfo__Fields {
    // 0x50 bytes of padding
    PAD_TO(0x00, 0x50);

    bool isSelected;    // Offset 0x50
    bool owned;         // Offset 0x51

    // Preloader is at 0x58. Current offset is 0x52.
    PAD_TO(0x52, 0x58);
    void* Preloader;    // Offset 0x58
};
*/

// Combines two tokens together (needed for unique padding names)
#define COMBINE_INNER(a, b) a##b
#define COMBINE(a, b) COMBINE_INNER(a, b)

// Creates a padding array of a specific size
#define PAD(size) uint8_t COMBINE(pad_, __LINE__)[size]

// Creates padding from current position to a target offset
#define PAD_TO(current, target) uint8_t COMBINE(pad_, __LINE__)[target - current]

typedef struct IL2CPP_String__Fields
{
    int32_t m_stringLength;
    uint16_t m_firstChar;
} GlumityV2_il2cppStr_fields;

typedef struct IL2CPP_String
{
    void *klass;
    void *monitor;
    struct IL2CPP_String__Fields fields;
} GlumityV2_il2cppStr;

/// @brief Allocates memory, caller has to manage returned pointer
/// @param str a pointer to an initialized il2cpp string
/// @return pointer to a newly allocated ascii, null terminated c string, il2cpp strings are normally utf encoded, so it could be lossy
inline char *IL2CPP_String_ToCString(GlumityV2_il2cppStr *str)
{
    int len = str->fields.m_stringLength;
    uint16_t *chars = &str->fields.m_firstChar;

    // TODO: Check for memory leaks, I believe the game manages it automatically though
    char *result = (char *)malloc(len + 1);
    if (!result)
        return NULL;

    for (int i = 0; i < len; i++)
    {
        result[i] = (char)chars[i];
    }

    result[len] = '\0';
    return result;
}

/// @brief Allocates memory, caller has to manage returned pointer, can be ignored as well and let the game "manage" it, might cause leaks eventually
/// @param input Any null terminated c string
/// @return A pointer to a newly allocated il2cpp string
GlumityV2_il2cppStr *IL2CPP_String_Create(const char *input);

#endif