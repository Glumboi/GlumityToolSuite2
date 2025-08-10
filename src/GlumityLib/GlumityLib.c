#include "GlumityLib.h"

void GlumityPlugin_printf(const char *fmt, const char *printHeaderInner, ...)
{
    // determine required buffer size
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    if (len < 0)
        return;

    // format message
    char *msg =
        (char *)malloc(len + 1); // or use heap allocation if implementation doesn't support VLAs

    va_start(args, fmt);
    vsnprintf(msg, len + 1, fmt, args);
    va_end(args);

    // call myFunction
    printf("[%s]: %s", printHeaderInner, msg);

    free(msg);
}

void GlumityV2Exports_Init(GlumityV2Exports *exports, HMODULE mod)
{
    if (!exports || !mod)
        return;

    // Init exports
    exports->GetLoaderInstance = INIT_GLUMITYV2_EXPORT(mod, "GetLoaderInstance", GetLoaderInstance_t);
}

void GlumityV2DumperExports_Init(GlumityV2DumperExports *dumperExports)
{
    if (!dumperExports)
        return;

    HMODULE mod = GetModuleHandleA(GLUMITYV2_DUMPER_MODULE);
    if (!mod)
    {
        Glumity_printf("Failed to init GlumityV2IL2CPPDumper exports, make sure the GlumityV2IL2CPPDumper plugin is installed and loaded!\n");
        return;
    }

    dumperExports->GlumityV2Dumper_GetFunctionPointer = INIT_GLUMITYV2_EXPORT(mod, "GlumityV2Dumper_GetFunctionPointer", GlumityV2Dumper_GetFunctionPointer_t);
}
