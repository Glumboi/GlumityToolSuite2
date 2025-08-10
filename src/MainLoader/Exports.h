#ifndef EXPORTS_H
#define EXPORTS_H

#include "PluginLoader.h";

#define EXPORT __declspec(dllexport)

extern GlumityPluginLoader globalLoader;
EXPORT GlumityPluginLoader *GetLoaderInstance();

#endif