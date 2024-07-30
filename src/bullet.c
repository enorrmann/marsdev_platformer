#include <genesis.h>
#include "bullet.h"
#include "resources.h"

#define FIRE_DELAY 5

bool initialized = FALSE;
s16 cameraOffset = 0;

struct Bullet bullets[MAX_BULLETS + 1];
struct Bullet *getUnusedBullet();
int fire_timer = FIRE_DELAY;

void BULLET_init()
{
	struct Bullet _bullet;
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bullets[i].sprite = SPR_addSprite(&bullet_sprite, 0, 0, TILE_ATTR(PLAYER_PALETTE, FALSE, FALSE, FALSE));
		bullets[i].speed = 6;
		bullets[i].active = FALSE;
	}

	initialized = TRUE;
}

void BULLET_fire(struct pBody *shooter)
{
	if (!initialized)
	{
		BULLET_init();
	}
	if (fire_timer > 0)
	{
		return;
	}

	struct Bullet *b = getUnusedBullet();
	if (b != NULL)
	{
		b->position = shooter->position;
		b->position.x +=shooter->centerOffset.x;
		b->position.y +=shooter->centerOffset.y;
		b->direction.x = shooter->facingDirection;
		fire_timer = FIRE_DELAY;
	}

	// cameraOffset = cameraPosition.x;
}

void BULLET_update()
{

	if (fire_timer > 0)
	{
		fire_timer--;
	}

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (!bullets[i].active)
		{
			if (bullets[i].position.x > 0)
			{
				SPR_setPosition(bullets[i].sprite, 0, 0);
			}
			continue;
		}
		// screen bounds
		if (bullets[i].position.x < -10 || bullets[i].position.x > 320)
		{
			bullets[i].active = FALSE;
			SPR_setPosition(bullets[i].sprite, 0, 0);
		}
		else
		{
			bullets[i].position.x = bullets[i].position.x + (bullets[i].speed * bullets[i].direction.x) - (cameraPosition.x - cameraOffset);
			SPR_setPosition(bullets[i].sprite, bullets[i].position.x, bullets[i].position.y);
		}
	}
	cameraOffset = cameraPosition.x;
}

struct Bullet *getUnusedBullet()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (!bullets[i].active)
		{
			bullets[i].active = TRUE;
			return &bullets[i];
		}
	}
	return NULL;
}