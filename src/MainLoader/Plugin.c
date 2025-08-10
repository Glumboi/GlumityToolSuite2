#include "Plugin.h"

GlumityPlugin GlumityPlugin_LoadPlugin(const char *dllPath)
{
    GlumityPlugin ret = {0};
    memset(ret.dllPath, 0, DLL_PATH_MAX);

    if (!dllPath)
        return ret;

    size_t psz = strlen(dllPath) + 1;
    if (psz > DLL_PATH_MAX)
        return ret;

    memcpy(ret.dllPath, dllPath, DLL_PATH_MAX);

    ret.hDll = LoadLibraryA(ret.dllPath);
    if (ret.hDll)
        ret.entryPoint = (GlumityPlugin_EntryPoint)GetProcAddress(ret.hDll, "GlumityMain");
    return ret;
}

void GlumityPlugin_FreePlugin(GlumityPlugin *plugin)
{
    if (!plugin)
        return;

    plugin->entryPoint = NULL;
    memset(plugin->dllPath, 0, DLL_PATH_MAX);
    FreeLibrary(plugin->hDll);
}
