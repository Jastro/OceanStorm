#ifndef BOMB_H
#define BOMB_H

#include "../utils/definitions.h"

// Declarar los arrays como externos
extern float[MaxActiveBombs] bomb_x;
extern float[MaxActiveBombs] bomb_y;
extern float[MaxActiveBombs] bomb_timer;
extern int[MaxActiveBombs] bomb_active;

// Prototipos de las funciones
void initialize_bombs();
void update_bombs();
void render_bombs();
int can_plant_bomb(float x, float y);
void plant_bomb(float x, float y);
void check_bomb_explosions();

#endif