#include "PluginLoader.h"

GlumityPluginLoader GlumityPluginLoader_Create()
{
    GlumityPluginLoader ret = {
        .plugins = NULL,
        .pluginsCap = DEFAULT_PLUGINCAP,
        .pluginsCount = 0,
        .thisDll = NULL};
    memset(ret.pluginsPath, 0, DLL_PATH_MAX);

    ret.plugins = (GlumityPlugin *)malloc(ret.pluginsCap * sizeof(GlumityPlugin));
    if (!ret.plugins)
    {
        ret.pluginsCap = 0;
        return ret;
    }

    CreateDirectoryA(PLUGINS_PATH, NULL);
    strncpy(ret.pluginsPath, PLUGINS_PATH, DLL_PATH_MAX - 1);

    ret.thisDll = GetModuleHandleA("GlumityToolSuite2");

    return ret;
}

uint8_t GlumityPluginLoader_LoadPlugin(GlumityPluginLoader *loader, const char *dllPath)
{
    if (!loader || !dllPath)
        return 0;

    if (loader->pluginsCount >= loader->pluginsCap)
    {
        loader->pluginsCap *= 2;
        GlumityPlugin *temp = (GlumityPlugin *)realloc(loader->plugins, loader->pluginsCap);
        if (!temp)
            return 0;
        loader->plugins = temp;
    }

    loader->plugins[loader->pluginsCount++] = GlumityPlugin_LoadPlugin(dllPath);

    return 1;
}

void GlumityPluginLoader_LoadAllPlugins(GlumityPluginLoader *loader)
{
    if (!loader || loader->pluginsPath[0] == '\0')
        return;

    int cnt = 0;
    char **dllFiles = Glumity_FileSystem_GetAllDllFilesFromDirectory(loader->pluginsPath, &cnt);

    if (!dllFiles)
        return;

    for (size_t i = 0; i < cnt; i++)
    {
        if (!GlumityPluginLoader_LoadPlugin(loader, dllFiles[i]))
        {
            Glumity_printf("Failed to load plugin: %s\n", dllFiles[i]);
        }
        else
        {
            Glumity_printf("Loaded plugin: %s\n", dllFiles[i]);
            if (loader->plugins[i].entryPoint)
            {

                loader->plugins[i].entryPoint();
                continue;
            }
            Glumity_printf("Entrypoint doesn't exist in plugin!\n");
        }
    }

    free(dllFiles);
}

void GlumityPluginLoader_ReloadAllPlugins(GlumityPluginLoader *loader)
{
    if (!loader)
        return;

    GlumityPluginLoader_Destroy(loader);
    GlumityPluginLoader_LoadAllPlugins(loader);
}

void GlumityPluginLoader_Destroy(GlumityPluginLoader *loader)
{
    if (!loader)
        return;

    Glumity_printf("Destroying all plugins...\n");

    for (size_t i = 0; i < loader->pluginsCount; i++)
    {
        GlumityPlugin_FreePlugin(&loader->plugins[i]);
    }

    loader->pluginsCount = 0;
    loader->pluginsCap = 0;
    free(loader->plugins);
    loader->plugins = NULL;
}

void GlumityPluginLoader_KeyboardRun(GlumityPluginLoader *loader)
{
    while (1)
    {
        if (GetAsyncKeyState(VK_F9) & 0x8000)
        {
            GlumityPluginLoader_ReloadAllPlugins(loader);
            Sleep(200); // Delay to avoid multiple prints
        }
    }
}