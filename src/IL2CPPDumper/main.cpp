#include <windows.h>
#include <vector>
#include <string>
#include "Dumper.hpp"

#define PRINT_HEAD "GlumityV2IL2CPPDumper"

GlumityV2Dumper::Dumper dumper;

EXPORT void GlumityV2Dumper_WaitForDumper()
{
    while (!dumper.GetInitState())
    {
        Sleep(60);
    }
}

EXPORT void *GlumityV2Dumper_GetFunctionPointer(Il2CppClass *klass, const char *name, int argsCount)
{
    if (!klass || !name || !il2cpp_class_get_method_from_name)
        return nullptr;

    const MethodInfo *method = il2cpp_class_get_method_from_name(klass, name, argsCount);
    if (!method)
        return nullptr;

    return (void *)method->methodPointer;
}

EXPORT char **GlumityV2Dumper_GetEverything()
{
    if (!dumper.GetInitState() || !il2cpp_domain_get || !il2cpp_thread_attach || !il2cpp_class_get_methods || !il2cpp_runtime_class_init)
        return nullptr;

    Il2CppDomain *domain = il2cpp_domain_get();
    if (!domain)
        return nullptr;

    il2cpp_thread_attach(domain);

    std::vector<std::string> tempStrings;

    for (auto *klass : dumper.GetClasses())
    {
        if (!klass)
            continue;

        const char *className = il2cpp_class_get_name(klass);
        if (!className)
            continue;

        il2cpp_runtime_class_init(klass);

        void *iterator = nullptr;
        while (const MethodInfo *method = il2cpp_class_get_methods(klass, &iterator))
        {
            if (!method || !method->name)
                continue;

            std::string entry = std::string(className) + " " + std::string(method->name);
            tempStrings.push_back(entry);
        }
    }

    if (tempStrings.empty())
        return nullptr;

    char **resultList = (char **)malloc((tempStrings.size() + 1) * sizeof(char *));
    if (!resultList)
        return nullptr;

    for (size_t i = 0; i < tempStrings.size(); ++i)
    {
        resultList[i] = _strdup(tempStrings[i].c_str());
    }
    resultList[tempStrings.size()] = nullptr;

    return resultList;
}

EXPORT void *GlumityV2Dumper_GetFunctionPointer_Global(const char *className, const char *functionName)
{
    if (!il2cpp_class_get_method_from_name) return nullptr;
    
    GlumityPlugin_printf("Beginning structured lookup for %s::%s\n", PRINT_HEAD, className, functionName);

    for (auto *klass : dumper.GetClasses())
    {
        if (!klass)
            continue;

        const char *kName = il2cpp_class_get_name(klass);
        if (!kName || strcmp(kName, className) != 0)
            continue;

        GlumityPlugin_printf("Found class match. Querying method pointer safely...\n", PRINT_HEAD);

        const MethodInfo *method = il2cpp_class_get_method_from_name(klass, functionName, -1);

        if (method && method->methodPointer)
        {
            GlumityPlugin_printf("Successfully found method pointer: %p\n", PRINT_HEAD, method->methodPointer);
            return (void *)method->methodPointer;
        }
    }
    return nullptr;
}

EXPORT void *GlumityV2Dumper_GetFunctionPointer_FromModule(
    const char *module,
    const char *className,
    const char *functionName)
{
    if (!module || !className || !functionName || !il2cpp_domain_get || !il2cpp_domain_get_assemblies || !il2cpp_class_get_method_from_name)
        return nullptr;

    Il2CppDomain *domain = il2cpp_domain_get();
    if (!domain)
        return nullptr;

    size_t count = 0;
    const Il2CppAssembly **assemblies = il2cpp_domain_get_assemblies(domain, &count);
    if (!assemblies)
        return nullptr;

    for (size_t i = 0; i < count; i++)
    {
        const Il2CppImage *image = il2cpp_assembly_get_image(assemblies[i]);
        if (!image)
            continue;

        const char *imgName = il2cpp_image_get_name(image);
        if (!imgName)
            continue;

        if (strstr(imgName, module) != nullptr) 
        {
            size_t classCount = il2cpp_image_get_class_count((Il2CppImage *)image);
            for (size_t j = 0; j < classCount; j++)
            {
                const Il2CppClass *klass = il2cpp_image_get_class((Il2CppImage *)image, j);
                if (!klass)
                    continue;

                const char *kName = il2cpp_class_get_name((Il2CppClass *)klass);
                if (kName && strcmp(kName, className) == 0)
                {
                    const MethodInfo *method = il2cpp_class_get_method_from_name((Il2CppClass *)klass, functionName, -1);
                    if (method && method->methodPointer)
                        return (void *)method->methodPointer;
                }
            }
        }
    }
    return nullptr;
}

void InitDumper()
{
    dumper.Init();
}

GLUMITYV2_PLUGIN_ENTRY
{
    GLUMITYV2_PLUGIN_THREADRUN(InitDumper, 0);
}

GLUMITYV2_PLUGIN_EXIT
{
    dumper.~Dumper();
    return TRUE;
}