#!/bin/bash

# Default values if no parameters provided
INPUT_JSON=${1:-"level.json"}
OUTPUT_C=${2:-"src/mob_positions.c"}

# Check jq
if ! command -v jq >/dev/null 2>&1; then
    echo "Error: jq no está instalado."
    exit 1
fi

# Extract x,y from mobs
MOBS=$(jq -r '.entities.Mob[] | "\(.x) \(.y)"' "$INPUT_JSON")

{
    echo "#include \"mob_positions.h\""
    echo ""
    echo "Vect2D_u16 mob_positions[] = {"

    while read -r X Y; do
        echo "    { $X, $Y },"
    done <<< "$MOBS"

    echo "};"
    echo ""
    echo "const u16 mob_positions_count = sizeof(mob_positions) / sizeof(mob_positions[0]);"
} > "$OUTPUT_C"

echo "Archivo generado: $OUTPUT_C"
