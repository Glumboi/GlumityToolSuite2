#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "Helpers.h"

#define BLOCKLIST_FILE "./GlumityV2BlockList.txt"

typedef struct
{
    char** blockedPlugins;
    size_t blockedCount;
} PluginBlockList;

PluginBlockList PluginBlockList_Load();
void PluginBlockList_Unload(PluginBlockList* blockList);
bool PluginBlockList_IsInList(PluginBlockList* blockList, char* target);

#endif