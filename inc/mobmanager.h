#pragma once

#include <genesis.h>

#include "physics.h"
#include "xtypes.h"
#include "camera.h"
#include "levelgenerator.h"
#include "player.h"

typedef struct SpawnPoint
{
    Vect2D_s16 position;
    struct pBody *mobPBody;
    bool spawned;
} ;
void initSpawnPoints();
void mobsInit();
void updateMobs();
void spawnMobs();
void spawnMob(struct pBody *body);