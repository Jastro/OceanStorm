#ifndef ENEMY_H
#define ENEMY_H

extern float[MaxEnemies] enemy_x;
extern float[MaxEnemies] enemy_y;
extern float[MaxEnemies] enemy_angle;
extern float[MaxEnemies] enemy_speed;
extern float[MaxEnemies] enemy_shoot_timer;
extern float[MaxEnemies] enemy_blink_timer;
extern int[MaxEnemies] enemy_spread_type;
extern int[MaxEnemies] enemy_behavior;
extern int[MaxEnemies] enemy_type;
extern int[MaxEnemies] enemy_health;
extern int[MaxEnemies] enemy_active;
extern int[MaxEnemies] enemy_pattern_index;
extern int[MaxEnemies] enemy_pattern_count;
extern int[MaxEnemies] enemy_is_reloading;
extern float[MaxEnemies] enemy_reload_start;
extern float[MaxEnemies] enemy_spawn_animation_timer;

extern int num_active_enemies;
extern int num_total_enemies;
extern int phase;
extern float phase_time;

// Funciones de enemigos
void initialize_enemies();
void spawn_enemy(float x, float y, int type, int behavior, int spread_type);
void update_enemy(int index);
void update_enemies();
void render_enemies();
void damage_enemy(int index, int damage);
void spawn_wave_of_enemies();
void check_phase_progress();
int num_active_turrets();
int get_current_phase();
void spawn_boss();

#endif