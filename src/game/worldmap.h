#ifndef WORLDMAP_H
#define WORLDMAP_H

#include "../utils/definitions.h"

// Variables para manejar el mapa
extern int[ WorldTilesY ][ WorldTilesX ] world_grid;
extern tileset world_tiles;
extern tilemap world_map;

void initialize_world();

#endif
