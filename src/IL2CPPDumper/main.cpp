#include <GlumityLib.h>
#include <sstream>
#include <windows.h>
#include <Psapi.h>
#include <vector>
#include "Dumper.hpp"

#define PRINT_HEAD "GlumityV2IL2CPPDumper"

GlumityV2Dumper::Dumper dumper{};

EXPORT void *GlumityV2Dumper_GetFunctionPointerWithPattern(HMODULE module, const char *signature)
{
    GlumityPlugin_printf("Looking for function with pattern: [%s]\n", PRINT_HEAD, signature);

    static auto pattern_to_byte = [](const char *pattern)
    {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char *>(pattern);
        auto end = const_cast<char *>(pattern) + strlen(pattern);

        for (auto current = start; current < end; ++current)
        {
            if (*current == '?')
            {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            }
            else
            {
                bytes.push_back(strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

    auto dosHeader = (PIMAGE_DOS_HEADER)module;
    auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t *)module + dosHeader->e_lfanew);

    auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
    auto patternBytes = pattern_to_byte(signature);
    auto scanBytes = reinterpret_cast<std::uint8_t *>(module);

    auto s = patternBytes.size();
    auto d = patternBytes.data();

    for (auto i = 0ul; i < sizeOfImage - s; ++i)
    {
        bool found = true;
        for (auto j = 0ul; j < s; ++j)
        {
            if (scanBytes[i + j] != d[j] && d[j] != -1)
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            return &scanBytes[i];
        }
    }
    return nullptr;
}

EXPORT void GlumityV2Dumper_WaitForDumper()
{
    while (!dumper.GetInitState())
    {
        Sleep(60);
        continue;
    }
}

EXPORT void *GlumityV2Dumper_GetFunctionPointer(const char *className, const char *functionName)
{
    GlumityPlugin_printf("Looking for function [%s] in class [%s]\n", PRINT_HEAD, functionName, className);
    auto ret = IL2CPP::Class::Utils::GetMethodPointer(className, functionName);
    if (!ret)
        GlumityPlugin_printf("Not found!\n", PRINT_HEAD);
    else
        GlumityPlugin_printf("Found!\n", PRINT_HEAD);
    return ret;
}

EXPORT void *GlumityV2Dumper_GetFunctionPointer_Global(const char *searchName, const char *functionName)
{
    std::vector<Unity::il2cppClass *> outClasses;
    IL2CPP::Class::FetchClasses(&outClasses, "", "");
    void *ret = nullptr;

    for (auto currentClass : outClasses)
    {
        if (!currentClass || !currentClass->m_pName)
            continue;

        if (strstr(currentClass->m_pName, searchName) != nullptr)
        {
            GlumityPlugin_printf("Match Found: %s in Image: %x\n",
                                 PRINT_HEAD,
                                 currentClass->m_pName,
                                 currentClass->m_pImage);

            ret = IL2CPP::Class::Utils::GetMethodPointer(currentClass, functionName);
        }
    }

    if (!ret)
        GlumityPlugin_printf("Global search for [%s] failed.\n", PRINT_HEAD, searchName);
    return nullptr;
}

EXPORT void *GlumityV2Dumper_GetFunctionPointer_FromModule(
    const char *module,
    const char *className,
    const char *functionName)
{
    std::vector<Unity::il2cppClass *> outClasses;
    IL2CPP::Class::FetchClasses(&outClasses, module, "");
    void *ret = nullptr;

    for (auto currentClass : outClasses)
    {
        if (!currentClass || !currentClass->m_pName)
            continue;
        GlumityPlugin_printf("Found Class: %s (Module: %x)\n", PRINT_HEAD, currentClass->m_pName, currentClass->m_pImage);
        if (strcmp(currentClass->m_pName, className) == 0)
        {
            GlumityPlugin_printf("Class match found!\n", PRINT_HEAD);

            ret = IL2CPP::Class::Utils::GetMethodPointer(currentClass, functionName);
            if (ret)
                GlumityPlugin_printf("Method found!\n", PRINT_HEAD);
            else
                GlumityPlugin_printf("Method not found!\n", PRINT_HEAD);
        }
    }

    return ret;
}

void InitDumper()
{
    dumper.Init();
}

GLUMITYV2_PLUGIN_ENTRY
{
    GLUMITYV2_PLUGIN_THREADRUN(InitDumper, 0);
}

GLUMITYV2_PLUGIN_EXIT
{
    dumper.~Dumper();
}