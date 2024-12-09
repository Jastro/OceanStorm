#ifndef ISLAND_H
#define ISLAND_H

#include "../utils/definitions.h"

// Arrays para almacenar datos de las islas
extern float[MaxIslands] island_x;       // x minima (izquierda)
extern float[MaxIslands] island_y;       // y minima (arriba)
extern float[MaxIslands] island_radius;  // radio de separacion
extern int[MaxIslands][MaxTilesX][MaxTilesY] island_tiles;
extern int num_islands;

// mapas de las islas
extern int[8][4][4] small_islands;
extern int[8][6][6] large_islands;

bool initialize_islands();
void render_islands(float camera_x, float camera_y);
int is_over_island(float x, float y);

#endif