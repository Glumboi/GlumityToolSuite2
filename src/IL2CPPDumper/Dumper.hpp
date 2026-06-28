#pragma once
#include <windows.h>
#include <vector>
#include <cstring>
#include <GlumityLib.h>

#define PRINT_HEAD "GlumityV2IL2CPPDumper"
#define MAIN_IMAGE "Assembly-CSharp"

#ifdef __cplusplus
extern "C"
{
#endif
#include "il2cppInternal.h"
#ifdef __cplusplus
}
#endif

namespace GlumityV2Dumper
{
    class Dumper
    {
    private:
        std::vector<Il2CppClass *> m_gameClasses;
        bool m_initState = false;
        const char *lastImage = nullptr;

        void EnsureIL2CPPThread()
        {
            if (il2cpp_domain_get)
            {
                Il2CppDomain *domain = il2cpp_domain_get();
                if (domain && il2cpp_thread_attach)
                {
                    il2cpp_thread_attach(domain);
                }
            }
        }

    public:
        Dumper() = default;

        bool GetInitState() const { return m_initState; }
        const std::vector<Il2CppClass *> &GetClasses() const { return m_gameClasses; }

        Il2CppClass *FindClass(const char *name)
        {
            EnsureIL2CPPThread();
            for (auto *klass : m_gameClasses)
            {
                if (!klass)
                    continue;

                const char *kName = il2cpp_class_get_name(klass);
                if (kName && strcmp(kName, name) == 0)
                    return klass;
            }
            return nullptr;
        }

        // Ensure lastImage is initialized to nullptr somewhere globally/in class header:
        // const char* lastImage = nullptr;

        void FetchClasses(const char *imageTargetName)
        {
            if (imageTargetName == nullptr)
                return;

            if (lastImage != nullptr && strcmp(imageTargetName, lastImage) == 0)
            {
                GlumityPlugin_printf("Aborting: read to previously cached image detected! (%s).\n", PRINT_HEAD, imageTargetName);
                return;
            }

            m_gameClasses.clear();
            EnsureIL2CPPThread();

            if (lastImage != nullptr)
            {
            }

            if (!il2cpp_domain_get || !il2cpp_domain_get_assemblies)
            {
                GlumityPlugin_printf("CRITICAL EXCEPTION: IL2CPP domain functions are missing!\n", PRINT_HEAD);
                return;
            }

            Il2CppDomain *domain = il2cpp_domain_get();
            if (!domain)
            {
                GlumityPlugin_printf("Aborting: Active IL2CPP Domain context returned null.\n", PRINT_HEAD);
                return;
            }

            size_t count = 0;
            GlumityPlugin_printf("Retrieving loaded IL2CPP assemblies...\n", PRINT_HEAD);
            const Il2CppAssembly **assemblies = il2cpp_domain_get_assemblies(domain, &count);

            if (!assemblies || count == 0)
            {
                GlumityPlugin_printf("Error: Failed to locate assemblies array or count is zero.\n", PRINT_HEAD);
                return;
            }
            GlumityPlugin_printf("Found %d total assemblies inside game domain.\n", PRINT_HEAD, (int)count);

            const Il2CppImage *targetImage = nullptr;

            for (size_t i = 0; i < count; i++)
            {
                const Il2CppAssembly *asm_ = assemblies[i];
                if (!asm_ || !il2cpp_assembly_get_image || !il2cpp_image_get_name)
                    continue;

                const Il2CppImage *image = il2cpp_assembly_get_image(asm_);
                if (!image)
                    continue;

                const char *imgName = il2cpp_image_get_name(image);
                if (!imgName)
                    continue;

                std::string currentImageName(imgName);
                std::string targetName(imageTargetName);

                // Ensure both have or don't have .dll for an exact equal comparison
                if (currentImageName == targetName || currentImageName == targetName + ".dll")
                {
                    GlumityPlugin_printf("Matched exact target metadata image context: %s\n", PRINT_HEAD, imgName);
                    targetImage = image;
                    break;
                }
            }

            if (!targetImage)
            {
                GlumityPlugin_printf("Error: Could not resolve target assembly '%s'.\n", PRINT_HEAD, imageTargetName);
                return;
            }

            if (!il2cpp_image_get_class_count || !il2cpp_image_get_class)
            {
                GlumityPlugin_printf("CRITICAL ERROR: Class retrieval maps are null!\n", PRINT_HEAD);
                return;
            }

            size_t classCount = il2cpp_image_get_class_count((Il2CppImage *)targetImage);
            GlumityPlugin_printf("Parsing classes. Total metadata classes discovered: %d\n", PRINT_HEAD, (int)classCount);

            m_gameClasses.reserve(classCount);

            for (size_t i = 0; i < classCount; i++)
            {
                const Il2CppClass *klass = il2cpp_image_get_class((Il2CppImage *)targetImage, i);
                if (klass)
                {
                    m_gameClasses.push_back((Il2CppClass *)klass);
                }
            }

            if (lastImage)
            {
                free((void *)lastImage);
            }
            lastImage = strdup(imageTargetName);
        }

        void Init()
        {
            GlumityPlugin_printf("Starting Dumper initialization thread...\n", PRINT_HEAD);

            HMODULE gameAssemblyHandle = nullptr;
            int loadRetries = 0;

            while (!gameAssemblyHandle && loadRetries < 200)
            {
                gameAssemblyHandle = GetModuleHandleA("GameAssembly.dll");
                if (!gameAssemblyHandle)
                    gameAssemblyHandle = GetModuleHandleW(L"GameAssembly.dll");
                if (!gameAssemblyHandle)
                    gameAssemblyHandle = LoadLibraryA("GameAssembly.dll");

                if (!gameAssemblyHandle)
                {
                    Sleep(100);
                    loadRetries++;
                }
            }

            if (!gameAssemblyHandle)
            {
                GlumityPlugin_printf("CRITICAL TIMEOUT: GameAssembly.dll could not be mapped into address space!\n", PRINT_HEAD);
                return;
            }

            GlumityPlugin_printf("Found target binary base address: %p\n", PRINT_HEAD, gameAssemblyHandle);
            GlumityPlugin_printf("Invoking dynamic runtime function binding via LoadIL2CPP...\n", PRINT_HEAD);

            if (!LoadIL2CPP(gameAssemblyHandle))
            {
                GlumityPlugin_printf("WARNING: Certain signatures failed to resolve via native exports/scanners.\n", PRINT_HEAD);
            }

            if (il2cpp_domain_get == nullptr)
            {
                GlumityPlugin_printf("CRITICAL ERROR: 'il2cpp_domain_get' resolved to null! Aborting dump pipeline.\n", PRINT_HEAD);
                return;
            }
            GlumityPlugin_printf("Successfully bound 'il2cpp_domain_get' pointer address: %p\n", PRINT_HEAD, (void *)il2cpp_domain_get);

            if (!il2cpp_domain_get())
            {
                GlumityPlugin_printf("Unity IL2CPP runtime domain is spinning up. Waiting for engine attachment...\n", PRINT_HEAD);
                while (!il2cpp_domain_get())
                {
                    Sleep(100);
                }
            }
            GlumityPlugin_printf("Unity Engine active IL2CPP Domain is initialized and validated.\n", PRINT_HEAD);

            GlumityPlugin_printf("Ensuring current background thread is attached to IL2CPP context...\n", PRINT_HEAD);
            EnsureIL2CPPThread();
            GlumityPlugin_printf("Thread attachment complete.\n", PRINT_HEAD);

            GlumityPlugin_printf("Initializing IL2CPP Dumper...\n", PRINT_HEAD);
            FetchClasses(MAIN_IMAGE);

            int safeClassCount = static_cast<int>(m_gameClasses.size());
            GlumityPlugin_printf("Loaded classes (%d):\n", PRINT_HEAD, safeClassCount);

            m_initState = true;
            GlumityPlugin_printf("Dumper state flagged as fully initialized.\n", PRINT_HEAD);
        }
    };
}