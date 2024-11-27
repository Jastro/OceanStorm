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
            
            if(spread == 0) {
                bullet_angle[i] = angle;
            } else {
                int spread_int = (int)(spread * 1000);
                float random_spread = (rand() % spread_int - spread_int/2) / 1000.0;
                bullet_angle[i] = angle + random_spread;
            }
            
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

void render_bullets() {
    select_texture(-1);  // Usar textura de la BIOS
    select_region(0);    // Región cuadrada
    set_multiply_color(color_yellow);  // Color amarillo para las balas
    
    for(int i = 0; i < MaxBullets; i++) {
        if(bullet_active[i]) {
            // Dibujar cada bala como un punto pequeño
            float screen_x = bullet_x[i] - camera_x;
            float screen_y = bullet_y[i] - camera_y;
            
            // Solo dibujar si está en pantalla
            if(screen_x >= 0 && screen_x < screen_width &&
               screen_y >= 0 && screen_y < screen_height) {
                draw_region_at(screen_x, screen_y);
            }
        }
    }
}