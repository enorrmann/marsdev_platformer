#include <genesis.h>
#include "mob.h"
#include "mobmanager.h"

#include "global.h"
#include "map.h"
#include "player.h"

#include "resources.h"
#include "interactions.h"

#define NUM_SPAWN_POINTS 6

int spawned = 0;
struct pBody *mobs[MAX_NUM_MOBS + 1];
struct SpawnPoint *spawnPoints[NUM_SPAWN_POINTS + 1];

void initSpawnPoints()
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (collisionMap[y][x] == 8)
			{

				if (spawned < NUM_SPAWN_POINTS)
				{
					// debug(x * 16, "x", 1);
					// debug(y * 16 - 15, "y", 2);
					spawnPoints[spawned] = (struct SpawnPoint *)malloc(sizeof(struct SpawnPoint));
					spawnPoints[spawned]->position.x = x * 16;
					spawnPoints[spawned]->position.y = y * 16 - 15;
					spawnPoints[spawned]->mobPBody = NULL;
					spawnPoints[spawned]->spawned = FALSE;

					spawned++;

				}
			}
		}
	}
}

void spawnMobs()
{
	for (int i = 0; i < NUM_SPAWN_POINTS; i++)
	{
		if (spawnPoints[i]->spawned)
		{
			continue;
		}
		s16 xDistance = abs(playerBody.globalPosition.x - spawnPoints[i]->position.x);
		if (xDistance < 100)
		{
			spawnUnusedMobAt(spawnPoints[i]);
		}
	}
}

void mobsInit()
{

	for (int i = 0; i < MAX_NUM_MOBS; i++)
	{
		mobs[i] = (struct pBody *)malloc(sizeof(struct pBody));
		mobs[i]->numero = i;
		mobInit(mobs[i]);
	}
}
void updateMobs()
{

	for (int i = 0; i < MAX_NUM_MOBS; i++)
	{
		updateMob(mobs[i]);
		bool colision = checkAABBIntersection(&playerBody, mobs[i]);
		if (colision)
		{
			// hitMob(mobs[i]);
			//  reset, un mob tocó al player 1
			SYS_reset();
		}
	}
}

void spawnUnusedMobAt(struct SpawnPoint *spawnPoint)
{
	for (int i = 0; i < MAX_NUM_MOBS; i++)
	{
		if (mobs[i]->dead)
		{
			mobs[i]->hp = mobs[i]->initialHp;
			mobs[i]->globalPosition.x = spawnPoint->position.x;
			mobs[i]->globalPosition.y = spawnPoint->position.y;

			mobs[i]->dead = FALSE;
			mobs[i]->active = TRUE;
			spawnPoint->spawned = TRUE;
			return;
		}
	}
}
