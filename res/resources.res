# 3 * 8 = 24 -> Size of the sprite
#SPRITE  player_sprite          "images/player.png"          3 3   FAST 5
SPRITE  player_sprite          "images/rabbit_gun_run.png"          4 4  FAST 5
SPRITE  mob_sprite          "images/mobs.png"          4 4  FAST 5
SPRITE bullet_sprite "images/bullet.png"        2 2 NONE 0


TILESET level_tileset          "images/level.png"           FAST ALL
MAP     level_map              "images/level.png"           level_tileset FAST 0
PALETTE level_palette          "images/level.png"

XGM     song                   "sound/sonic2Emerald.vgm"    AUTO
WAV     jump                   "sound/jump.wav"             XGM