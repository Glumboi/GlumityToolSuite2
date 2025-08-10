#include <GlumityLib.h>
#include "Dumper.hpp"

#define PRINT_HEAD "GlumityV2IL2CPPDumper"
#define MAIN_IMAGE "Assembly-CSharp"

extern "C"
{
    // testing
    const char *const TEST_CLASS_TO_DUMP = "BenchSelection";

    __declspec(dllexport) void GlumityMain(HMODULE glumityInternal)
    {
        GlumityV2Dumper::Dumper dumper{};
        dumper.Init(glumityInternal);

        GlumityPlugin_printf("Printing %s methods...\n", PRINT_HEAD, TEST_CLASS_TO_DUMP);
        dumper.PrintClassMethods(TEST_CLASS_TO_DUMP);
        // dumper.DumpClassMethodOffset(TEST_CLASS_TO_DUMP, );
    }
}