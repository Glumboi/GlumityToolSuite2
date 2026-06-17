#include "il2cppInternal.h"

#define DO_API(ret, name, args) \
    name##_t name = NULL;

#include "il2cppInternalApiFunctions.h"
#undef DO_API

// Raw byte pattern matching helper
static unsigned char* ScanMemoryPattern(unsigned char* baseAddress, DWORD size, const unsigned char* pattern, const char* mask)
{
    DWORD patternLength = (DWORD)strlen(mask);
    for (DWORD i = 0; i < size - patternLength; i++)
    {
        BOOL found = TRUE;
        for (DWORD j = 0; j < patternLength; j++)
        {
            if (mask[j] == 'x' && baseAddress[i + j] != pattern[j])
            {
                found = FALSE;
                break;
            }
        }
        if (found)
        {
            return &baseAddress[i];
        }
    }
    return NULL;
}

// Scans GameAssembly text segment if API symbols are omitted/stripped
static void* FindStrippedSymbol(HMODULE module, const char* name)
{
    if (!module) return NULL;

    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)module;
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((BYTE*)module + dosHeader->e_lfanew);
    unsigned char* baseAddress = (unsigned char*)module;
    DWORD sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;

    // Fallback pattern resolution arrays for core engine symbols
    if (strcmp(name, "il2cpp_domain_get") == 0)
    {
        unsigned char pattern1[] = { 0x48, 0x8B, 0x05, 0x00, 0x00, 0x00, 0x00, 0xC3 };
        unsigned char* match = ScanMemoryPattern(baseAddress, sizeOfImage, pattern1, "xxx????x");
        if (match) return match;

        unsigned char pattern2[] = { 0x48, 0x83, 0xEC, 0x28, 0x48, 0x8B, 0x05, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x28, 0xC3 };
        match = ScanMemoryPattern(baseAddress, sizeOfImage, pattern2, "xxxxxxx????xxxxx");
        if (match) return match;
    }
    
    // Universal signature fallback hook: If il2cpp_domain_get is found, trace relative addresses
    if (il2cpp_domain_get != NULL)
    {
        if (strcmp(name, "il2cpp_domain_get_assemblies") == 0)
        {
            unsigned char subPattern[] = { 0x48, 0x8B, 0x01, 0x48, 0x85, 0xC0 };
            unsigned char* match = ScanMemoryPattern((unsigned char*)il2cpp_domain_get, 0x10000, subPattern, "xxxxxx");
            if (match) return match;
        }
    }

    return NULL;
}

BOOL LoadIL2CPP(HMODULE module)
{
    if (module == NULL)
    {
        return FALSE;
    }

    BOOL executionSuccess = TRUE;

#define DO_API(ret, name, args)                                           \
    name = (name##_t)GetProcAddress(module, #name);                       \
    if (name == NULL)                                                     \
    {                                                                     \
        name = (name##_t)FindStrippedSymbol(module, #name);               \
        if (name == NULL)                                                 \
        {                                                                 \
            executionSuccess = FALSE;                                     \
        }                                                                 \
    }

#include "il2cppInternalApiFunctions.h"
#undef DO_API

    return executionSuccess;
}