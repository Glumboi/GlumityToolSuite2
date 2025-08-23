#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <windows.h>
#include <stdio.h>
#include "Plugin.h"
#include "Helpers.h"
#include "ConfigFile.h"

#define DEFAULT_PLUGINCAP 10 * sizeof(GlumityPlugin)

#define PLUGINS_PATH "./Plugins"

typedef struct
{
    GlumityPlugin *plugins;
    PluginBlockList blockList;

    char pluginsPath[DLL_PATH_MAX];
    size_t pluginsCap;
    size_t pluginsCount;

    HMODULE thisDll;
} GlumityPluginLoader;

GlumityPluginLoader GlumityPluginLoader_Create();
uint8_t GlumityPluginLoader_LoadPlugin(GlumityPluginLoader *loader, const char *dllPath);
void GlumityPluginLoader_LoadAllPlugins(GlumityPluginLoader *loader);
void GlumityPluginLoader_ReloadAllPlugins(GlumityPluginLoader *loader);
void GlumityPluginLoader_Destroy(GlumityPluginLoader *loader);
void GlumityPluginLoader_KeyboardRun(GlumityPluginLoader *loader);

#endif