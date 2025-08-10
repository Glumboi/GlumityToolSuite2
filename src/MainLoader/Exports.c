#include "Exports.h"

GlumityPluginLoader globalLoader = {0};

__declspec(dllexport) GlumityPluginLoader* GetLoaderInstance()
{
    return &globalLoader;
}
