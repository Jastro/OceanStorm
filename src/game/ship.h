#ifndef SHIP_H
#define SHIP_H

#define ShipDetectionRange 400
#define ShipRotationSpeed 0.02
#define ShipBaseSpeed 1.0
#define ShipHealth 200
#define MaxMissiles 10

// Estado global del barco
extern float[MaxShips] ship_x;
extern float[MaxShips] ship_y;
extern float[MaxShips] ship_angle;
extern float[MaxShips] ship_target_angle;
extern int[MaxShips] ship_health;
extern int[MaxShips] ship_active;
extern float[MaxShips] ship_blink_timer;
extern int[MaxShips] ship_frame;

// Estado de las torretas de cada barco
extern float[MaxShips] front_turret_angle;
extern float[MaxShips] back_turret_angle;
extern float[MaxShips] rocket_turret_angle;
extern float[MaxShips] turret_fire_timer;
extern float[MaxShips] missile_fire_timer;

// Estado de los misiles
extern float[MaxShips][MaxMissiles] missile_x;
extern float[MaxShips][MaxMissiles] missile_y;
extern float[MaxShips][MaxMissiles] missile_angle;
extern float[MaxShips][MaxMissiles] missile_lifetime;
extern int[MaxShips][MaxMissiles] missile_active;

// Funciones auxiliares
float get_target_angle(float current_angle, float desired_angle);  // <-- Añadir esta línea
void get_turret_position(int ship_index, float offset_x, float offset_y, float* out_x, float* out_y);

// Funciones principales
void initialize_ships();
void spawn_ship(float x, float y);
void update_ships();
void render_ships();
void damage_ship(int index, int damage);
int check_ship_collision(float x, float y, float radius);
void spawn_wave_of_ships(int count);
void update_ship_weapons(int index);
void update_missiles();


#endif