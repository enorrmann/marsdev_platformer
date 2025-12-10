cp ./c_template/matrix_header.c map.c
./generate_mob_array.sh ./ldtk/genesis_level_2/simplified/Level_0/data.json 
awk '{print "{",$0, "},"}' ./ldtk/genesis_level_2/simplified/Level_0/Background.csv >> map.c
echo "};" >> map.c
mv map.c ./src
convert ./ldtk/genesis_level_2/simplified/Level_0/_composite.png -colors 255 ./res/images/level.png 

