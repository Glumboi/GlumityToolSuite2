#include "GlumityLib.h"

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
    dumperExports->GlumityV2Dumper_GetFunctionPointer_FromModule = INIT_GLUMITYV2_EXPORT(mod, "GlumityV2Dumper_GetFunctionPointer_FromModule", GlumityV2Dumper_GetFunctionPointer_FromModule_t);
    dumperExports->GlumityV2Dumper_GetFunctionPointerWithPattern = INIT_GLUMITYV2_EXPORT(mod, "GlumityV2Dumper_GetFunctionPointerWithPattern", GlumityV2Dumper_GetFunctionPointerWithPattern_t);
    dumperExports->GlumityV2Dumper_GetFunctionPointer_Global = INIT_GLUMITYV2_EXPORT(mod, "GlumityV2Dumper_GetFunctionPointer_Global", GlumityV2Dumper_GetFunctionPointer_Global_t);
    dumperExports->GlumityV2Dumper_WaitForDumper = INIT_GLUMITYV2_EXPORT(mod, "GlumityV2Dumper_WaitForDumper", GlumityV2Dumper_WaitForDumper_t);
}

char *IL2CPP_String_ToCString(GlumityV2_il2cppStr *str)
{
    int len = str->fields.m_stringLength;
    uint16_t *chars = &str->fields.m_firstChar;

    // TODO: Check for memory leaks, I believe the game should manage it
    char *result = (char *)malloc(len + 1);

    for (int i = 0; i < len; i++)
    {
        result[i] = (char)chars[i];
    }

    result[len] = '\0';
    return result;
}

GlumityV2_il2cppStr *IL2CPP_String_Create(const char *input)
{
    if (!input)
        return NULL;

    size_t len = strlen(input);
    size_t size = sizeof(GlumityV2_il2cppStr) + (len * sizeof(uint16_t));

    GlumityV2_il2cppStr *str = (GlumityV2_il2cppStr *)malloc(size);
    if (!str)
        return NULL;

    str->klass = NULL;
    str->monitor = NULL;

    str->fields.m_stringLength = (int32_t)len;

    uint16_t *buffer = &str->fields.m_firstChar;

    for (size_t i = 0; i < len; i++)
    {
        buffer[i] = (uint16_t)input[i];
    }

    return str;
}
