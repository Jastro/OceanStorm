// En bullet.h (nuevo archivo)
#ifndef BULLET_H
#define BULLET_H

#define BulletTypePlayer 0
#define BulletTypeTurret 1

extern int[MaxBullets] bullet_type;

void create_bullet(float x, float y, float angle, float spread, int type);
void update_bullets();
void render_bullets();
void create_turret_bullet(float x, float y, float angle);

#endif