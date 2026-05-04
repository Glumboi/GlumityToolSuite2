#ifndef GLUMITY_DUMPER_H
#define GLUMITY_DUMPER_H

#include <windows.h>

typedef void *(*GlumityV2Dumper_GetFunctionPointer_t)(const char *, const char *);
typedef void *(*GlumityV2Dumper_GetFunctionPointerWithPattern_t)(HMODULE, const char *);
typedef void *(*GlumityV2Dumper_GetFunctionPointer_FromModule_t)(const char *, const char *, const char *);
typedef void *(*GlumityV2Dumper_GetFunctionPointer_Global_t)(const char *, const char *);
typedef void (*GlumityV2Dumper_WaitForDumper_t)();

typedef struct
{
    GlumityV2Dumper_GetFunctionPointer_t GlumityV2Dumper_GetFunctionPointer;
    GlumityV2Dumper_GetFunctionPointerWithPattern_t GlumityV2Dumper_GetFunctionPointerWithPattern;
    GlumityV2Dumper_GetFunctionPointer_FromModule_t GlumityV2Dumper_GetFunctionPointer_FromModule;
    GlumityV2Dumper_GetFunctionPointer_Global_t GlumityV2Dumper_GetFunctionPointer_Global;
    GlumityV2Dumper_WaitForDumper_t GlumityV2Dumper_WaitForDumper;
} GlumityV2DumperExports;

// Only works if dumper plugin is loaded
void GlumityV2DumperExports_Init(GlumityV2DumperExports *dumperExports);

// Declare to use it here already
void IL2CPP_ResolveFunctions();

#define GLUMITYV2_DUMPER_WAITFOR_INIT(dumperExports)   \
    if (!dumperExports.GlumityV2Dumper_WaitForDumper)  \
    {                                                  \
        GlumityV2DumperExports_Init(&dumperExports);   \
    }                                                  \
    if (dumperExports.GlumityV2Dumper_WaitForDumper)   \
    {                                                  \
        dumperExports.GlumityV2Dumper_WaitForDumper(); \
        IL2CPP_ResolveFunctions();                     \
    }

#define GLUMITYV2_DUMPER_GET_GAME_FUNCTION(className, functionName, dumperExports) \
    dumperExports.GlumityV2Dumper_GetFunctionPointer(className, functionName);

#define GLUMITYV2_PLUGIN_INIT_IL2CPP \
    if (IL2CPP::Initialize(true) && IL2CPP::Thread::Attach(IL2CPP::Domain::Get()))

#endif