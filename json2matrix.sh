cp ./c_template/matrix_header.c map.c
./merge.sh ./ldtk/genesis_level_2/simplified/Level_0/Background.csv ./ldtk/genesis_level_2/simplified/Level_0/Mobs.csv >> ./ldtk/genesis_level_2/simplified/Level_0/pp.csv
awk '{print "{",$0, "},"}' ./ldtk/genesis_level_2/simplified/Level_0/pp.csv >> map.c
echo "};" >> map.c
mv map.c ./src
convert ./ldtk/genesis_level_2/simplified/Level_0/Background.png -colors 255 ./res/images/level.png 

