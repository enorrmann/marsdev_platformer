#include <genesis.h>

#include "levels.h"
#include "camera.h"
#include "player.h"
#include "mob.h"
#include "mobmanager.h"
#include "interactions.h"
#include "dev.h"

void inGameJoyEvent(u16 joy, u16 changed, u16 state);

int main(bool resetType)
{

	VDP_setTextPlane(WINDOW);
	// Soft resets don't clear RAM, this can bring some bugs so we hard reset every time we detect a soft reset
	if (!resetType)
		SYS_hardReset();

	// Initialize joypad and sprite engine in order to use them
	JOY_init();
	SPR_init();

	// Setup the basic things we need for this demo
	loadLevel();
	playerInit();
	initSpawnPoints();
	mobsInit();
	setupCamera(newVector2D_u16(160, 112), 20, 20); // We have to setup always after the player, it directly depends on player's data

	// Setup a callback when a button is pressed, we could call it a "pseudo parallel" joypad handler
	JOY_setEventHandler(inGameJoyEvent);

	SYS_getFPS();

	// Infinite loop where the game is run
	while (TRUE)
	{
		VDP_showFPS(0);
		VDP_showCPULoad();

		// First we update all the things that have to be updated each frame
		updatePlayer();
		spawnMobs();
		updateMobs();
		updateCamera();
		BULLET_update();
		checkHitMobs();

		// Then we update sprites and after that we tell the Mega Drive that we have finished processing all the frame data
		SPR_update();
		SYS_doVBlankProcess();
	}

	return 0;
}

// In order to make this data more accessible from all scripts we write them into a struct from global.h
void inGameJoyEvent(u16 joy, u16 changed, u16 state)
{
	input.joy = joy;
	input.changed = changed;
	input.state = state;

	playerInputChanged();
}