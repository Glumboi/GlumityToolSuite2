#include "GlumityOldIl2CppInterface.h"

static il2cpp_domain_get_t _il2cpp_domain_get;
static il2cpp_thread_attach_t _il2cpp_thread_attach;
static il2cpp_runtime_invoke_t _il2cpp_runtime_invoke;
static il2cpp_class_get_method_from_name_t _il2cpp_class_get_method_from_name;

struct Il2CppObject *IL2CPP_InvokeMethod(struct Il2CppClass *klass, const char *method_name, void *instance, void **params)
{
    const struct MethodInfo *method =
        il2cpp_class_get_method_from_name(klass, method_name, -1);

    if (!method)
        return 0;

    struct Il2CppObject *exception = 0;

    struct Il2CppObject *result =
        il2cpp_runtime_invoke(method, instance, params, &exception);

    if (exception)
    {
        // TODO: Handle managed exception here
        return 0;
    }

    return result;
}

#ifdef GLUMTIYV2_USE_OLD_IL2CPP_INTERFACE
void IL2CPP_ResolveFunctions()
{
    HMODULE mod = GetModuleHandleA("GameAssembly.dll");

    _il2cpp_domain_get = (il2cpp_domain_get_t)GetProcAddress(mod, "il2cpp_domain_get");
    _il2cpp_thread_attach = (il2cpp_thread_attach_t)GetProcAddress(mod, "il2cpp_thread_attach");
    _il2cpp_runtime_invoke = (il2cpp_runtime_invoke_t)GetProcAddress(mod, "il2cpp_runtime_invoke");
    _il2cpp_class_get_method_from_name = (il2cpp_class_get_method_from_name_t)GetProcAddress(mod, "il2cpp_class_get_method_from_name");
}
#else
#include "il2cppInternal.h"
void IL2CPP_ResolveFunctions()
{
#pragma message("Warning: Currently defaulting to new il2cpp internal api! Make sure the plugin is compatible...")
    LoadIL2CPP(GetModuleHandleA(GAME_ASSEMBLY));
}
#endif