#define MY_PLUGIN "TestJITPlugin2"

void GlumityPlugin_printf(const char *format, const char *pluginHeader, ...);

int GlumityMain()
{
    GlumityPlugin_printf("Hello, World!", MY_PLUGIN);
    return 0;
}