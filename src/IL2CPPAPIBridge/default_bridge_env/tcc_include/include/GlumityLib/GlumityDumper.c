#include "GlumityDumper.h"
#include "GlumityLib.h"

void GlumityV2DumperExports_Init(GlumityV2DumperExports *dumperExports)
{
    if (!dumperExports)
        return;

    HMODULE mod = GetModuleHandleA(GLUMITYV2_DUMPER_MODULE);
    if (!mod)
    {
        GlumityPlugin_printf("Failed to init GlumityV2IL2CPPDumper exports, make sure the GlumityV2IL2CPPDumper plugin is installed and loaded!\n", GLUMITYLIB_PRINT_HEADER);
        return;
    }

    dumperExports->GlumityV2Dumper_GetFunctionPointer = INIT_GLUMITYV2_EXPORT(mod, "GlumityV2Dumper_GetFunctionPointer", GlumityV2Dumper_GetFunctionPointer_t);
    dumperExports->GlumityV2Dumper_GetFunctionPointer_FromModule = INIT_GLUMITYV2_EXPORT(mod, "GlumityV2Dumper_GetFunctionPointer_FromModule", GlumityV2Dumper_GetFunctionPointer_FromModule_t);
    dumperExports->GlumityV2Dumper_GetFunctionPointerWithPattern = INIT_GLUMITYV2_EXPORT(mod, "GlumityV2Dumper_GetFunctionPointerWithPattern", GlumityV2Dumper_GetFunctionPointerWithPattern_t);
    dumperExports->GlumityV2Dumper_GetFunctionPointer_Global = INIT_GLUMITYV2_EXPORT(mod, "GlumityV2Dumper_GetFunctionPointer_Global", GlumityV2Dumper_GetFunctionPointer_Global_t);
    dumperExports->GlumityV2Dumper_WaitForDumper = INIT_GLUMITYV2_EXPORT(mod, "GlumityV2Dumper_WaitForDumper", GlumityV2Dumper_WaitForDumper_t);
}