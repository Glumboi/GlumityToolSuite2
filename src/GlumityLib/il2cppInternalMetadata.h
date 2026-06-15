#pragma once

#include <stdint.h>

struct Il2CppType;
typedef struct Il2CppType Il2CppType;
struct Il2CppClass;
struct MethodInfo;
struct Il2CppType;
struct Il2CppGenericClass;
typedef struct Il2CppGenericClass Il2CppGenericClass;

typedef enum
{
    IL2CPP_TYPE_END = 0x00,
    IL2CPP_TYPE_VOID = 0x01,
    IL2CPP_TYPE_BOOLEAN = 0x02,
    IL2CPP_TYPE_CHAR = 0x03,
    IL2CPP_TYPE_I1 = 0x04,
    IL2CPP_TYPE_U1 = 0x05,
    IL2CPP_TYPE_I2 = 0x06,
    IL2CPP_TYPE_U2 = 0x07,
    IL2CPP_TYPE_I4 = 0x08,
    IL2CPP_TYPE_U4 = 0x09,
    IL2CPP_TYPE_I8 = 0x0a,
    IL2CPP_TYPE_U8 = 0x0b,
    IL2CPP_TYPE_R4 = 0x0c,
    IL2CPP_TYPE_R8 = 0x0d,
    IL2CPP_TYPE_STRING = 0x0e,
    IL2CPP_TYPE_PTR = 0x0f,
    IL2CPP_TYPE_BYREF = 0x10,
    IL2CPP_TYPE_VALUETYPE = 0x11,
    IL2CPP_TYPE_CLASS = 0x12,
    IL2CPP_TYPE_VAR = 0x13,
    IL2CPP_TYPE_ARRAY = 0x14,
    IL2CPP_TYPE_GENERICINST = 0x15,
    IL2CPP_TYPE_TYPEDBYREF = 0x16,
    IL2CPP_TYPE_I = 0x18,
    IL2CPP_TYPE_U = 0x19,
    IL2CPP_TYPE_FNPTR = 0x1b,
    IL2CPP_TYPE_OBJECT = 0x1c,
    IL2CPP_TYPE_SZARRAY = 0x1d,
    IL2CPP_TYPE_MVAR = 0x1e,
    IL2CPP_TYPE_CMOD_REQD = 0x1f,
    IL2CPP_TYPE_CMOD_OPT = 0x20,
    IL2CPP_TYPE_INTERNAL = 0x21,

    IL2CPP_TYPE_MODIFIER = 0x40,
    IL2CPP_TYPE_SENTINEL = 0x41,
    IL2CPP_TYPE_PINNED = 0x45,

    IL2CPP_TYPE_ENUM = 0x55,
    IL2CPP_TYPE_IL2CPP_TYPE_INDEX = 0xff
} Il2CppTypeEnum;

typedef int32_t TypeIndex;
typedef int32_t TypeDefinitionIndex;
typedef int32_t FieldIndex;
typedef int32_t DefaultValueIndex;
typedef int32_t DefaultValueDataIndex;
typedef int32_t CustomAttributeIndex;
typedef int32_t ParameterIndex;
typedef int32_t MethodIndex;
typedef int32_t GenericMethodIndex;
typedef int32_t PropertyIndex;
typedef int32_t EventIndex;
typedef int32_t GenericContainerIndex;
typedef int32_t GenericParameterIndex;
typedef int16_t GenericParameterConstraintIndex;
typedef int32_t NestedTypeIndex;
typedef int32_t InterfacesIndex;
typedef int32_t VTableIndex;
typedef int32_t RGCTXIndex;
typedef int32_t StringIndex;
typedef int32_t StringLiteralIndex;
typedef int32_t GenericInstIndex;
typedef int32_t ImageIndex;
typedef int32_t AssemblyIndex;
typedef int32_t InteropDataIndex;

typedef int32_t TypeFieldIndex;
typedef int32_t TypeMethodIndex;
typedef int32_t MethodParameterIndex;
typedef int32_t TypePropertyIndex;
typedef int32_t TypeEventIndex;
typedef int32_t TypeInterfaceIndex;
typedef int32_t TypeNestedTypeIndex;
typedef int32_t TypeInterfaceOffsetIndex;
typedef int32_t GenericContainerParameterIndex;
typedef int32_t AssemblyTypeIndex;
typedef int32_t AssemblyExportedTypeIndex;

static const TypeIndex kTypeIndexInvalid = -1;
static const TypeDefinitionIndex kTypeDefinitionIndexInvalid = -1;
static const DefaultValueDataIndex kDefaultValueIndexNull = -1;
static const CustomAttributeIndex kCustomAttributeIndexInvalid = -1;
static const EventIndex kEventIndexInvalid = -1;
static const FieldIndex kFieldIndexInvalid = -1;
static const MethodIndex kMethodIndexInvalid = -1;
static const PropertyIndex kPropertyIndexInvalid = -1;
static const GenericContainerIndex kGenericContainerIndexInvalid = -1;
static const GenericParameterIndex kGenericParameterIndexInvalid = -1;
static const RGCTXIndex kRGCTXIndexInvalid = -1;
static const StringLiteralIndex kStringLiteralIndexInvalid = -1;
static const InteropDataIndex kInteropDataIndexInvalid = -1;

#define PUBLIC_KEY_BYTE_LENGTH 8
static const int kPublicKeyByteLength = PUBLIC_KEY_BYTE_LENGTH;

typedef struct Il2CppMethodSpec
{
    MethodIndex methodDefinitionIndex;
    GenericInstIndex classIndexIndex;
    GenericInstIndex methodIndexIndex;
} Il2CppMethodSpec;

typedef enum Il2CppRGCTXDataType
{
    IL2CPP_RGCTX_DATA_INVALID,
    IL2CPP_RGCTX_DATA_TYPE,
    IL2CPP_RGCTX_DATA_CLASS,
    IL2CPP_RGCTX_DATA_METHOD,
    IL2CPP_RGCTX_DATA_ARRAY,
    IL2CPP_RGCTX_DATA_CONSTRAINED,
} Il2CppRGCTXDataType;

typedef union Il2CppRGCTXDefinitionData
{
    int32_t rgctxDataDummy;
    MethodIndex __methodIndex;
    TypeIndex __typeIndex;
} Il2CppRGCTXDefinitionData;

typedef struct Il2CppRGCTXConstrainedData
{
    TypeIndex __typeIndex;
    uint32_t __encodedMethodIndex;
} Il2CppRGCTXConstrainedData;

typedef struct Il2CppRGCTXDefinition
{
    Il2CppRGCTXDataType type;
    const void *data;
} Il2CppRGCTXDefinition;

typedef struct
{
    MethodIndex methodIndex;
    MethodIndex invokerIndex;
    MethodIndex adjustorThunkIndex;
} Il2CppGenericMethodIndices;

typedef struct Il2CppGenericMethodFunctionsDefinitions
{
    GenericMethodIndex genericMethodIndex;
    Il2CppGenericMethodIndices indices;
} Il2CppGenericMethodFunctionsDefinitions;

static inline uint32_t GetTokenType(uint32_t token)
{
    return token & 0xFF000000;
}

static inline uint32_t GetTokenRowId(uint32_t token)
{
    return token & 0x00FFFFFF;
}

typedef const struct ___Il2CppMetadataImageHandle *Il2CppMetadataImageHandle;
typedef const struct ___Il2CppMetadataCustomAttributeHandle *Il2CppMetadataCustomAttributeHandle;
typedef const struct ___Il2CppMetadataTypeHandle *Il2CppMetadataTypeHandle;
typedef const struct ___Il2CppMetadataMethodHandle *Il2CppMetadataMethodDefinitionHandle;
typedef const struct ___Il2CppMetadataGenericContainerHandle *Il2CppMetadataGenericContainerHandle;
typedef const struct ___Il2CppMetadataGenericParameterHandle *Il2CppMetadataGenericParameterHandle;

struct Il2CppClass;
struct MethodInfo;

typedef struct
{
    const Il2CppType *etype;
    uint8_t rank;
    uint8_t numsizes;
    uint8_t numlobounds;
    int *sizes;
    int *lobounds;
} Il2CppArrayType;

typedef struct Il2CppType
{
    union
    {
        void *dummy;
        TypeDefinitionIndex __klassIndex;
        Il2CppMetadataTypeHandle typeHandle;
        const struct Il2CppType *type;
        Il2CppArrayType *array;
        GenericParameterIndex __genericParameterIndex;
        Il2CppMetadataGenericParameterHandle genericParameterHandle;
        Il2CppGenericClass *generic_class;
    } data;

    uint16_t attrs;
    uint8_t type;
    uint8_t num_mods;
    uint8_t byref;
    uint8_t pinned;
    uint8_t valuetype;

} Il2CppType;

typedef struct
{
    uint32_t type_argc;
    const struct Il2CppType **type_argv;
} Il2CppGenericInst;

typedef struct
{
    const Il2CppGenericInst *class_inst;
    const Il2CppGenericInst *method_inst;
} Il2CppGenericContext;

typedef struct Il2CppGenericClass
{
    const struct Il2CppType *type;
    Il2CppGenericContext context;
    struct Il2CppClass *cached_class;
} Il2CppGenericClass;

typedef struct
{
    const struct MethodInfo *methodDefinition;
    Il2CppGenericContext context;
} Il2CppGenericMethod;

typedef struct
{
    const Il2CppType *type;
    const char *name;
    uint32_t token;
} Il2CppMetadataFieldInfo;

typedef struct
{
    Il2CppMetadataMethodDefinitionHandle handle;
    const char *name;
    const Il2CppType *return_type;
    uint32_t token;
    uint16_t flags;
    uint16_t iflags;
    uint16_t slot;
    uint16_t parameterCount;
} Il2CppMetadataMethodInfo;

typedef struct
{
    const char *name;
    uint32_t token;
    const Il2CppType *type;
} Il2CppMetadataParameterInfo;

typedef struct
{
    const char *name;
    const struct MethodInfo *get;
    const struct MethodInfo *set;
    uint32_t attrs;
    uint32_t token;
} Il2CppMetadataPropertyInfo;

typedef struct
{
    const char *name;
    const Il2CppType *type;
    const struct MethodInfo *add;
    const struct MethodInfo *remove;
    const struct MethodInfo *raise;
    uint32_t token;
} Il2CppMetadataEventInfo;

typedef struct
{
    const Il2CppType *interfaceType;
    int32_t offset;
} Il2CppInterfaceOffsetInfo;

typedef struct
{
    Il2CppMetadataGenericContainerHandle containerHandle;
    const char *name;
    uint16_t num;
    uint16_t flags;
} Il2CppGenericParameterInfo;
