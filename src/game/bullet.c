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
int[MaxBullets] bullet_type;

// En bullet.c
void create_bullet(float x, float y, float angle, float spread, int type)
{
    for (int i = 0; i < MaxBullets; i++)
    {
        if (!bullet_active[i])
        {
            bullet_x[i] = x;
            bullet_y[i] = y;

            if (spread == 0)
            {
                bullet_angle[i] = angle;
            }
            else
            {
                int spread_int = (int)(spread * 1000);
                float random_spread = (rand() % spread_int - spread_int / 2) / 1000.0;
                bullet_angle[i] = angle + random_spread;
            }

            bullet_speed[i] = 5.0;
            bullet_damage[i] = weapon_damage[current_weapon];
            bullet_distance[i] = 0;
            bullet_active[i] = 1;
            bullet_type[i] = type; // Añadir el tipo de bala
            break;
        }
    }
}

void update_bullets()
{
    for (int i = 0; i < MaxBullets; i++)
    {
        if (bullet_active[i])
        {
            float dx = cos(bullet_angle[i]) * bullet_speed[i];
            float dy = sin(bullet_angle[i]) * bullet_speed[i];

            bullet_x[i] += dx;
            bullet_y[i] += dy;
            bullet_distance[i] += sqrt(dx * dx + dy * dy);

            if (bullet_distance[i] > weapon_range[current_weapon])
            {
                bullet_active[i] = 0;
            }
        }
    }
}

void render_bullets()
{
    for (int i = 0; i < MaxBullets; i++)
    {
        if (bullet_active[i])
        {
            if (bullet_type[i] == BulletTypePlayer)
            {
                select_texture(TextureBullet); // Textura de balas del jugador
                select_region(0);
                define_region(0, 0, BulletSize, BulletSize, BulletSize / 2, BulletSize / 2);
            }
            else
            {
                select_texture(TextureEnemyBullet);
                select_region(0);
                define_region(0, 0, BulletSize, BulletSize, BulletSize / 2, BulletSize / 2);
            }

            select_region(0); // Asumiendo que la bala es la región 0
            draw_region_at(bullet_x[i] - camera_x, bullet_y[i] - camera_y);
        }
    }
}