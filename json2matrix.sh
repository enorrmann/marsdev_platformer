cp ./c_template/matrix_header.c map.c
awk '{print "{",$0, "},"}' /home/emilio/game_dev/genesis_level_2/simplified/Level_0/IntGrid.csv >> map.c
echo "};" >> map.c
mv map.c ./src
convert /home/emilio/game_dev/genesis_level_2/simplified/Level_0/_composite.png -colors 255 ./res/images/level.png 

