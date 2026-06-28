#pragma once
#include <windows.h>

#include "il2cppInternalApiTypes.h"
#include "il2cppInternalConfig.h"
#include "il2cppInternalClass.h"
#include "il2cppInternalMetadata.h"

#ifdef __cplusplus
extern "C" {
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