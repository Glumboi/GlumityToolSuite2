#include <GlumityLib.h>

#include "defines.h"
#include "LuaAPI.h"

int InitLua(void)
{
    GlumityLuaLoader_Init();
    // GlumityLuaLoader_LoadScripts();
    GlumityLuaLoader_LoadScripts_MT();
    return 0;
}

GLUMITYV2_PLUGIN_ENTRY
{
    GLUMITYV2_PLUGIN_THREADRUN(InitLua, NULL);
}

GLUMITYV2_PLUGIN_EXIT
{
    GlumityLuaLoader_Destroy();
}