#include "il2cppInternal.h"

#define DO_API(ret, name, args) \
    name##_t name = NULL;

#include "il2cppInternalApiFunctions.h"
#undef DO_API

static void *GlumityResolveSymbol(HMODULE module, const char *name)
{
    return (void *)GetProcAddress(module, name);
}

BOOL LoadIL2CPP(HMODULE module)
{
    if (module == NULL)
    {
        return FALSE;
    }

#define DO_API(ret, name, args)                           \
    name = (name##_t)GlumityResolveSymbol(module, #name); \
    if (name == NULL)                                     \
    {                                                     \
        /* TODO: Handle missing exports here if needed */ \
    }

#include "il2cppInternalApiFunctions.h"
#undef DO_API

    return TRUE;
}