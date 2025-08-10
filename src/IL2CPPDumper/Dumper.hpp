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

        bool InitIL2CPPResolver()
        {
            // init IL2CPP
            if (IL2CPP::Initialize(true) && IL2CPP::Thread::Attach(IL2CPP::Domain::Get()))
            {
                return true;
            }

            return false;
        }

    public:
        Dumper() = default;

        void Init()
        {
            GlumityPlugin_printf("GlumityMain called, initializing IL2CPP...\n", PRINT_HEAD);
            if (!InitIL2CPPResolver())
            {
                GlumityPlugin_printf("Failed to init IL2CPP, aborting...\n", PRINT_HEAD);
                return;
            }
            GlumityPlugin_printf("Initialized IL2CPP!\n", PRINT_HEAD);

            GlumityPlugin_printf("Initializing GlumityV2Exports...\n", PRINT_HEAD);
            GlumityV2Exports_Init(&m_exports);
            GlumityPlugin_printf("Initialized GlumityV2Exports!\n", PRINT_HEAD);

            m_loader = m_exports.GetLoaderInstance();
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

        ~Dumper()
        {
            m_gameClasses.clear();
        };
    };

}