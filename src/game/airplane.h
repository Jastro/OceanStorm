#ifndef AIRPLANE_H
#define AIRPLANE_H

// Variables globales para el estado del avión
extern float airplane_x;
extern float airplane_y;
extern float airplane_angle;
extern float airplane_scale;
extern float airplane_velocity;

// Funciones del avión
void initialize_airplane();
void reset_airplane();
void update_airplane();
void render_airplane();

#endif