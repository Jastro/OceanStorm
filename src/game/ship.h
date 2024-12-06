// ship.h
#ifndef SHIP_H
#define SHIP_H

// Tipos de barco
#define ShipTypeLarge 0
#define ShipTypeMedium 1
#define ShipTypeSmall 2

// Estados del barco
extern float[MaxShips] ship_x;
extern float[MaxShips] ship_y;
extern float[MaxShips] ship_health;
extern int[MaxShips] ship_type;
extern int[MaxShips] ship_active;

// Estados de torretas en barcos
extern float[MaxShips][3] ship_turret_angle;  // 3 torretas máximo por barco
extern float[MaxShips][3] ship_turret_last_shot;
extern int[MaxShips][3] ship_turret_active;

// Estados de misiles
extern float[MaxMissiles] missile_x;
extern float[MaxMissiles] missile_y;
extern float[MaxMissiles] missile_angle;
extern float[MaxMissiles] missile_speed;
extern float[MaxMissiles] missile_lifetime;
extern int[MaxMissiles] missile_active;

void initialize_ships();
void spawn_ship(float x, float y, int type);
void update_ships();
void render_ships();
void damage_ship(int index, int damage);
void launch_missile(float x, float y, float angle);

#endif