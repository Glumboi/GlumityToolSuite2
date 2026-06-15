#pragma once

// LINK of inspiration: https://github.com/phoenixperry/intro_to_game_dev_unity/tree/master/week08/class_code/New%20Unity%20Project/test_ios/Libraries/libil2cpp/include

#include "il2cppInternalApiTypes.h"
#include "il2cppInternalConfig.h"
#include "il2cppInternalClass.h"
#include "il2cppInternalMetadata.h"

#if IL2CPP_API_DYNAMIC_NO_DLSYM
IL2CPP_EXPORT void il2cpp_api_register_symbols(void);
IL2CPP_EXPORT void *il2cpp_api_lookup_symbol(const char *name);
#endif

#ifndef _WIN32
typedef int BOOL;
typedef void *HMODULE;
#else
#include <windows.h>
#endif

static void *GlumityResolveSymbol(HMODULE module, const char *name);

#ifdef __cplusplus
extern "C"
{
#endif

#define DO_API(ret, name, args)  \
    typedef ret(*name##_t) args; \
    extern name##_t name;

#include "il2cppInternalApiFunctions.h"
#undef DO_API

BOOL LoadIL2CPP(HMODULE module);

#ifdef __cplusplus
}
#endif
