#pragma once

#include <genesis.h>

#include "physics.h"
#include "xtypes.h"
#include "camera.h"
#include "levelgenerator.h"
#include "player.h"

#define MAX_NUM_MOBS 6

typedef struct SpawnPoint
{
    Vect2D_s16 position;
    struct pBody *mobPBody;
    bool spawned;
};

extern struct pBody *mobs[MAX_NUM_MOBS + 1];

void initSpawnPoints();
void mobsInit();
void updateMobs();
void spawnMobs();
