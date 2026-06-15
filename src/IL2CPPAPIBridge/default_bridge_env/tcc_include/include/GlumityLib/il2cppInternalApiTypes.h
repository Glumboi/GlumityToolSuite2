#pragma once

#include <stdint.h>
#include <stddef.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif


typedef struct Il2CppClass Il2CppClass;
typedef struct EventInfo EventInfo;
typedef struct MethodInfo MethodInfo;
typedef struct FieldInfo FieldInfo;
typedef struct PropertyInfo PropertyInfo;

typedef struct Il2CppAssembly Il2CppAssembly;
typedef struct Il2CppArray Il2CppArray;
typedef struct Il2CppDelegate Il2CppDelegate;
typedef struct Il2CppDomain Il2CppDomain;
typedef struct Il2CppImage Il2CppImage;
typedef struct Il2CppException Il2CppException;
typedef struct Il2CppProfiler Il2CppProfiler;
typedef struct Il2CppObject Il2CppObject;
typedef struct Il2CppReflectionMethod Il2CppReflectionMethod;
typedef struct Il2CppReflectionType Il2CppReflectionType;
typedef struct Il2CppString Il2CppString;
typedef struct Il2CppThread Il2CppThread;
typedef struct Il2CppAsyncResult Il2CppAsyncResult;
typedef struct Il2CppManagedMemorySnapshot Il2CppManagedMemorySnapshot;
typedef struct Il2CppCustomAttrInfo Il2CppCustomAttrInfo;
typedef struct Il2CppStackFrameInfo Il2CppStackFrameInfo;

typedef enum Il2CppProfileFlags
{
	IL2CPP_PROFILE_NONE = 0,
	IL2CPP_PROFILE_APPDOMAIN_EVENTS = 1 << 0,
	IL2CPP_PROFILE_ASSEMBLY_EVENTS = 1 << 1,
	IL2CPP_PROFILE_MODULE_EVENTS = 1 << 2,
	IL2CPP_PROFILE_CLASS_EVENTS = 1 << 3,
	IL2CPP_PROFILE_JIT_COMPILATION = 1 << 4,
	IL2CPP_PROFILE_INLINING = 1 << 5,
	IL2CPP_PROFILE_EXCEPTIONS = 1 << 6,
	IL2CPP_PROFILE_ALLOCATIONS = 1 << 7,
	IL2CPP_PROFILE_GC = 1 << 8,
	IL2CPP_PROFILE_THREADS = 1 << 9,
	IL2CPP_PROFILE_REMOTING = 1 << 10,
	IL2CPP_PROFILE_TRANSITIONS = 1 << 11,
	IL2CPP_PROFILE_ENTER_LEAVE = 1 << 12,
	IL2CPP_PROFILE_COVERAGE = 1 << 13,
	IL2CPP_PROFILE_INS_COVERAGE = 1 << 14,
	IL2CPP_PROFILE_STATISTICAL = 1 << 15,
	IL2CPP_PROFILE_METHOD_EVENTS = 1 << 16,
	IL2CPP_PROFILE_MONITOR_EVENTS = 1 << 17,
	IL2CPP_PROFILE_IOMAP_EVENTS = 1 << 18,
	IL2CPP_PROFILE_GC_MOVES = 1 << 19,
	IL2CPP_PROFILE_FILEIO = 1 << 20
} Il2CppProfileFlags;

typedef enum
{
	IL2CPP_PROFILE_FILEIO_WRITE = 0,
	IL2CPP_PROFILE_FILEIO_READ
} Il2CppProfileFileIOKind;

typedef enum
{
	IL2CPP_GC_EVENT_START,
	IL2CPP_GC_EVENT_MARK_START,
	IL2CPP_GC_EVENT_MARK_END,
	IL2CPP_GC_EVENT_RECLAIM_START,
	IL2CPP_GC_EVENT_RECLAIM_END,
	IL2CPP_GC_EVENT_END,
	IL2CPP_GC_EVENT_PRE_STOP_WORLD,
	IL2CPP_GC_EVENT_POST_STOP_WORLD,
	IL2CPP_GC_EVENT_PRE_START_WORLD,
	IL2CPP_GC_EVENT_POST_START_WORLD
} Il2CppGCEvent;

typedef enum
{
	IL2CPP_GC_MODE_DISABLED = 0,
	IL2CPP_GC_MODE_ENABLED = 1,
	IL2CPP_GC_MODE_MANUAL = 2
} Il2CppGCMode;

typedef enum Il2CppStat
{
	IL2CPP_STAT_NEW_OBJECT_COUNT,
	IL2CPP_STAT_INITIALIZED_CLASS_COUNT,
	IL2CPP_STAT_METHOD_COUNT,
	IL2CPP_STAT_CLASS_STATIC_DATA_SIZE,
	IL2CPP_STAT_GENERIC_INSTANCE_COUNT,
	IL2CPP_STAT_GENERIC_CLASS_COUNT,
	IL2CPP_STAT_INFLATED_METHOD_COUNT,
	IL2CPP_STAT_INFLATED_TYPE_COUNT,
} Il2CppStat;

typedef enum
{
	IL2CPP_UNHANDLED_POLICY_LEGACY,
	IL2CPP_UNHANDLED_POLICY_CURRENT
} Il2CppRuntimeUnhandledExceptionPolicy;

typedef void (*Il2CppMethodPointer)();

typedef struct Il2CppMethodDebugInfo
{
	Il2CppMethodPointer methodPointer;
	int32_t code_size;
	const char *file;
} Il2CppMethodDebugInfo;

typedef struct
{
	const char *name;
	void (*connect)(const char *address);
	int (*wait_for_attach)(void);
	void (*close1)(void);
	void (*close2)(void);
	int (*send)(void *buf, int len);
	int (*recv)(void *buf, int len);
} Il2CppDebuggerTransport;

#if !__SNC__
#ifndef __has_feature
#define __has_feature(x) 0
#endif
#endif

#if _MSC_VER
typedef wchar_t Il2CppChar;
#elif __has_feature(cxx_unicode_literals)
typedef uint16_t Il2CppChar;
#else
typedef uint16_t Il2CppChar;
#endif

#if _MSC_VER
typedef wchar_t Il2CppNativeChar;
#define IL2CPP_NATIVE_STRING(str) L##str
#else
typedef char Il2CppNativeChar;
#define IL2CPP_NATIVE_STRING(str) str
#endif

typedef void (*il2cpp_register_object_callback)(Il2CppObject **arr, int size, void *userdata);
typedef void (*il2cpp_liveness_reallocate_callback)(void *ptr, size_t size, void *userdata);
typedef void (*Il2CppFrameWalkFunc)(const Il2CppStackFrameInfo *info, void *user_data);
typedef void (*Il2CppProfileFunc)(Il2CppProfiler *prof);
typedef void (*Il2CppProfileMethodFunc)(Il2CppProfiler *prof, const MethodInfo *method);
typedef void (*Il2CppProfileAllocFunc)(Il2CppProfiler *prof, Il2CppObject *obj, Il2CppClass *klass);
typedef void (*Il2CppProfileGCFunc)(Il2CppProfiler *prof, Il2CppGCEvent event, int generation);
typedef void (*Il2CppProfileGCResizeFunc)(Il2CppProfiler *prof, int64_t new_size);
typedef void (*Il2CppProfileFileIOFunc)(Il2CppProfiler *prof, Il2CppProfileFileIOKind kind, int count);
typedef void (*Il2CppProfileThreadFunc)(Il2CppProfiler *prof, unsigned long tid);

typedef const Il2CppNativeChar *(*Il2CppSetFindPlugInCallback)(const Il2CppNativeChar *);
typedef void (*Il2CppLogCallback)(const char *);

typedef size_t (*Il2CppBacktraceFunc)(Il2CppMethodPointer *buffer, size_t maxSize);

typedef uintptr_t il2cpp_array_size_t;
#define ARRAY_LENGTH_AS_INT32(a) ((int32_t)a)

#if IL2CPP_DEBUGGER_ENABLED
struct Il2CppDebugDocument;
struct Il2CppDebugTypeInfo;
struct Il2CppDebugMethodInfo;
struct Il2CppDebugLocalsInfo;
#endif

typedef enum
{
	FRAME_TYPE_MANAGED = 0,
	FRAME_TYPE_DEBUGGER_INVOKE = 1,
	FRAME_TYPE_MANAGED_TO_NATIVE = 2,
	FRAME_TYPE_SENTINEL = 3
} StackFrameType;

struct Il2CppStackFrameInfo
{
	const MethodInfo *method;

#if IL2CPP_DEBUGGER_ENABLED
	int32_t id;
	StackFrameType type;
	uint32_t il_offset;
	void *this_ptr;
	void **params;
	uint32_t params_count;
	void **locals;
	uint32_t locals_count;
#endif
};

typedef struct
{
	void *(*malloc_func)(size_t size);
	void (*free_func)(void *ptr);
	void *(*calloc_func)(size_t nmemb, size_t size);
	void *(*realloc_func)(void *ptr, size_t size);
} Il2CppMemoryCallbacks;

typedef void (*register_object_callback)(void **arr, int size, void *userdata);
typedef void (*WorldChangedCallback)();

typedef struct
{
	uint32_t offset;
	uint32_t typeIndex;
	const char *name;
	bool isStatic;
} Il2CppMetadataField;

typedef enum
{
	kNone = 0,
	kValueType = 1 << 0,
	kArray = 1 << 1,
	kArrayRankMask = 0xFFFF0000
} Il2CppMetadataTypeFlags;

typedef struct
{
	Il2CppMetadataTypeFlags flags;
	Il2CppMetadataField *fields;
	uint32_t fieldCount;
	uint32_t staticsSize;
	uint8_t *statics;
	uint32_t baseOrElementTypeIndex;
	char *name;
	const char *assemblyName;
	uint64_t typeInfoAddress;
	uint32_t size;
} Il2CppMetadataType;

typedef struct
{
	uint32_t typeCount;
	Il2CppMetadataType *types;
} Il2CppMetadataSnapshot;

typedef struct
{
	uint64_t sectionStartAddress;
	uint32_t sectionSize;
	uint8_t *sectionBytes;
} Il2CppManagedMemorySection;

typedef struct
{
	uint32_t sectionCount;
	Il2CppManagedMemorySection *sections;
} Il2CppManagedHeap;

typedef struct
{
	uint32_t stackCount;
	Il2CppManagedMemorySection *stacks;
} Il2CppStacks;

typedef struct
{
	uint32_t gcHandleIndex;
	uint32_t size;
	uint32_t instanceId;
	uint32_t classId;
	uint32_t referencedNativeObjectIndicesCount;
	uint32_t *referencedNativeObjectIndices;
} NativeObject;

typedef struct
{
	uint32_t trackedObjectCount;
	uint64_t *pointersToObjects;
} Il2CppGCHandles;

typedef struct
{
	uint32_t pointerSize;
	uint32_t objectHeaderSize;
	uint32_t arrayHeaderSize;
	uint32_t arrayBoundsOffsetInHeader;
	uint32_t arraySizeOffsetInHeader;
	uint32_t allocationGranularity;
} Il2CppRuntimeInformation;

struct Il2CppManagedMemorySnapshot
{
	Il2CppManagedHeap heap;
	Il2CppStacks stacks;
	Il2CppMetadataSnapshot metadata;
	Il2CppGCHandles gcHandles;
	Il2CppRuntimeInformation runtimeInformation;
	void *additionalUserInformation;
};