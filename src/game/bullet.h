#ifndef BULLET_H
#define BULLET_H

#define BulletTypePlayer 0
#define BulletTypeTurret 1

extern float spiral_pattern_angle;
extern float[MaxBullets] bullet_x;
extern float[MaxBullets] bullet_y;
extern float[MaxBullets] bullet_angle;
extern float[MaxBullets] bullet_speed;
extern float[MaxBullets] bullet_damage;
extern float[MaxBullets] bullet_distance;
extern float[MaxBullets] bullet_range;
extern int[MaxBullets] bullet_active;
extern int[MaxBullets] bullet_type;

void create_bullet(float x, float y, float angle, float spread, int type);
void initialize_bullets();
void reset_bullets();
void update_bullets();
void render_bullets();
void create_turret_bullet(float x, float y, float angle);
void create_spread_pattern(float x, float y, float base_angle, int spread_type);

#endif