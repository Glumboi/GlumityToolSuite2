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