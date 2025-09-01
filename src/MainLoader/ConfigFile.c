#include "ConfigFile.h"

GlumityV2Config GlumityV2Config_Load()
{
    GlumityV2Config ret = GLUMITYV2_CONFIG_DEFAULT;

    FILE *f = fopen(GLUMITYV2_CONFIG_PATH, "r");
    if (!f) // File does not exist, create default
    {
        f = fopen(GLUMITYV2_CONFIG_PATH, "w");
        if (f)
        {
            fprintf(f, "useConsole=%d\n", ret.useConsole);
            fprintf(f, "pluginsPath=%s\n", ret.pluginsPath);
            fclose(f);
        }
        return ret;
    }

    char line[256];
    while (fgets(line, sizeof(line), f))
    {
        char key[128];
        char value[128];

        if (sscanf(line, "%127[^=]=%127s", key, value) == 2)
        {
            if (strcmp(key, "useConsole") == 0)
            {
                ret.useConsole = atoi(value);
                continue;
            }

            if (strcmp(key, "pluginsPath") == 0)
            {
                strcpy(ret.pluginsPath, value);
                continue;
            }
            // Add more keys here if needed in the future
        }
    }

    fclose(f);
    return ret;
}


