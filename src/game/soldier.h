#ifndef SOLDIER_H
#define SOLDIER_H

#define MaxArmor 3
#define ImmunityTime 1.0
#define SoldierSpeed 2.0
#define BombCount 2

// Estados del soldado
#define SoldierStateNone 0
#define SoldierStateActive 1
#define SoldierStateBlinking 2
#define SoldierStateImmune 3

// Variables del soldado
extern float soldier_x;
extern float soldier_y;
extern float soldier_angle;
extern int soldier_state;
extern int soldier_armor;
extern int soldier_bombs;
extern float soldier_immunity_timer;
extern float soldier_blink_timer;
extern int[3] soldier_has_weapon;
extern float camera_zoom;
extern float target_zoom;
extern float[MaxBullets] bullet_x;
extern float[MaxBullets] bullet_y; 
extern float[MaxBullets] bullet_angle;
extern float[MaxBullets] bullet_speed;
extern float[MaxBullets] bullet_damage;
extern float[MaxBullets] bullet_distance;
extern int[MaxBullets] bullet_active;

// Funciones del soldado
void initialize_soldier();
void update_soldier();
void render_soldier();
void soldier_take_damage();
void place_bomb();
void collect_supplies();
void switch_weapon_next();
void switch_weapon_previous();
void give_weapon(int weapon_type);
void enter_vehicle();

#endif