#include <GlumityLib.h>
#include <vector>
#include <windows.h>
#include "Dumper.hpp"

#define PRINT_HEAD "GlumityV2IL2CPPDumper"

GlumityV2Dumper::Dumper dumper;

EXPORT void GlumityV2Dumper_WaitForDumper()
{
    while (!dumper.GetInitState())
        Sleep(60);
}

EXPORT void *GlumityV2Dumper_GetFunctionPointer(Il2CppClass *klass, const char *name, int argsCount)
{
    if (!klass || !name)
        return nullptr;

    const MethodInfo *method = il2cpp_class_get_method_from_name(klass, name, argsCount);
    if (!method)
        return nullptr;

    return (void *)method->methodPointer;
}

EXPORT char **GlumityV2Dumper_GetEverything()
{
    il2cpp_thread_attach(il2cpp_domain_get());

    std::vector<char *> tempContainer;

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

            size_t neededSize = strlen(className) + 1 + strlen(method->name) + 1;
            char *allocation = (char *)malloc(neededSize);

            if (allocation)
            {
                sprintf_s(allocation, neededSize, "%s %s", className, method->name);
                tempContainer.push_back(allocation);
            }
        }
    }

    if (tempContainer.empty())
        return nullptr;

    char **finalResultBlock = (char **)malloc((tempContainer.size() + 1) * sizeof(char *));
    if (!finalResultBlock)
    {
        for (char *ptr : tempContainer)
            free(ptr);
        return nullptr;
    }

    for (size_t i = 0; i < tempContainer.size(); i++)
    {
        finalResultBlock[i] = tempContainer[i];
    }

    finalResultBlock[tempContainer.size()] = nullptr;

    return finalResultBlock;
}

EXPORT void *GlumityV2Dumper_GetFunctionPointer_Global(const char *className, const char *functionName)
{
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
    if (!module || !className || !functionName)
        return nullptr;

    size_t count = 0;
    const Il2CppAssembly **assemblies = il2cpp_domain_get_assemblies(il2cpp_domain_get(), &count);
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

        if (strstr(imgName, module) != nullptr) // Loose name matching
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
    return 0;
}