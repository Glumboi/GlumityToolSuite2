#ifndef GLUMITYLIB_H
#define GLUMITYLIB_H

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <MinHook.h>

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

    typedef void *(*il2cpp_domain_get_t)();
    typedef void *(*il2cpp_thread_attach_t)(void *domain);
    typedef void *(*il2cpp_runtime_invoke_t)(void *method, void *obj, void **params, void **exc);
    typedef void *(*il2cpp_class_get_method_from_name_t)(void *klass, const char *name, int argsCount);

    // Global il2cpp function pointers
    il2cpp_domain_get_t _il2cpp_domain_get;
    il2cpp_thread_attach_t _il2cpp_thread_attach;
    il2cpp_runtime_invoke_t _il2cpp_runtime_invoke;
    il2cpp_class_get_method_from_name_t _il2cpp_class_get_method_from_name;

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

typedef void (*IL2CPP_Method_Pointer)();
typedef void *(*InvokerMethod)(IL2CPP_Method_Pointer, struct IL2CPP_MethodInfo const *, void *, void **);
typedef const void *Il2CppMetadataMethodDefinitionHandle;
typedef const void *const *Il2CppMetadataGenericContainerHandle;
typedef struct ___Il2CppMetadataImageHandle const *Il2CppMetadataImageHandle;
typedef struct ___Il2CppMetadataTypeHandle const *Il2CppMetadataTypeHandle;
typedef struct ___Il2CppMetadataGenericParameterHandle const *Il2CppMetadataGenericParameterHandle;

struct IL2CPP_AssemblyName
{
    char const *name;
    char const *culture;
    uint8_t const *public_key;
    uint32_t hash_alg;
    int32_t hash_len;
    uint32_t flags;
    int32_t major;
    int32_t minor;
    int32_t build;
    int32_t revision;
    uint8_t public_key_token[0x8];
};

struct IL2CPP_Assembly
{
    struct Il2CppImage *image;
    uint32_t token;
    int32_t referencedAssemblyStart;
    int32_t referencedAssemblyCount;
    struct IL2CPP_AssemblyName aname;
};

struct IL2CPP_Image
{
    char const *name;
    char const *nameNoExt;
    struct IL2CPP_Assembly *assembly;
    uint32_t typeCount;
    uint32_t exportedTypeCount;
    uint32_t customAttributeCount;
    Il2CppMetadataImageHandle metadataHandle;
    struct Il2CppNameToTypeHandleHashTable *nameToClassHashTable;
    struct Il2CppCodeGenModule const *codeGenModule;
    uint32_t token;
    uint8_t dynamic;
};

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
    union
    {
        void *dummy;
        int32_t __klassIndex;
        Il2CppMetadataTypeHandle typeHandle;
        struct Il2CppType const *type;
        struct Il2CppArrayType *array;
        int32_t __genericParameterIndex;
        Il2CppMetadataGenericParameterHandle genericParameterHandle;
        struct Il2CppGenericClass *generic_class;
    } data;
    union
    {
        uint32_t attrs;
        enum Il2CppTypeEnum type;
        uint32_t num_mods;
        uint32_t byref;
        uint32_t pinned;
    } __bitfield8;
};

static_assert(sizeof(struct IL2CPP_Type) == 0x10, "Struct size mismatch!");

struct IL2CPP_Class
{
    struct IL2CPP_Image const *image;
    PAD_TO(0x08, 0x010);
    char const *name;
    char const *namespaze;
    struct IL2CPP_Type byval_arg;
    struct IL2CPP_Type this_arg;
    struct IL2CPP_Class *element_class;
    struct IL2CPP_Class *castClass;
    struct IL2CPP_Class *declaringType;
    struct IL2CPP_Class *parent;
    PAD_TO(0x60, 0x0338);
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

struct GameObject__VTable
{
    struct IL2CPP_VirtualInvokeData Equals;
    struct IL2CPP_VirtualInvokeData Finalize;
    struct IL2CPP_VirtualInvokeData GetHashCode;
    struct IL2CPP_VirtualInvokeData ToString;
};

struct IL2CPP_Class_0
{
    struct Il2CppImage const *image;
    void *gc_desc;
    char const *name;
    char const *namespaze;
    struct IL2CPP_Type byval_arg;
    struct IL2CPP_Type this_arg;
    struct IL2CPP_Class *element_class;
    struct IL2CPP_Class *castClass;
    struct IL2CPP_Class *declaringType;
    struct IL2CPP_Class *parent;
    struct Il2CppGenericClass *generic_class;
    Il2CppMetadataTypeHandle typeMetadataHandle;
    struct Il2CppInteropData const *interopData;
    struct IL2CPP_Class *klass;
    struct FieldInfo *fields;
    struct EventInfo const *events;
    struct PropertyInfo const *properties;
    struct MethodInfo const **methods;
    struct IL2CPP_Class **nestedTypes;
    struct IL2CPP_Class **implementedInterfaces;
};

struct GameObject__Class
{
    struct IL2CPP_Class_0 _0;
    struct Il2CppRuntimeInterfaceOffsetPair *interfaceOffsets;
    struct GameObject__StaticFields *static_fields;
    union Il2CppRGCTXData const *rgctx_data;
    PAD_TO(0xC8, 0x12A);
    uint16_t unknown_12a;
    uint8_t unknown_12c;
    PAD_TO(0x12D, 0x138);

    struct GameObject__VTable vtable;
};

static_assert(sizeof(struct GameObject__Class) == 0x0178, "Struct size mismatch!");

struct Object_1__Fields
{
    void *m_CachedPtr;
};

struct GameObject__Fields
{
    struct Object_1__Fields _;
};

struct GameObject
{
    struct GameObject__Class *klass;
    struct MonitorData *monitor;
    struct GameObject__Fields fields;
};

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

struct IL2CPP_Object;
struct IL2CPP_Exception;

struct IL2CPP_Object *IL2CPP_InvokeMethod(struct IL2CPP_Class *klass, const char *method_name, void *instance, void **params);
void IL2CPP_ResolveFunctions();

#endif