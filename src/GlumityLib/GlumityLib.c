#include "GlumityLib.h"

#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <MinHook.h>

static il2cpp_domain_get_t _il2cpp_domain_get;
static  il2cpp_thread_attach_t _il2cpp_thread_attach;
static  il2cpp_runtime_invoke_t _il2cpp_runtime_invoke;
static  il2cpp_class_get_method_from_name_t _il2cpp_class_get_method_from_name;

BOOL Glumity_GetErrorMessage(DWORD dwErrorCode, LPTSTR pBuffer, DWORD cchBufferLength)
{
    if (cchBufferLength == 0)
    {
        return FALSE;
    }

    DWORD cchMsg = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, /* (not used with FORMAT_MESSAGE_FROM_SYSTEM) */
                                 dwErrorCode,
                                 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                 pBuffer,
                                 cchBufferLength,
                                 NULL);
    return (cchMsg > 0);
}

GlumityV2_il2cppStr *IL2CPP_String_Create(const char *input)
{
    if (!input)
        return NULL;

    size_t len = strlen(input);
    size_t size = sizeof(GlumityV2_il2cppStr) + (len * sizeof(uint16_t));

    GlumityV2_il2cppStr *str = (GlumityV2_il2cppStr *)malloc(size);
    if (!str)
        return NULL;

    str->klass = NULL;
    str->monitor = NULL;

    str->fields.m_stringLength = (int32_t)len;

    uint16_t *buffer = &str->fields.m_firstChar;

    for (size_t i = 0; i < len; i++)
    {
        buffer[i] = (uint16_t)input[i];
    }

    return str;
}

void IL2CPP_ResolveFunctions()
{
    HMODULE mod = GetModuleHandleA("GameAssembly.dll");

    _il2cpp_domain_get = (il2cpp_domain_get_t)GetProcAddress(mod, "il2cpp_domain_get");
    _il2cpp_thread_attach = (il2cpp_thread_attach_t)GetProcAddress(mod, "il2cpp_thread_attach");
    _il2cpp_runtime_invoke = (il2cpp_runtime_invoke_t)GetProcAddress(mod, "il2cpp_runtime_invoke");
    _il2cpp_class_get_method_from_name = (il2cpp_class_get_method_from_name_t)GetProcAddress(mod, "il2cpp_class_get_method_from_name");
}

struct IL2CPP_Object *IL2CPP_InvokeMethod(struct IL2CPP_Class *klass, const char *method_name,
                                          void *instance, void **params)
{
    const struct IL2CPP_MethodInfo *method =
        _il2cpp_class_get_method_from_name(klass, method_name, -1);

    if (!method)
        return NULL;

    struct IL2CPP_Exception *exception = NULL;

    // 2. Invoke the method
    // For static methods, pass NULL as the 'instance'
    struct IL2CPP_Object *result =
        _il2cpp_runtime_invoke(method, instance, params, &exception);

    if (exception)
    {
        // Handle managed exception here if needed
        return NULL;
    }

    return result;
}
