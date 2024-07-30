#pragma once

#include <genesis.h>

#include "resources.h"
#include "physics.h"
#include "xtypes.h"
#include "camera.h"
#include "levelgenerator.h"
#include "player.h"

#define MAX_BULLETS 1

struct Bullet
{
    Sprite *sprite;
    Vect2D_s16 position;
    Vect2D_s16 direction;
    u16 hp;
    int speed;
    bool active;
};

extern struct Bullet bullets[MAX_BULLETS + 1];

void BULLET_fire(struct pBody *shooter);

void BULLET_update();