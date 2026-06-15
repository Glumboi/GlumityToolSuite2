#include "GlumityOldIl2CppInterface.h"

static il2cpp_domain_get_t _il2cpp_domain_get;
static  il2cpp_thread_attach_t _il2cpp_thread_attach;
static  il2cpp_runtime_invoke_t _il2cpp_runtime_invoke;
static  il2cpp_class_get_method_from_name_t _il2cpp_class_get_method_from_name;

struct IL2CPP_Object *IL2CPP_InvokeMethod(struct IL2CPP_Class *klass, const char *method_name, void *instance, void **params)
{
    const struct IL2CPP_MethodInfo *method =
        _il2cpp_class_get_method_from_name(klass, method_name, -1);

    if (!method)
        return 0;

    struct IL2CPP_Exception *exception = 0;

    struct IL2CPP_Object *result =
        _il2cpp_runtime_invoke(method, instance, params, &exception);

    if (exception)
    {
        // TODO: Handle managed exception here
        return 0;
    }

    return result;
}

void IL2CPP_ResolveFunctions()
{
    HMODULE mod = GetModuleHandleA("GameAssembly.dll");

    _il2cpp_domain_get = (il2cpp_domain_get_t)GetProcAddress(mod, "il2cpp_domain_get");
    _il2cpp_thread_attach = (il2cpp_thread_attach_t)GetProcAddress(mod, "il2cpp_thread_attach");
    _il2cpp_runtime_invoke = (il2cpp_runtime_invoke_t)GetProcAddress(mod, "il2cpp_runtime_invoke");
    _il2cpp_class_get_method_from_name = (il2cpp_class_get_method_from_name_t)GetProcAddress(mod, "il2cpp_class_get_method_from_name");
}