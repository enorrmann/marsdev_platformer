#pragma once

#include <genesis.h>

#include "resources.h"
#include "physics.h"
#include "xtypes.h"
#include "camera.h"
#include "levelgenerator.h"
#include "player.h"

struct Bullet
{
    Sprite *sprite;
    Vect2D_s16 position;
    Vect2D_s16 direction;
    u16 hp;
    int speed;
    bool active;
};

void BULLET_fire(struct pBody *shooter);

void BULLET_update();