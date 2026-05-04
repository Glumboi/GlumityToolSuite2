#ifndef HELPERS_H
#define HELPERS_H

#include <windows.h>

#ifdef __cplusplus
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

#define GLUMITY_PRINT_HEADER "[GlumityV2]: "

/// @brief Printf for glumity specific code, uses malloc and free for a temp char*, caller doesn't need to manage
/// @param fmt
/// @param
void Glumity_printf(const char *fmt, ...);

#endif