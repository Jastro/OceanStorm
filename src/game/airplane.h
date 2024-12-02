#ifndef AIRPLANE_H
#define AIRPLANE_H

// Variables globales para el estado del avión
extern float airplane_x;
extern float airplane_y;
extern float camera_x;
extern float camera_y;
extern float airplane_angle;
extern float airplane_scale;
extern float airplane_velocity;
extern float fuel;
extern int airplane_frame;
extern int anim_timer;
extern int is_player_in_vehicle;

// Funciones del avión
void initialize_airplane();
void reset_airplane();
void update_airplane();
void render_airplane();
void render_fuel_gauge();
int soldier_is_over_carrier(float x, float y);
int is_over_carrier();
void exit_vehicle();

#endif