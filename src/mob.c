#include <genesis.h>
#include "mob.h"

#include "global.h"
#include "map.h"
#include "player.h"

#include "resources.h"
#include "interactions.h"

const u16 oneWayPlatformErrorCorrectionM = 5;
Vect2D_s16 mobStartPos = {0, 0};

void updateMobAnimations(struct pBody *mobBody);
void checkMobCollisions(struct pBody *mobBody);

void mobInit(struct pBody *mobBody)
{

	mobBody->input.x = 1;

	// Create the sprite and palette for the player
	mobBody->sprite = SPR_addSprite(&mob_sprite, mobStartPos.x, mobStartPos.y, TILE_ATTR(PLAYER_PALETTE, FALSE, FALSE, FALSE));
	//PAL_setPalette(PLAYER_PALETTE, player_sprite.palette->data, DMA);

	// Set the global position of the player, the local position will be updated once we are in the main loop
	mobBody->globalPosition = mobStartPos;

	// We set collider size of the player
	mobBody->aabb = NEW_PLAYER_AABB

	// Default movement values
	mobBody->speed = 1;
	mobBody->climbingSpeed = 1;
	mobBody->maxFallSpeed = 6;
	mobBody->jumpSpeed = 7;
	mobBody->facingDirection = 1;
	mobBody->acceleration = FIX16(.25);
	mobBody->deceleration = FIX16(.2);

	// always on ground because we dont check for mob terrain collisions
	mobBody->onGround = FALSE;
	mobBody->climbingStair = FALSE;
	mobBody->active = FALSE;
	mobBody->dead = TRUE;
	mobBody->initialHp = 2;
	mobBody->hp = mobBody->initialHp;

	mobStartPos.x += 30;
}

void updateMobAnimations(struct pBody *mobBody)
{
	// Sprite flip depending on the horizontal input
	if (mobBody->velocity.x > 0)
	{
		SPR_setHFlip(mobBody->sprite, FALSE);
		mobBody->facingDirection = 1;
	}
	else if (mobBody->velocity.x < 0)
	{
		SPR_setHFlip(mobBody->sprite, TRUE);
		mobBody->facingDirection = -1;
	}

	// If the player is on ground and not climbing the stair it can be idle or running
	if (mobBody->velocity.fixY == 0 && !mobBody->climbingStair)
	{
		if (mobBody->velocity.x != 0 && mobBody->runningAnim == FALSE && mobBody->onGround)
		{
			SPR_setAnim(mobBody->sprite, 1);
			mobBody->runningAnim = TRUE;
		}
		else if (mobBody->velocity.x == 0 && mobBody->onGround)
		{
			SPR_setAnim(mobBody->sprite, 0);
			mobBody->runningAnim = FALSE;
		}
	}

	// Climb animation
	if (mobBody->climbingStair)
	{
		SPR_setAnim(mobBody->sprite, 2);
	}
}

void updateMob(struct pBody *mobBody)
{

	if (!mobBody->active || mobBody->dead)
	{
		if (mobBody->position.x > 0)
		{
			SPR_setPosition(mobBody->sprite, 0, 0);
		}
		return;
	}

	if (mobBody->input.x > 0)
	{
		if (mobBody->velocity.x != mobBody->speed)
			mobBody->velocity.fixX += mobBody->acceleration;
	}
	else if (mobBody->input.x < 0)
	{
		if (mobBody->velocity.x != -mobBody->speed)
			mobBody->velocity.fixX -= mobBody->acceleration;
	}
	else if (mobBody->onGround)
	{
		if (mobBody->velocity.x > 0)
			mobBody->velocity.fixX -= mobBody->deceleration;
		else if (mobBody->velocity.x < 0)
			mobBody->velocity.fixX += mobBody->deceleration;
		else
			mobBody->velocity.fixX = 0;
	}
	mobBody->velocity.x = clamp(fix16ToInt(mobBody->velocity.fixX), -mobBody->speed, mobBody->speed);

	// Apply gravity with a terminal velocity
	if (!mobBody->onGround && !mobBody->climbingStair)
	{
		if (fix16ToInt(mobBody->velocity.fixY) <= mobBody->maxFallSpeed)
		{
			mobBody->velocity.fixY = mobBody->velocity.fixY + gravityScale;
		}
		else
		{
			mobBody->velocity.fixY = FIX16(mobBody->maxFallSpeed);
		}
	}

	// Once all the input-related have been calculated, we apply the velocities to the global positions
	mobBody->globalPosition.x += mobBody->velocity.x;
	mobBody->globalPosition.y += fix16ToInt(mobBody->velocity.fixY);

	// Now we can check for collisions and correct those positions
	checkMobCollisions(mobBody);

	// Once the positions are correct, we position the player taking into account the camera position
	mobBody->position.x = mobBody->globalPosition.x - cameraPosition.x;
	mobBody->position.y = mobBody->globalPosition.y - cameraPosition.y;
	SPR_setPosition(mobBody->sprite, mobBody->position.x, mobBody->position.y);

	// Update the player animations
	updateMobAnimations(mobBody);
}

void checkMobCollisions(struct pBody *mobBody)
{

	// Create level limits
	AABB levelLimits = roomSize;

	// Easy access to the bounds in global pos
	if (mobBody->climbingStair)
	{
		playerBounds = newAABB(
			mobBody->globalPosition.x + mobBody->climbingStairAABB.min.x,
			mobBody->globalPosition.x + mobBody->climbingStairAABB.max.x,
			mobBody->globalPosition.y + mobBody->climbingStairAABB.min.y,
			mobBody->globalPosition.y + mobBody->climbingStairAABB.max.y);
	}
	else
	{
		playerBounds = newAABB(
			mobBody->globalPosition.x + mobBody->aabb.min.x,
			mobBody->globalPosition.x + mobBody->aabb.max.x,
			mobBody->globalPosition.y + mobBody->aabb.min.y,
			mobBody->globalPosition.y + mobBody->aabb.max.y);
	}

	// We can see this variables as a way to avoid thinking that a ground tile is a wall tile
	// Skin width (yIntVelocity) changes depending on the vertical velocity
	s16 yIntVelocity = fix16ToRoundedInt(mobBody->velocity.fixY);
	s16 playerHeadPos = mobBody->aabb.min.y - yIntVelocity + mobBody->globalPosition.y;
	s16 playerFeetPos = mobBody->aabb.max.y - yIntVelocity + mobBody->globalPosition.y;

	// Positions in tiles
	Vect2D_u16 minTilePos = posToTile(newVector2D_s16(playerBounds.min.x, playerBounds.min.y));
	Vect2D_u16 maxTilePos = posToTile(newVector2D_s16(playerBounds.max.x, playerBounds.max.y));

	// Used to limit how many tiles we have to check for collision
	Vect2D_u16 tileBoundDifference = newVector2D_u16(maxTilePos.x - minTilePos.x, maxTilePos.y - minTilePos.y);

	// First we check for horizontal collisions
	for (u16 i = 0; i <= tileBoundDifference.y; i++)
	{
		// Height position constant as a helper
		const u16 y = minTilePos.y + i;

		// Right position constant as a helper
		const u16 rx = maxTilePos.x;

		u16 rTileValue = getTileValue(rx, y);
		// After getting the tile value, we check if that is one of whom we can collide/trigger with horizontally
		if (rTileValue == GROUND_TILE)
		{
			AABB tileBounds = getTileBounds(rx, y);
			// Before taking that tile as a wall, we have to check if that is within the player hitbox, e.g. not seeing ground as a wall
			if (tileBounds.min.x < levelLimits.max.x && tileBounds.min.y < playerFeetPos && tileBounds.max.y > playerHeadPos)
			{
				levelLimits.max.x = tileBounds.min.x;
				break;
			}
		}

		// Left position constant as a helper
		const s16 lx = minTilePos.x;

		u16 lTileValue = getTileValue(lx, y);
		// We do the same here but for the left side
		if (lTileValue == GROUND_TILE)
		{
			AABB tileBounds = getTileBounds(lx, y);
			if (tileBounds.max.x > levelLimits.min.x && tileBounds.min.y < playerFeetPos && tileBounds.max.y > playerHeadPos)
			{
				levelLimits.min.x = tileBounds.max.x;
				break;
			}
		}
	}

	// After checking for horizontal positions we can modify the positions if the player is colliding
	if (levelLimits.min.x > playerBounds.min.x)
	{
		mobBody->globalPosition.x = levelLimits.min.x - mobBody->aabb.min.x;
		mobBody->velocity.x = mobBody->velocity.fixX = 0;
		mobBody->input.x *= -1;
	}
	if (levelLimits.max.x < playerBounds.max.x)
	{
		mobBody->globalPosition.x = levelLimits.max.x - mobBody->aabb.max.x;
		mobBody->velocity.x = mobBody->velocity.fixX = 0;
		mobBody->input.x *= -1;
	}

	// Then, we modify the player position so we can use them to check for vertical collisions
	if (mobBody->climbingStair)
	{
		playerBounds = newAABB(
			mobBody->globalPosition.x + mobBody->climbingStairAABB.min.x,
			mobBody->globalPosition.x + mobBody->climbingStairAABB.max.x,
			mobBody->globalPosition.y + mobBody->climbingStairAABB.min.y,
			mobBody->globalPosition.y + mobBody->climbingStairAABB.max.y);
	}
	else
	{
		playerBounds = newAABB(
			mobBody->globalPosition.x + mobBody->aabb.min.x,
			mobBody->globalPosition.x + mobBody->aabb.max.x,
			mobBody->globalPosition.y + mobBody->aabb.min.y,
			mobBody->globalPosition.y + mobBody->aabb.max.y);
	}

	// And do the same to the variables that are used to check for them
	minTilePos = posToTile(newVector2D_s16(playerBounds.min.x, playerBounds.min.y));
	maxTilePos = posToTile(newVector2D_s16(playerBounds.max.x - 1, playerBounds.max.y));
	tileBoundDifference = newVector2D_u16(maxTilePos.x - minTilePos.x, maxTilePos.y - minTilePos.y);

	bool onStair = FALSE;

	// To avoid having troubles with player snapping to ground ignoring the upward velocity, we separate top and bottom collisions depending on the velocity
	if (yIntVelocity >= 0)
	{
		for (u16 i = 0; i <= tileBoundDifference.x; i++)
		{
			s16 x = minTilePos.x + i;
			u16 y = maxTilePos.y;

			// This is the exact same method that we use for horizontal collisions
			u16 bottomTileValue = getTileValue(x, y);
			if (bottomTileValue == GROUND_TILE || bottomTileValue == ONE_WAY_PLATFORM_TILE)
			{
				if (getTileRightEdge(x) == levelLimits.min.x || getTileLeftEdge(x) == levelLimits.max.x)
					continue;

				u16 bottomEdgePos = getTileTopEdge(y);
				// The error correction is used to add some extra width pixels in case the player isn't high enough by just some of them
				if (bottomEdgePos < levelLimits.max.y && bottomEdgePos >= (playerFeetPos - oneWayPlatformErrorCorrectionM))
				{
					levelLimits.max.y = bottomEdgePos;
				}
			}
		}
	}
	else
	{
		for (u16 i = 0; i <= tileBoundDifference.x; i++)
		{
			s16 x = minTilePos.x + i;
			u16 y = minTilePos.y;

			// And the same once again
			u16 topTileValue = getTileValue(x, y);
			if (topTileValue == GROUND_TILE)
			{
				if (getTileRightEdge(x) == levelLimits.min.x || getTileLeftEdge(x) == levelLimits.max.x)
					continue;

				u16 upperEdgePos = getTileBottomEdge(y);
				if (upperEdgePos < levelLimits.max.y)
				{
					levelLimits.min.y = upperEdgePos;
					break;
				}
			}
		}
	}

	// Now we modify the player position and some properties if necessary
	if (levelLimits.min.y > playerBounds.min.y)
	{
		mobBody->globalPosition.y = levelLimits.min.y - mobBody->aabb.min.y;
		mobBody->velocity.fixY = 0;
	}
	if (levelLimits.max.y <= playerBounds.max.y)
	{
		if (levelLimits.max.y == 768)
		{
			mobBody->falling = TRUE;
		}
		else
		{
			mobBody->onStair = onStair;
			mobBody->onGround = TRUE;
			mobBody->climbingStair = FALSE;
			mobBody->jumping = FALSE;
			mobBody->globalPosition.y = levelLimits.max.y - mobBody->aabb.max.y;
			mobBody->velocity.fixY = 0;
		}
	}
	else
	{
		mobBody->onStair = mobBody->onGround = FALSE;
	}
	// This time we don't need to update the playerBounds as they will be updated at the beginning of the function the next frame
	mobBody->globalAABB = playerBounds; // para no calcularlo nuevamente en interactions.c
}

void hitMob(struct pBody *mobBody)
{
		//debug(mobBody->numero, "hit numero", 7);
	mobBody->hp--; // Reducir la vida del mob
	if (mobBody->hp <= 0)
	{
		mobBody->dead = TRUE;
		mobBody->active = FALSE;

		mobBody->globalPosition.x = 0;
		mobBody->globalPosition.y = 0;
		SPR_setPosition(mobBody->sprite, 0, 0);

	}
}