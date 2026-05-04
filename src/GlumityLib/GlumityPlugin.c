#include "GlumityPlugin.h"

void GlumityPlugin_printf(const char *fmt, const char *printHeaderInner, ...)
{
    va_list args;
    va_start(args, printHeaderInner);
    int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    if (len < 0)
        return;

    char *msg = (char *)malloc(len + 1);

    va_start(args, printHeaderInner);
    vsnprintf(msg, len + 1, fmt, args);
    va_end(args);

    printf("[%s]: %s", printHeaderInner, msg);

    free(msg);
}