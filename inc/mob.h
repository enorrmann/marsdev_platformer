#pragma once

#include <genesis.h>

#include "physics.h"
#include "xtypes.h"
#include "camera.h"
#include "levelgenerator.h"
#include "player.h"

void mobInit(struct pBody *body);
void updateMob(struct pBody *body);
void hitMob(struct pBody *mobBody);
