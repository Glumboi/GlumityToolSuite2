#include "Helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void Glumity_printf(const char *fmt, ...)
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
    printf("%s%s", GLUMITY_PRINT_HEADER, msg);

    free(msg);
}
