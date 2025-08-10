#pragma once

#include <GlumityLib.h>

#include "IL2CPPResolver/IL2CPP_Resolver.hpp"

#define PRINT_HEAD "GlumityV2IL2CPPDumper"
#define MAIN_IMAGE "Assembly-CSharp"

namespace GlumityV2Dumper
{
    class Dumper
    {
    private:
        std::vector<Unity::il2cppClass *> m_gameClasses;
        Unity::il2cppAssembly **m_gameAssemblies = nullptr;
        GlumityV2Exports m_exports = {0};
        GlumityPluginLoader *m_loader = nullptr;

    public:
        Dumper() = default;

        void Init(HMODULE glumityInternal)
        {
            GlumityPlugin_printf("GlumityMain called, initializing IL2CPP...\n", PRINT_HEAD);
            if (!IL2CPP::Initialize())
            {
                GlumityPlugin_printf("Failed to init IL2CPP, aborting...\n", PRINT_HEAD);
                return;
            }
            GlumityPlugin_printf("Initialized IL2CPP!\n", PRINT_HEAD);

            GlumityPlugin_printf("Initializing GlumityV2Exports...\n", PRINT_HEAD);
            GlumityV2Exports_Init(&m_exports, glumityInternal);
            GlumityPlugin_printf("Initialized GlumityV2Exports!\n", PRINT_HEAD);

            GlumityPlugin_printf("GlumityV2 PluginLoader: %p\n", PRINT_HEAD, m_loader);

            size_t outSz = 0;
            auto asms = IL2CPP::Domain::GetAssemblies(&outSz);
            for (size_t i = 0; i < outSz; i++)
            {
                auto *img = asms[i];
                GlumityPlugin_printf("Found Image: %s\n", PRINT_HEAD, img->m_aName.m_pName);
            }

            IL2CPP::Class::FetchClasses(&m_gameClasses, MAIN_IMAGE, "");
            for (size_t i = 0; i < m_gameClasses.size(); i++)
            {
                auto *c = m_gameClasses[i];
                GlumityPlugin_printf("Found Class: %s | Parent Class: %p\n", PRINT_HEAD, c->m_pName, c->m_pParentClass->m_pName);
            }
        }

        void PrintClassMethods(const char *className)
        {
            if (!className)
                return;

            for (auto *c : m_gameClasses)
            {
                // Check class name
                if (c->m_pName && strcmp(c->m_pName, className) == 0)
                {
                    void *iter = nullptr;
                    Unity::il2cppMethodInfo *method;

                    while ((method = IL2CPP::Class::GetMethods(c, &iter)) != nullptr)
                    {
                        GlumityPlugin_printf("Found %s in %s\n", PRINT_HEAD, method->m_pName, className);
                    }
                }
            }
        }

        uintptr_t DumpClassMethodOffset(const char *className, const char *methodName)
        {
            if (!className || !methodName)
                return 0;

            for (auto *c : m_gameClasses)
            {
                // Check class name
                if (c->m_pName && strcmp(c->m_pName, className) == 0)
                {
                    void *iter = nullptr;
                    Unity::il2cppMethodInfo *method;

                    while ((method = IL2CPP::Class::GetMethods(c, &iter)) != nullptr)
                    {
                        if (method->m_pName && strcmp(method->m_pName, methodName) == 0)
                        {
                            uintptr_t addr = reinterpret_cast<uintptr_t>(method->m_pMethodPointer);
                            uintptr_t offset = addr - reinterpret_cast<uintptr_t>(GetModuleHandleA("GameAssembly.dll"));

                            GlumityPlugin_printf(
                                "Found %s::%s at offset 0x%llX\n",
                                PRINT_HEAD,
                                c->m_pName,
                                method->m_pName,
                                (unsigned long long)offset);

                            return offset;
                        }
                    }
                }
            }

            GlumityPlugin_printf("Method %s not found in class %s\n", PRINT_HEAD, methodName, className);
            return 0;
        }

        ~Dumper() = default;
    };
}