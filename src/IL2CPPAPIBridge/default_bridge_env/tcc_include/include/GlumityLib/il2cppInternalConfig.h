#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef void (*Il2CppMethodPointer)(void);

#if defined(SN_TARGET_ORBIS)
	#define IL2CPP_TARGET_PS4 1
	#define _UNICODE 1
	#define UNICODE 1
#elif defined(_XBOX)
	#define IL2CPP_TARGET_XBOX360 1
	#define _UNICODE 1
	#define UNICODE 1
#elif defined(_XBOX_ONE)
	#define IL2CPP_TARGET_XBOXONE 1
	#define _UNICODE 1
	#define UNICODE 1
#elif defined(_MSC_VER)
	#define IL2CPP_TARGET_WINDOWS 1
	#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
		#define IL2CPP_TARGET_WINRT 1
	#else
		#define IL2CPP_TARGET_WINRT 0
	#endif
	#define _UNICODE 1
	#define UNICODE 1
	#define NOMINMAX 1
	#define STRICT 1
#elif defined(__APPLE__)
	#define IL2CPP_TARGET_DARWIN 1
	#include "TargetConditionals.h"
	#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR || TARGET_TVOS_SIMULATOR
		#define IL2CPP_TARGET_IOS 1
	#endif
#elif defined(__ANDROID__)
	#define IL2CPP_TARGET_ANDROID 1
#elif defined(EMSCRIPTEN)
	#define IL2CPP_TARGET_JAVASCRIPT 1
#elif defined(TIZEN)
    #define IL2CPP_TARGET_TIZEN 1
#elif defined(__linux__)
	#define IL2CPP_TARGET_LINUX 1
#elif defined(NN_PLATFORM_CTR)
	#define IL2CPP_TARGET_N3DS 1
#else
	// Standard C99 Fallback target
	#define IL2CPP_TARGET_LINUX 1
#endif

#ifndef IL2CPP_TARGET_WINDOWS
#define IL2CPP_TARGET_WINDOWS 0
#endif

#ifndef IL2CPP_TARGET_DARWIN
#define IL2CPP_TARGET_DARWIN 0
#endif

#ifndef IL2CPP_TARGET_IOS
#define IL2CPP_TARGET_IOS 0
#endif

#ifndef IL2CPP_TARGET_ANDROID
#define IL2CPP_TARGET_ANDROID 0
#endif

#ifndef IL2CPP_TARGET_JAVASCRIPT
#define IL2CPP_TARGET_JAVASCRIPT 0
#endif

#ifndef IL2CPP_TARGET_TIZEN
#define IL2CPP_TARGET_TIZEN 0
#endif

#ifndef IL2CPP_TARGET_LINUX
#define IL2CPP_TARGET_LINUX 0
#endif

#ifndef IL2CPP_TARGET_XBOX360
#define IL2CPP_TARGET_XBOX360 0
#endif

#ifndef IL2CPP_TARGET_XBOXONE
#define IL2CPP_TARGET_XBOXONE 0
#endif

#ifndef IL2CPP_TARGET_N3DS
#define IL2CPP_TARGET_N3DS 0
#endif

#ifndef IL2CPP_TARGET_PS4
#define IL2CPP_TARGET_PS4 0
#endif

#ifndef IL2CPP_TARGET_PSP2
#define IL2CPP_TARGET_PSP2 0
#endif

#define IL2CPP_TARGET_POSIX (IL2CPP_TARGET_DARWIN || IL2CPP_TARGET_JAVASCRIPT || IL2CPP_TARGET_LINUX || IL2CPP_TARGET_ANDROID || IL2CPP_TARGET_PS4 || IL2CPP_TARGET_PSP2 || IL2CPP_TARGET_TIZEN)
#define IL2CPP_COMPILER_MSVC (IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_XBOXONE)
#define IL2CPP_PLATFORM_WIN32 (IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_XBOXONE)

#ifndef IL2CPP_EXCEPTION_DISABLED
#define IL2CPP_EXCEPTION_DISABLED 0
#endif

#ifndef IL2CPP_API_DYNAMIC_NO_DLSYM
#define IL2CPP_API_DYNAMIC_NO_DLSYM 0
#endif

#ifdef _MSC_VER
# include <malloc.h>
# define IL2CPP_EXPORT __declspec(dllexport)
#elif IL2CPP_TARGET_PSP2 || IL2CPP_TARGET_PS4
# define IL2CPP_EXPORT __declspec(dllexport)
#else
# define IL2CPP_EXPORT __attribute__ ((visibility ("default")))
#endif

#if defined(__ARMCC_VERSION)
	#include <assert.h>
	#include <wchar.h>
	#include <ctype.h>
	#define INTPTR_MAX 2147483647
#endif

#if IL2CPP_TARGET_DARWIN
	#define IL2CPP_METHOD_ATTR
#else
	#define IL2CPP_METHOD_ATTR
#endif

#if defined(_MSC_VER)
	#if defined(_M_X64)
		#define IL2CPP_SIZEOF_VOID_P 8
	#elif defined(_M_IX86) || defined(_M_ARM) || defined(_XBOX)
		#define IL2CPP_SIZEOF_VOID_P 4
	#else
		#error invalid windows architecture
	#endif
#elif defined(__GNUC__) || defined(__SNC__) || defined(__clang__)
	#if defined(__x86_64__) || defined(__amd64__) || defined(__aarch64__) || defined(__arm64__)
		#define IL2CPP_SIZEOF_VOID_P 8
	#else
		#define IL2CPP_SIZEOF_VOID_P 4
	#endif
#else
	#define IL2CPP_SIZEOF_VOID_P 8
#endif

#if defined(_MSC_VER)
#define IL2CPP_CXX_ABI_MSVC 1
#else
#define IL2CPP_CXX_ABI_MSVC 0
#endif

#if IL2CPP_COMPILER_MSVC
    #ifndef STDCALL
        #define STDCALL __stdcall
    #endif
    #ifndef CDECL
        #define CDECL __cdecl
    #endif
#else
    #ifndef STDCALL
        #define STDCALL
    #endif
    #ifndef CDECL
        #define CDECL
    #endif
#endif

#if IL2CPP_COMPILER_MSVC || defined(__ARMCC_VERSION)
#define NORETURN __declspec(noreturn)
#elif IL2CPP_TARGET_DARWIN
#define NORETURN __attribute__ ((noreturn))
#else
#define NORETURN
#endif

#if IL2CPP_COMPILER_MSVC || defined(__ARMCC_VERSION)
#define IL2CPP_NO_INLINE __declspec(noinline)
#else
#define IL2CPP_NO_INLINE __attribute__ ((noinline))
#endif

#define IL2CPP_ENABLE_MONO_BUG_EMULATION 1

// Align macro fix for plain C
#ifndef ALIGN_OF
#if defined(_MSC_VER)
#define ALIGN_OF(T) __alignof(T)
#else
#define ALIGN_OF(T) __alignof__(T)
#endif
#endif
