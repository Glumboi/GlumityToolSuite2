#include <GlumityLib.h>
#include <sstream>
#include "Dumper.hpp"

#define PRINT_HEAD "GlumityV2IL2CPPDumper"

GlumityV2Dumper::Dumper dumper{};

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
    {
        GlumityPlugin_printf("Not found!\n", PRINT_HEAD, functionName, className);
        return ret;
    }
    GlumityPlugin_printf("Found!\n", PRINT_HEAD, functionName, className);
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