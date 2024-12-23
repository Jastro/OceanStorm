#ifndef WORLDMAP_H
#define WORLDMAP_H

#include "../utils/definitions.h"

// Variables para manejar el mapa
extern int[ WorldTilesY ][ WorldTilesX ] world_grid;
extern tileset world_tiles;
extern tilemap world_map;
extern float screen_shake_time;
extern float screen_shake_angle;

void initialize_world();
void reset_screen_shake();
void update_screen_shake();

#endif
