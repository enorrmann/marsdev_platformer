#include "levels.h"

#include "levelgenerator.h"
#include "map.h"
#include "global.h"

#include "resources.h"

void loadLevel() {
	//Setup the level background with the MAP tool from SGDK
	PAL_setPalette(LEVEL_PALETTE, level_palette.data, DMA);
	VDP_loadTileSet(&level_tileset, VDPTilesFilled, DMA);
	bga = MAP_create(&level_map, TILEMAP_PLANE, TILE_ATTR_FULL(LEVEL_PALETTE, FALSE, FALSE, FALSE, VDPTilesFilled));

	//Update the number of tiles filled in order to avoid overlaping them when loading more
	VDPTilesFilled += level_tileset.numTile;

	//We need to call this function at some point, this place seems to be a good one for doing it
	generateCollisionMap(collisionMap);

	//Start play the level's song
	// PLAY SONG
	//XGM_startPlay(song);
}
