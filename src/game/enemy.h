#ifndef ENEMY_H
#define ENEMY_H

extern float[MaxEnemies] enemy_x;
extern float[MaxEnemies] enemy_y;
extern float[MaxEnemies] enemy_angle;
extern float[MaxEnemies] enemy_speed;
extern float[MaxEnemies] enemy_shoot_timer;
extern float[MaxEnemies] enemy_blink_timer;  // Añadido
extern int[MaxEnemies] enemy_spread_type;
extern int[MaxEnemies] enemy_behavior;
extern int[MaxEnemies] enemy_type;
extern int[MaxEnemies] enemy_health;
extern int[MaxEnemies] enemy_active;
extern int num_active_enemies;
extern int phase;

void initialize_enemies();
void spawn_enemy(float x, float y, int type, int behavior, int spread_type);
void update_enemy(int index);  // Añadido
void update_enemies();
void render_enemies();
void damage_enemy(int index, int damage);
void spawn_wave_of_enemies();
void check_phase_progress();
int num_active_turrets();
void spawn_boss();

#endif