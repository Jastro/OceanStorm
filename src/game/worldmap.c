#include "worldmap.h"

// Variables para manejar el mapa
int[ WorldTilesY ][ WorldTilesX ] world_grid;
tileset world_tiles;
tilemap world_map;

void initialize_world() {  
    // - - - - - - - - - - - - - - - - - - -
    // definir nuestro tileset
    
    // define dimensions for our tiles
    world_tiles.width  = TileSize;
    world_tiles.height = TileSize;
    
    // adjacent tiles have neither space nor overlap
    world_tiles.gap_x = 0;
    world_tiles.gap_y = 0;
    
    // define texture and regions for our tiles
    world_tiles.texture_id = TextureIsland;
    world_tiles.tile_zero_region_id = TileEmpty;
    
    // this particular tile set makes use of its
    // first tile (it's not discarded as transparent)
    world_tiles.draw_tile_zero = true;
    
    // - - - - - - - - - - - - - - - - - - -
    // 2.2: definir nuestro tilemap
    
    // assign our tiles to the map
    world_map.tiles = &world_tiles;
    
    // define the map array
    world_map.map = &world_grid[ 0 ][ 0 ];
    world_map.map_width  = WorldTilesX;
    world_map.map_height = WorldTilesY;
}
