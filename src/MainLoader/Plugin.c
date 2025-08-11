#include "Plugin.h"
#include "Helpers.h"

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

    ret.name = Glumity_GetFileNameFromPath(ret.dllPath);

    ret.hDll = LoadLibraryA(ret.dllPath);
    if (ret.hDll)
    {
        ret.entryPoint = (GlumityPlugin_EntryPoint)GetProcAddress(ret.hDll, "GlumityMain");
        ret.exitPoint = (GlumityPlugin_ExitPoint)GetProcAddress(ret.hDll, "GlumityExit");
    }
    return ret;
}

void GlumityPlugin_FreePlugin(GlumityPlugin *plugin)
{
    if (!plugin)
        return;

    if (plugin->exitPoint)
        plugin->exitPoint();

    plugin->entryPoint = NULL;
    plugin->exitPoint = NULL;
    memset(plugin->dllPath, 0, DLL_PATH_MAX);
    FreeLibrary(plugin->hDll);
}
