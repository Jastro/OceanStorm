#ifndef SHIP_H
#define SHIP_H

#define MaxShips 6
#define MinLargeShips 2
#define DefaultTurretOffset 20

// Tipos de barcos
#define ShipTypeLarge 0
#define ShipTypeMedium 1
#define ShipTypeSmall 2

// Estados del barco
#define ShipStateActive 0
#define ShipStateDestroyed 1

// Puntos de montaje de armas
#define WeaponMountFront 0
#define WeaponMountMiddle 1
#define WeaponMountBack 2

// Arrays para almacenar el estado de los barcos
extern float[MaxShips] ship_x;
extern float[MaxShips] ship_y;
extern float[MaxShips] ship_angle;
extern float[MaxShips] ship_speed;
extern int[MaxShips] ship_type;
extern int[MaxShips] ship_state;
extern int[MaxShips] ship_health;
extern int[MaxShips] ship_active;

// Arrays para las armas de los barcos grandes
extern float[MaxShips][3] ship_weapon_offset_x;  // Offset X para cada punto de montaje
extern float[MaxShips][3] ship_weapon_offset_y;  // Offset Y para cada punto de montaje
extern int[MaxShips][3] ship_weapon_type;        // 0 = torreta, 1 = lanzacohetes

void initialize_ships();
void spawn_ships();
void update_ships();
void render_ships();
int find_valid_ship_position(float* out_x, float* out_y);
int check_ship_collision(float x, float y, float width, float height, int current_ship);
void damage_ship(int index, int damage);

#endif