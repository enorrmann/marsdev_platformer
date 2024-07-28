# 3 * 8 = 24 -> Size of the sprite
SPRITE  player_sprite          "images/player.png"          3 3   FAST 5
SPRITE bullet_sprite "images/bullet_8x8.png"        1 1 NONE 0
#SPRITE  player_sprite_2          "images/player_2.png"          4 4   FAST 5

TILESET level_tileset          "images/level.png"           FAST ALL
MAP     level_map              "images/level.png"           level_tileset FAST 0
PALETTE level_palette          "images/level.png"

XGM     song                   "sound/sonic2Emerald.vgm"    AUTO
WAV     jump                   "sound/jump.wav"             XGM