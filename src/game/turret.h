// turret.h
#ifndef TURRET_H
#define TURRET_H

// Estado de las torretas
extern float[MaxTurrets] turret_x;
extern float[MaxTurrets] turret_y;
extern float[MaxTurrets] turret_angle;
extern float[MaxTurrets] turret_last_shot;
extern int[MaxTurrets] turret_active;

void initialize_turrets();
void reset_turrets();
void spawn_turret(float x, float y);
void update_turrets();
void render_turrets();

#endif