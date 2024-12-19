#ifndef HELI_H
#define HELI_H

// Variables globales para el estado del heli
extern float heli_x;
extern float heli_y;
extern float heli_angle;
extern float heli_scale;
extern float heli_velocity;
extern float fuel;
extern int heli_frame;
extern int anim_timer;
extern int is_player_in_vehicle;
extern int heli_current_ammo;
extern float heli_last_shot_time;
extern int heli_health;
extern float health_flash_timer;

// Funciones del heli
void initialize_heli();
void reset_heli();
void update_heli();
void render_heli();
void render_fuel_gauge();
void exit_vehicle();
void shoot_from_heli();
void reload_heli();
void render_heli_gui();

#endif