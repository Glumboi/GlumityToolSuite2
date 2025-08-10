#ifndef HELPERS_H
#define HELPERS_H

#include <windows.h>
#include "PluginLoader.h"

#ifdef __cplusplus
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif
#define GLUMITY_PRINT_HEADER "[GlumityToolsuiteV2]: "

/// @brief Printf for glumity specific code, uses malloc and free for a temp char*, caller doesn't need to manage
/// @param fmt
/// @param
void Glumity_printf(const char *fmt, ...);

/// @brief Caller has to free
/// @param dir
/// @param outCount
/// @return allocated using malloc/realloc
char **Glumity_FileSystem_GetAllDllFilesFromDirectory(const char *dir, int *outCount);

const char *Glumity_GetFileNameFromPath(const char *path);

#endif