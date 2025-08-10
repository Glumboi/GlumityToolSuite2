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
        Glumity_printf("FindNextFile failed (%lu)\n", err);
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
