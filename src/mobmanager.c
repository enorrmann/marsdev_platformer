#include <genesis.h>
#include "mob.h"
#include "mobmanager.h"

#include "global.h"
#include "map.h"
#include "player.h"

#include "resources.h"
#include "interactions.h"

#define MAX_NUM_MOBS 3
#define NUM_SPAWN_POINTS 2

struct pBody *mobs[MAX_NUM_MOBS + 1];
struct SpawnPoint *spawnPoints[NUM_SPAWN_POINTS + 1];
void hitMob(struct pBody *mobBody);
void initSpawnPoints()
{
	spawnPoints[0] = (struct SpawnPoint *)malloc(sizeof(struct SpawnPoint));
	spawnPoints[1] = (struct SpawnPoint *)malloc(sizeof(struct SpawnPoint));
	spawnPoints[0]->position.x = 200;
	spawnPoints[0]->position.y = 120;
	spawnPoints[0]->mobPBody = NULL;
	spawnPoints[0]->spawned = FALSE;
	spawnPoints[1]->position.x = 500;
	spawnPoints[1]->position.y = 168;
	spawnPoints[1]->mobPBody = NULL;
	spawnPoints[1]->spawned = FALSE;
}

void spawnMobs()
{
	for (int i = 0; i < NUM_SPAWN_POINTS; i++)
	{

		s16 xDistance = abs(playerBody.globalPosition.x - spawnPoints[i]->position.x);
		/*debug(playerBody.globalPosition.x, "globalPosition.x", 1);
		debug(playerBody.globalPosition.y, "globalPosition.y", 2);
		debug(spawnPoints[0]->position.x, "spawnPoint.x", 3);
		debug(xDistance, "xDistance", 4);*/
		if (xDistance < 100 && !spawnPoints[i]->spawned)
		{
			// mobs[i]->globalPosition.x = spawnPoints[i]->position.x;
			// mobs[i]->globalPosition.y = spawnPoints[i]->position.y;

			spawnUnusedMobAt(spawnPoints[i]);
		}
	}
}

void mobsInit()
{

	for (int i = 0; i < MAX_NUM_MOBS; i++)
	{
		mobs[i] = (struct pBody *)malloc(sizeof(struct pBody));
		mobInit(mobs[i]);
	}
}
void updateMobs()
{

	for (int i = 0; i < MAX_NUM_MOBS; i++)
	{
		updateMob(mobs[i]);
		int colision = checkAABBIntersection(&playerBody, mobs[i]);
		if (colision)
		{
			hitMob(mobs[i]);
		}
		// debug(colision, "colision", i + 1);
	}
}

void spawnUnusedMobAt(struct SpawnPoint *spawnPoint)
{
	for (int i = 0; i < MAX_NUM_MOBS; i++)
	{
		if (mobs[i]->dead)
		{
			mobs[i]->globalPosition.x = spawnPoint->position.x;
			mobs[i]->globalPosition.y = spawnPoint->position.y;

			mobs[i]->dead = FALSE;
			mobs[i]->active = TRUE;
			spawnPoint->spawned = TRUE;
			return;
		}
	}
}
void hitMob(struct pBody *mobBody)
{
	mobBody->dead = TRUE;
	mobBody->active = FALSE;
	mobBody->globalPosition.x = 0;
	mobBody->globalPosition.y = 0;
	SPR_setPosition(mobBody->sprite, 0, 0);
}