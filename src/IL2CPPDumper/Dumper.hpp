#pragma once

#include <GlumityLib.h>

#include <IL2CPP_Resolver.hpp>

#define PRINT_HEAD "GlumityV2IL2CPPDumper"
#define MAIN_IMAGE "Assembly-CSharp"

namespace GlumityV2Dumper
{
    class Dumper
    {
    private:
        std::vector<Unity::il2cppClass *> m_gameClasses;
        Unity::il2cppAssembly **m_gameAssemblies = nullptr;
        Unity::il2cppAssembly *m_gameMainAssembly = nullptr;
        GlumityPluginLoader *m_loader = nullptr;
        bool m_initState = false;

        bool InitIL2CPPResolver()
        {
            // init IL2CPP
            m_initState = (IL2CPP::Initialize(true) && IL2CPP::Thread::Attach(IL2CPP::Domain::Get()));
            return m_initState;
        }

    public:
        Dumper() = default;

        bool GetInitState() const
        {
            return m_initState;
        }

        void Init()
        {
            GlumityPlugin_printf("GlumityMain called, initializing IL2CPP...\n", PRINT_HEAD);
            if (!InitIL2CPPResolver())
            {
                GlumityPlugin_printf("Failed to init IL2CPP, aborting...\n", PRINT_HEAD);
                return;
            }
            GlumityPlugin_printf("Initialized IL2CPP!\n", PRINT_HEAD);

            size_t outSz = 0;
            m_gameAssemblies = IL2CPP::Domain::GetAssemblies(&outSz);
#ifdef __DEBUG
            for (size_t i = 0; i < outSz; i++)
            {
                auto *img = m_gameAssemblies[i];
                GlumityPlugin_printf("Found Image: %s\n", PRINT_HEAD, img->m_aName.m_pName);
            }
#endif

            IL2CPP::Class::FetchClasses(&m_gameClasses, MAIN_IMAGE, "");
#ifdef __DEBUG
            for (size_t i = 0; i < m_gameClasses.size(); i++)
            {
                auto *c = m_gameClasses[i];
                GlumityPlugin_printf("Found Class: %s | Parent Class: %s\n", PRINT_HEAD, c->m_pName, c->m_pParentClass->m_pName);
            }
#endif
        }

        ~Dumper()
        {
            m_gameClasses.clear();
        };
    };

}