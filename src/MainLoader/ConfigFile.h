#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "Helpers.h"

// TODO: W.I.P

#define GLUMITYV2_CONFIG_PATH "./GlumityV2.cfg"

typedef struct
{
    bool useConsole;
    char pluginsPath[MAX_PATH];
} GlumityV2Config;

GlumityV2Config GlumityV2Config_Load();

#define GLUMITYV2_CONFIG_DEFAULT \
    (GlumityV2Config){.useConsole = true, .pluginsPath = "./Plugins"}

#endif