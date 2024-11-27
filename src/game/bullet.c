// En bullet.c (nuevo archivo)
#include "bullet.h"
#include "weapon.h"
// resto de includes necesarios

float[MaxBullets] bullet_x;
float[MaxBullets] bullet_y; 
float[MaxBullets] bullet_angle;
float[MaxBullets] bullet_speed;
float[MaxBullets] bullet_damage;
float[MaxBullets] bullet_distance;
int[MaxBullets] bullet_active;

void create_bullet(float x, float y, float angle, float spread) {
    for(int i = 0; i < MaxBullets; i++) {
        if(!bullet_active[i]) {
            bullet_x[i] = x;
            bullet_y[i] = y;
            bullet_angle[i] = angle + (rand() % (int)(spread * 2) - spread);
            bullet_speed[i] = 5.0;
            bullet_damage[i] = weapon_damage[current_weapon];
            bullet_distance[i] = 0;
            bullet_active[i] = 1;
            break;
        }
    }
}

void update_bullets() {
    for(int i = 0; i < MaxBullets; i++) {
        if(bullet_active[i]) {
            float dx = cos(bullet_angle[i]) * bullet_speed[i];
            float dy = sin(bullet_angle[i]) * bullet_speed[i];
            
            bullet_x[i] += dx;
            bullet_y[i] += dy;
            bullet_distance[i] += sqrt(dx*dx + dy*dy);
            
            if(bullet_distance[i] > weapon_range[current_weapon]) {
                bullet_active[i] = 0;
            }
        }
    }
}