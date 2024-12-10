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
    for(int i = 0; i < num_islands; i++) {
        float local_x = (x - island_x[i]) / TileSize;
        float local_y = (y - island_y[i]) / TileSize;
        
        if(local_x >= 0 && local_x < MaxTilesX && 
           local_y >= 0 && local_y < MaxTilesY) {
            int tile_x = local_x;
            int tile_y = local_y;
            if(island_tiles[i][tile_x][tile_y] != TileEmpty) {
                return 1;
            }
        }
    }
    return 0;
}

bool is_over_ocean(float x, float y) {
    return !is_over_carrier(x, y) && !is_over_island(x, y );
}
