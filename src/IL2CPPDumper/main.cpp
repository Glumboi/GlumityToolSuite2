#include <GlumityLib.h>
#include "Dumper.hpp"

#define PRINT_HEAD "GlumityV2IL2CPPDumper"
#define MAIN_IMAGE "Assembly-CSharp"

extern "C"
{
    EXPORT void *GlumityV2Dumper_GetFunctionPointer(const char *className, const char *functionName)
    {
        GlumityPlugin_printf("Looking for function [%s] in class [%s]...\n", PRINT_HEAD, functionName, className);
        auto ret = IL2CPP::Class::Utils::GetMethodPointer(className, functionName);
        if (!ret)
            GlumityPlugin_printf("Not found!\n", PRINT_HEAD, functionName, className);
        return ret;
    }

    EXPORT void GlumityMain(HMODULE glumityInternal)
    {
        GlumityV2Dumper::Dumper dumper{};
        dumper.Init(glumityInternal);
    }
}