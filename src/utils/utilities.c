#include "utilities.h"
#include "definitions.h"
#include "../game/island.h"

float clamp(float value, float min_value, float max_value) {
    if(value < min_value) return min_value;
    if(value > max_value) return max_value;
    return value;
}

bool is_over_carrier(float x, float y) {
    // Comprobar si estamos sobre el carrier completo
    float dx = x - (StartingX - CarrierWidth / 2);
    float dy = y - (StartingY - CarrierHeight / 2);

    // El área de aterrizaje cubre todo el carrier
    return (dx >= 0 && dx <= CarrierWidth) && (dy >= 0 && dy <= CarrierHeight);
}

bool is_over_island(float x, float y) {
    
    int tile_x = x / TileSize;
    int tile_y = y / TileSize;
    return (world_grid[tile_y][tile_x] != TileEmpty);
}

bool is_over_ocean(float x, float y) {
    return !is_over_carrier(x, y) && !is_over_island(x, y );
}
