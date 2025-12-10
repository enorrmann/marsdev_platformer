#!/bin/bash

# Usage: ./json2matrix.sh <level_name>
# Example: ./json2matrix.sh Level_0

if [ -z "$1" ]; then
    echo "Error: level name required"
    echo "Usage: $0 <level_name>"
    echo "Example: $0 Level_0"
    exit 1
fi

LEVEL_NAME="$1"
LDTK_BASE="./ldtk/genesis_level_2/simplified"
LEVEL_DIR="$LDTK_BASE/$LEVEL_NAME"

# Check if level directory exists
if [ ! -d "$LEVEL_DIR" ]; then
    echo "Error: level directory not found: $LEVEL_DIR"
    exit 1
fi

cp ./c_template/matrix_header.c map.c
./generate_mob_array.sh "$LEVEL_DIR/data.json" 
awk '{print "{",$0, "},"}' "$LEVEL_DIR/Background.csv" >> map.c
echo "};" >> map.c
mv map.c ./src
convert "$LEVEL_DIR/_composite.png" -colors 255 ./res/images/level.png 

