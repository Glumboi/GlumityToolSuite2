#include "GlumityLib.h"

char **Glumity_FileSystem_GetAllDllFilesFromDirectory(const char *dir, int *outCount)
{
    if (!dir)
        return NULL;

    char searchPath[MAX_PATH];
    snprintf(searchPath, MAX_PATH, "%s\\*.dll", dir);

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath, &findData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return NULL;
    }

    int capacity = 10;
    int count = 0;
    char **dllFiles = (char **)malloc(capacity * sizeof(char *));
    if (!dllFiles)
    {
        FindClose(hFind);
        return NULL;
    }

    do
    {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            char fullPath[MAX_PATH];
            snprintf(fullPath, MAX_PATH, "%s\\%s", dir, findData.cFileName);

            if (count >= capacity)
            {
                capacity *= 2;
                char **temp = (char **)realloc(dllFiles, capacity * sizeof(char *));
                if (!temp)
                {
                    // Cleanup already allocated strings
                    for (int i = 0; i < count; ++i)
                        free(dllFiles[i]);
                    free(dllFiles);
                    FindClose(hFind);
                    return NULL;
                }
                dllFiles = temp;
            }

            dllFiles[count] = _strdup(fullPath); // or strdup depending on compiler
            if (!dllFiles[count])
            {
                for (int i = 0; i < count; ++i)
                    free(dllFiles[i]);
                free(dllFiles);
                FindClose(hFind);
                return NULL;
            }

            count++;
        }
    } while (FindNextFileA(hFind, &findData) != 0);

    DWORD err = GetLastError();
    if (err != ERROR_NO_MORE_FILES)
    {
        GlumityPlugin_printf("FindNextFile failed (%lu)\n", GLUMITYLIB_PRINT_HEADER, err);
    }

    FindClose(hFind);

    if (count == 0)
    {
        free(dllFiles);
        return NULL;
    }

    if (outCount)
        *outCount = count;

    return dllFiles;
}

char **Glumity_FileSystem_GetAllFilesWithExtensionFromDirectory(const char *dir, const char *extension, int *outCount)
{
    if (!dir)
        return NULL;

    char searchPath[MAX_PATH];
    snprintf(searchPath, MAX_PATH, "%s\\*%s", dir, extension);

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath, &findData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return NULL;
    }

    int capacity = 10;
    int count = 0;
    char **files = (char **)malloc(capacity * sizeof(char *));
    if (!files)
    {
        FindClose(hFind);
        return NULL;
    }

    do
    {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            char fullPath[MAX_PATH];
            snprintf(fullPath, MAX_PATH, "%s\\%s", dir, findData.cFileName);

            if (count >= capacity)
            {
                capacity *= 2;
                char **temp = (char **)realloc(files, capacity * sizeof(char *));
                if (!temp)
                {
                    // Cleanup already allocated strings
                    for (int i = 0; i < count; ++i)
                        free(files[i]);
                    free(files);
                    FindClose(hFind);
                    return NULL;
                }
                files = temp;
            }

            files[count] = _strdup(fullPath); // or strdup depending on compiler
            if (!files[count])
            {
                for (int i = 0; i < count; ++i)
                    free(files[i]);
                free(files);
                FindClose(hFind);
                return NULL;
            }

            count++;
        }
    } while (FindNextFileA(hFind, &findData) != 0);

    DWORD err = GetLastError();
    if (err != ERROR_NO_MORE_FILES)
    {
        GlumityPlugin_printf("FindNextFile failed (%lu)\n", GLUMITYLIB_PRINT_HEADER, err);
    }

    FindClose(hFind);

    if (count == 0)
    {
        free(files);
        return NULL;
    }

    if (outCount)
        *outCount = count;

    return files;
}

const char *Glumity_GetFileNameFromPath(const char *path)
{
    const char *slash = strrchr(path, '/');
    const char *backslash = strrchr(path, '\\');
    const char *filename = path;

    if (slash && backslash)
        filename = (slash > backslash) ? slash + 1 : backslash + 1;
    else if (slash)
        filename = slash + 1;
    else if (backslash)
        filename = backslash + 1;

    return filename;
}

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
        GlumityPlugin_printf("Failed to init GlumityV2IL2CPPDumper exports, make sure the GlumityV2IL2CPPDumper plugin is installed and loaded!\n", GLUMITYLIB_PRINT_HEADER);
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

    // TODO: Check for memory leaks, I believe the game manages it automatically though
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

void IL2CPP_ResolveFunctions()
{
    HMODULE mod = GetModuleHandleA("GameAssembly.dll");

    _il2cpp_domain_get = (il2cpp_domain_get_t)GetProcAddress(mod, "il2cpp_domain_get");
    _il2cpp_thread_attach = (il2cpp_thread_attach_t)GetProcAddress(mod, "il2cpp_thread_attach");
    _il2cpp_runtime_invoke = (il2cpp_runtime_invoke_t)GetProcAddress(mod, "il2cpp_runtime_invoke");
    _il2cpp_class_get_method_from_name = (il2cpp_class_get_method_from_name_t)GetProcAddress(mod, "il2cpp_class_get_method_from_name");
}

struct IL2CPP_Object *IL2CPP_InvokeMethod(struct IL2CPP_Class *klass, const char *method_name,
                                          void *instance, void **params)
{
    const struct IL2CPP_MethodInfo *method =
        _il2cpp_class_get_method_from_name(klass, method_name, -1);

    if (!method)
        return NULL;

    struct IL2CPP_Exception *exception = NULL;

    // 2. Invoke the method
    // For static methods, pass NULL as the 'instance'
    struct IL2CPP_Object *result =
        _il2cpp_runtime_invoke(method, instance, params, &exception);

    if (exception)
    {
        // Handle managed exception here if needed
        return NULL;
    }

    return result;
}
