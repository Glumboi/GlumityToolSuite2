#include <GlumityLib.h>

GlumityV2Exports exports;

__declspec(dllexport) void GlumityMain(HMODULE glumityInternal)
{
    GlumityV2Exports_Init(&exports, glumityInternal);
    GlumityPluginLoader *loader = exports.GetLoaderInstance();
    if (loader)
    {
        GlumityPlugin_printf("Found loader\nTotal plugins loaded: %d\n", "Example Plugin", loader->pluginsCount);
    }
}