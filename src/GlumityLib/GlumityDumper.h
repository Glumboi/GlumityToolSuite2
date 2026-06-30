#ifndef GLUMITY_DUMPER_H
#define GLUMITY_DUMPER_H

#include <windows.h>

// Forward declaring the abstract IL2CPP handle so compilation succeeds without headers
typedef struct Il2CppClass Il2CppClass;

// MATCHED EXACTLY WITH THE EXPORT ENTRIES
typedef void *(*GlumityV2Dumper_GetFunctionPointerWithPattern_t)(HMODULE, const char *);
typedef void *(*GlumityV2Dumper_GetFunctionPointer_FromModule_t)(const char *, const char *, const char *);
typedef void *(*GlumityV2Dumper_GetFunctionPointer_Global_t)(const char *, const char *, const char *);
typedef char **(*GlumityV2Dumper_GetEverything_t)();
typedef void (*GlumityV2Dumper_WaitForDumper_t)();
typedef void (*GlumityV2Dumper_GetFieldOffset_t)(const char *, const char *, const char *);
typedef void (*GlumityV2Dumper_GetFieldPtr_t)(const char *, const char *, const char *, void *);

typedef struct
{
    GlumityV2Dumper_GetFunctionPointer_Global_t GlumityV2Dumper_GetFunctionPointer;
    GlumityV2Dumper_GetFunctionPointerWithPattern_t GlumityV2Dumper_GetFunctionPointerWithPattern;
    GlumityV2Dumper_GetFunctionPointer_FromModule_t GlumityV2Dumper_GetFunctionPointer_FromModule;
    GlumityV2Dumper_GetFunctionPointer_Global_t GlumityV2Dumper_GetFunctionPointer_Global;
    GlumityV2Dumper_GetEverything_t GlumityV2Dumper_GetEverything;
    GlumityV2Dumper_WaitForDumper_t GlumityV2Dumper_WaitForDumper;
    GlumityV2Dumper_GetFieldOffset_t GlumityV2Dumper_GetFieldOffset;
    GlumityV2Dumper_GetFieldPtr_t GlumityV2Dumper_GetFieldPtr;
} GlumityV2DumperExports;

void GlumityV2DumperExports_Init(GlumityV2DumperExports *dumperExports);

#define GLUMITYV2_DUMPER_WAITFOR_INIT(dumperExports)   \
    if (!dumperExports.GlumityV2Dumper_WaitForDumper)  \
    {                                                  \
        GlumityV2DumperExports_Init(&dumperExports);   \
    }                                                  \
    if (dumperExports.GlumityV2Dumper_WaitForDumper)   \
    {                                                  \
        dumperExports.GlumityV2Dumper_WaitForDumper(); \
    }                                                  \
    LoadIL2CPP(GetModuleHandleA(GAME_ASSEMBLY));

#define GLUMITYV2_DUMPER_GET_GAME_FUNCTION(className, functionName, dumperExports) \
    dumperExports.GlumityV2Dumper_GetFunctionPointer_Global(className, functionName);

#define GLUMITYV2_PLUGIN_INIT_IL2CPP \
    il2cpp_thread_attach(il2cpp_domain_get())

#endif