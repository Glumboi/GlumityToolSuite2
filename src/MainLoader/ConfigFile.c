#include "ConfigFile.h"

PluginBlockList PluginBlockList_Load()
{
    PluginBlockList ret = {0};
    FILE *f = fopen(BLOCKLIST_FILE, "r");
    if (!f)
        return ret;

    char line[64];
    while (fgets(line, sizeof(line), f))
    {
        // trim new line
        line[strcspn(line, "\n")] = 0;

        void *temp = realloc(ret.blockedPlugins, (ret.blockedCount + 1) * sizeof(char *));
        if (temp)
            ret.blockedPlugins = temp;
        ret.blockedPlugins[ret.blockedCount++] = strdup(line);

        Glumity_printf("Blocked plugin: %s\n", ret.blockedPlugins[ret.blockedCount - 1]);
    }
    fclose(f);
    return ret;
}

void PluginBlockList_Unload(PluginBlockList *blockList)
{
    if (!blockList)
        return;
    for (size_t i = 0; i < blockList->blockedCount; i++)
    {
        free(blockList->blockedPlugins[i]);
    }

    blockList->blockedCount = 0;
    free(blockList->blockedPlugins);
    blockList->blockedPlugins = NULL;
}

bool PluginBlockList_IsInList(PluginBlockList *blockList, char *target)
{
    if (!blockList || !target)
        return false;

    char *targetName = Glumity_GetFileNameFromPath(target);

    for (size_t i = 0; i < blockList->blockedCount; i++)
    {
        if (strcmp(blockList->blockedPlugins[i], targetName) == 0)
            return true;
    }

    return false;
}
