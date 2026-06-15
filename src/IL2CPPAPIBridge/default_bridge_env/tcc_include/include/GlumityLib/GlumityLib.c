#include "GlumityLib.h"

#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <MinHook.h>

BOOL Glumity_GetErrorMessage(DWORD dwErrorCode, LPTSTR pBuffer, DWORD cchBufferLength)
{
    if (cchBufferLength == 0)
    {
        return FALSE;
    }

    DWORD cchMsg = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, /* (not used with FORMAT_MESSAGE_FROM_SYSTEM) */
                                 dwErrorCode,
                                 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                 pBuffer,
                                 cchBufferLength,
                                 NULL);
    return (cchMsg > 0);
}

GlumityV2_il2cppStr *IL2CPP_String_Create(const char *input)
{
    if (!input)
        return NULL;

    size_t len = strlen(input);
    size_t size = sizeof(GlumityV2_il2cppStr) + (len * sizeof(uint16_t));

    GlumityV2_il2cppStr *str = (GlumityV2_il2cppStr *)malloc(size);
    if (!str)
        return NULL;

    str->klass = NULL;
    str->monitor = NULL;

    str->fields.m_stringLength = (int32_t)len;

    uint16_t *buffer = &str->fields.m_firstChar;

    for (size_t i = 0; i < len; i++)
    {
        buffer[i] = (uint16_t)input[i];
    }

    return str;
}