#ifndef ISLAND_H
#define ISLAND_H

#include "../utils/definitions.h"

// Arrays para almacenar datos de las islas
extern float[MaxIslands] island_x;
extern float[MaxIslands] island_y;
extern int[MaxIslands][MaxTilesX][MaxTilesY] island_tiles;
extern int num_islands;

void initialize_islands();
void render_islands(float camera_x, float camera_y);
int is_over_island(float x, float y);

#endif