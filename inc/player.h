#pragma once

#include <genesis.h>

#include "physics.h"
#include "xtypes.h"
#include "camera.h"
#include "levelgenerator.h"

struct pBody {
	Sprite* sprite;
	AABB aabb;
	AABB climbingStairAABB;
	AABB globalAABB;

	int facingDirection;
	int speed;
	fix16 acceleration;
	fix16 deceleration;
	int climbingSpeed;
	u16 maxFallSpeed;
	u16 jumpSpeed;

	u16 currentAnimation;

	bool onGround;
	bool onStair;
	bool jumping;
	bool falling;
	bool climbingStair;

	bool runningAnim;

	bool active;
	bool dead;

	Vect2D_s16 position;
	Vect2D_s16 globalPosition;
	Vect2D_u16 centerOffset;

	struct {
		fix16 fixX;
		s16 x;
		fix16 fixY;
	}velocity;

	Vect2D_s16 input;
	s16 hp;
	s16 initialHp;
	int numero;
};

extern struct pBody playerBody;

void playerInputChanged();
void playerInit();
void updatePlayer();