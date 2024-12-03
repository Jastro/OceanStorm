#include "bullet.h"
#include "weapon.h"
#include "airplane.h"
#include "soldier.h"
#include "math.h"
#include "video.h"
#include "misc.h"

float[MaxBullets] bullet_x;
float[MaxBullets] bullet_y;
float[MaxBullets] bullet_angle;
float[MaxBullets] bullet_speed;
float[MaxBullets] bullet_damage;
float[MaxBullets] bullet_distance;
int[MaxBullets] bullet_active;
int[MaxBullets] bullet_type;

// Comprueba colisión entre un punto y un círculo
int check_circle_collision(float px, float py, float cx, float cy, float radius)
{
    float dx = px - cx;
    float dy = py - cy;
    return (dx * dx + dy * dy) <= (radius * radius);
}

void check_bullet_collisions()
{
    for (int i = 0; i < MaxBullets; i++)
    {
        if (!bullet_active[i])
            continue;

        // Si es bala de torreta, comprobar colisión con jugador
        if (bullet_type[i] == BulletTypeTurret)
        {
            // Colisión con avión
            if (is_player_in_vehicle)
            {
                if (check_circle_collision(
                        bullet_x[i], bullet_y[i],
                        airplane_x, airplane_y,
                        AirplaneFrameWidth * airplane_scale * 0.3)) // Radio más pequeño que el sprite
                {
                    bullet_active[i] = 0;
                    airplane_health -= TurretDamagePerBullet;
                    health_flash_timer = HealthFlashTime;
                    if (airplane_health <= 0)
                    {
                        game_state = StateGameOver;
                    }
                    continue;
                }
            }
            // Colisión con soldado
            else if (soldier_state != SoldierStateNone &&
                     soldier_state != SoldierStateImmune)
            {
                if (check_circle_collision(
                        bullet_x[i], bullet_y[i],
                        soldier_x, soldier_y,
                        SoldierWidth * 0.3)) // Radio más pequeño que el sprite
                {
                    bullet_active[i] = 0;
                    soldier_take_damage();
                    continue;
                }
            }
        }
        // Las balas del jugador ya no necesitan comprobar colisiones con torretas
    }
}

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
            bullet_type[i] = type;
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

    check_bullet_collisions();
}

void render_bullets()
{
    for (int i = 0; i < MaxBullets; i++)
    {
        if (bullet_active[i])
        {
            if (bullet_type[i] == BulletTypePlayer)
            {
                select_texture(TextureBullet);
                select_region(0);
                define_region(0, 0, BulletSize, BulletSize, BulletSize / 2, BulletSize / 2);
            }
            else
            {
                select_texture(TextureEnemyBullet);
                select_region(0);
                define_region(0, 0, BulletSize, BulletSize, BulletSize / 2, BulletSize / 2);
            }

            select_region(0);
            draw_region_at(bullet_x[i] - camera_x, bullet_y[i] - camera_y);
        }
    }
}