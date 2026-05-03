#ifndef GLUMITYLIB_H
#define GLUMITYLIB_H

#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <MinHook.h>

#define GLUMITYV2_DUMPER_MODULE "GlumityV2IL2CPPDumper"
#define GLUMITYV2_DUMPER_DLL "GlumityV2IL2CPPDumper.dll"

#define GLUMITYV2_MODULE "GlumityToolSuite2"
#define GLUMITYV2_DLL "GlumityToolSuite2.dll"

#ifdef __cplusplus
extern "C"
{
#endif

#include "../MainLoader/PluginLoader.h"
#include "../MainLoader/Plugin.h"

    /// @brief Printf for glumity plugin specific code, uses malloc and free for a temp char*, caller doesn't need to manage
    /// @param fmt
    /// @param
    void GlumityPlugin_printf(const char *fmt, const char *printHeaderInner, ...);

    /// @brief entry point of plugins, passes a handle of glumity's loaded main dll (the loader)
    typedef void (*GlumityPlugin_EntryPoint)();

#define GLUMITYV2_PLUGIN_ENTRY \
    EXPORT void GlumityMain()

    /// @brief exit point of plugins, get's called on application termination/loader termination
    typedef void (*GlumityPlugin_ExitPoint)();

#define GLUMITYV2_PLUGIN_EXIT \
    EXPORT void GlumityExit()

#define INIT_GLUMITYV2_EXPORT(mod, exportName, exportType) \
    (exportType) GetProcAddress(mod, exportName);

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

#define GLUMITYV2_INIT_HOOKING(pluginName, onFail)                           \
    if (MH_Initialize() != MH_OK)                                            \
    {                                                                        \
        GlumityPlugin_printf("Failed to initialize Minhook!\n", pluginName); \
        onFail;                                                              \
    }                                                                        \
    else

#define GLUMITYV2_GAME_HOOK_CREATE(name, original, hook)  \
    {                                                     \
        MH_STATUS stat = MH_CreateHook(                   \
            (LPVOID *)(original),                         \
            &hook,                                        \
            (LPVOID *)&original);                         \
        if (stat == MH_OK)                                \
            Glumity_printf("Created a hook: %s\n", name); \
    }

#define GLUMITYV2_GAME_HOOK_ENABLE_ALL(pluginName)                    \
    {                                                                 \
        MH_STATUS stat = MH_EnableHook(MH_ALL_HOOKS);                 \
        if (stat == MH_OK)                                            \
            GlumityPlugin_printf("Enabled all hooks!\n", pluginName); \
    }

#define GLUMITYV2_GAME_HOOK_ENABLE(name, original)        \
    {                                                     \
        MH_STATUS stat = MH_EnableHook(original);         \
        if (stat == MH_OK)                                \
            Glumity_printf("Enabled a hook: %s\n", name); \
    }

#define GLUMITYV2_GAME_HOOK_TYPE(retType, name) typedef retType(*name)

    // Recommended to use this to run most code,
    // specially function hooking since you have to rely on the dumper being loaded
#define GLUMITYV2_PLUGIN_THREADRUN(function, pParam)                                     \
    freopen("CONOUT$", "w", stdout); /* enable plugin printing */                        \
    HANDLE hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)function, pParam, 0, 0); \
    if (hThread)                                                                         \
        CloseHandle(hThread);

#define GLUMITYV2_DUMPER_WAITFOR_INIT(dumperExports)   \
    if (!dumperExports.GlumityV2Dumper_WaitForDumper)  \
    {                                                  \
        GlumityV2DumperExports_Init(&dumperExports);   \
    }                                                  \
    if (dumperExports.GlumityV2Dumper_WaitForDumper)   \
    {                                                  \
        dumperExports.GlumityV2Dumper_WaitForDumper(); \
    }

#define GLUMITYV2_DUMPER_GET_GAME_FUNCTION(className, functionName, dumperExports) \
    dumperExports.GlumityV2Dumper_GetFunctionPointer(className, functionName);

#define GLUMITYV2_PLUGIN_INIT_IL2CPP \
    if (IL2CPP::Initialize(true) && IL2CPP::Thread::Attach(IL2CPP::Domain::Get()))
#ifdef __cplusplus
}
#endif

// Some il2cpp type helpers
// e.g:
/*
struct DinoInfo__Fields {
    // 0x00 to 0x50 is exactly 0x50 bytes of padding
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

typedef void (*IL2CPP_Method_Pointer)();
typedef void *(*InvokerMethod)(IL2CPP_Method_Pointer, struct IL2CPP_MethodInfo const *, void *, void **);
typedef const void *Il2CppMetadataMethodDefinitionHandle;
typedef const void *const *Il2CppMetadataGenericContainerHandle;

union IL2CPP_RGCTXData
{
    PAD_TO(0x00, 0x08); // stubbing this for now
};

static_assert(sizeof(union IL2CPP_RGCTXData) == 0x08, "Struct size mismatch!");

struct IL2CPP_GenericMethod
{
    PAD_TO(0x00, 0x18);
};

static_assert(sizeof(struct IL2CPP_GenericMethod) == 0x18, "Struct size mismatch!");

struct IL2CPP_Type
{
    PAD_TO(0x00, 0x10);
};

static_assert(sizeof(struct IL2CPP_Type) == 0x10, "Struct size mismatch!");

struct IL2CPP_Class
{
    PAD_TO(0x00, 0x010);
    char const *name;
    char const *namespaze;
    PAD_TO(0x20, 0x0338);
};

static_assert(sizeof(struct IL2CPP_Class) == 0x0338, "Struct size mismatch!");

struct ParameterInfo
{
    PAD_TO(0x00, 0x18);
};

struct IL2CPP_MethodInfo
{
    IL2CPP_Method_Pointer methodPointer;
    InvokerMethod invoker_method;
    char const *name;
    struct IL2CPP_Class *klass;
    struct IL2CPP_Type const *return_type;
    struct ParameterInfo const *parameters;
    union
    {
        union IL2CPP_RGCTXData const *rgctx_data;
        Il2CppMetadataMethodDefinitionHandle methodMetadataHandle;
    } Il2CppVariant;
    union
    {
        struct IL2CPP_GenericMethod const *genericMethod;
        Il2CppMetadataGenericContainerHandle genericContainerHandle;
    };
    uint32_t token;
    uint16_t flags;
    uint16_t iflags;
    uint16_t slot;
    uint8_t parameters_count;
    PAD_TO(0x50, 0x58);
};

static_assert(sizeof(struct IL2CPP_MethodInfo) == 0x58, "Struct size mismatch!");

struct IL2CPP_VirtualInvokeData
{
    IL2CPP_Method_Pointer methodPtr;
    struct IL2CPP_MethodInfo const *method;
};

static_assert(sizeof(struct IL2CPP_VirtualInvokeData) == 0x10, "Struct size mismatch!");

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
char *IL2CPP_String_ToCString(GlumityV2_il2cppStr *str);

/// @brief Allocates memory, caller has to manage returned pointer, can be ignored as well and let the game "manage" it, might cause leaks eventually
/// @param input Any null terminated c string
/// @return A pointer to a newly allocated il2cpp string
GlumityV2_il2cppStr *IL2CPP_String_Create(const char *input);

#endif