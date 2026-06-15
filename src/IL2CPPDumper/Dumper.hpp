#pragma once
#include <GlumityLib.h>
#include <vector>
#include <windows.h>
#include <cstring>

#define PRINT_HEAD "GlumityV2IL2CPPDumper"
#define MAIN_IMAGE "Assembly-CSharp"

namespace GlumityV2Dumper
{
    class Dumper
    {
    private:
        std::vector<Il2CppClass *> m_gameClasses;
        bool m_initState = false;

        void EnsureIL2CPPThread()
        {
            il2cpp_thread_attach(il2cpp_domain_get());
        }

    public:
        Dumper() = default;

        bool GetInitState() const { return m_initState; }
        const std::vector<Il2CppClass *>& GetClasses() const { return m_gameClasses; }

        Il2CppClass* FindClass(const char* name)
        {
            EnsureIL2CPPThread();
            for (auto* klass : m_gameClasses)
            {
                if (!klass) continue;
                
                const char* kName = il2cpp_class_get_name(klass);
                if (kName && strcmp(kName, name) == 0)
                    return klass;
            }
            return nullptr;
        }

        void FetchClasses(const char* moduleName)
        {
            m_gameClasses.clear();
            EnsureIL2CPPThread();

            size_t count = 0;
            const Il2CppAssembly** assemblies = il2cpp_domain_get_assemblies(il2cpp_domain_get(), &count);

            if (!assemblies || count == 0)
                return;

            const Il2CppImage* targetImage = nullptr;

            for (size_t i = 0; i < count; i++)
            {
                const Il2CppAssembly* asm_ = assemblies[i];
                if (!asm_) continue;

                const Il2CppImage* image = il2cpp_assembly_get_image(asm_);
                if (!image) continue;

                const char* imgName = il2cpp_image_get_name(image);
                if (!imgName) continue;

                char cleanImgName[256];
                strncpy(cleanImgName, imgName, sizeof(cleanImgName) - 1);
                cleanImgName[sizeof(cleanImgName) - 1] = '\0';
                char* dot = strrchr(cleanImgName, '.');
                if (dot) *dot = '\0'; 

                if (strcmp(cleanImgName, moduleName) == 0)
                {
                    targetImage = image;
                    break;
                }
            }

            if (!targetImage)
                return;

            size_t classCount = il2cpp_image_get_class_count((Il2CppImage*)targetImage);

            for (size_t i = 0; i < classCount; i++)
            {
                const Il2CppClass* klass = il2cpp_image_get_class((Il2CppImage*)targetImage, i);
                if (klass)
                    m_gameClasses.push_back((Il2CppClass*)klass);
            }
        }

        void Init()
        {
            LoadIL2CPP(GetModuleHandleA(GAME_ASSEMBLY));
            EnsureIL2CPPThread();

            GlumityPlugin_printf("Initializing IL2CPP Dumper...\n", PRINT_HEAD);

            FetchClasses(MAIN_IMAGE);

            GlumityPlugin_printf("Loaded classes (%d):\n", PRINT_HEAD, (int)m_gameClasses.size());

            m_initState = true;
        }
    };
}