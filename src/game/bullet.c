#include "bullet.h"
#include "weapon.h"
#include "heli.h"
#include "soldier.h"

float[MaxBullets] bullet_x;
float[MaxBullets] bullet_y;
float[MaxBullets] bullet_angle;
float[MaxBullets] bullet_speed;
float[MaxBullets] bullet_damage;
float[MaxBullets] bullet_distance;
int[MaxBullets] bullet_active;
int[MaxBullets] bullet_type;
float spiral_pattern_angle = 0;

// Comprueba colisión entre un punto y un círculo
int check_circle_collision(float px, float py, float cx, float cy, float radius)
{
    float dx = px - cx;
    float dy = py - cy;
    return (dx * dx + dy * dy) <= (radius * radius);
}

void create_spread_pattern(float x, float y, float base_angle, int spread_type)
{
    switch (spread_type)
    {
    case SpreadTypeSingle:
        create_bullet(x, y, base_angle, 0, BulletTypeTurret);
        break;
    case SpreadTypeCircle:
        for (int i = 0; i < 8; i++)
        {
            float angle = base_angle + (2 * pi * i) / 8;
            create_bullet(x, y, angle, 0, BulletTypeTurret);
        }
        break;

    case SpreadTypeCross:
        for (int i = 0; i < 4; i++)
        {
            float angle = base_angle + (pi * i) / 2;
            create_bullet(x, y, angle, 0, BulletTypeTurret);
        }
        break;

    case SpreadTypeWall:
        for (int i = 0; i < 7; i++)
        {
            if (i % 2 == 0)
            { // Deja espacios
                create_bullet(x, y, base_angle, 0, BulletTypeTurret);
            }
            x += 30; // Separación entre balas
        }
        break;

    case SpreadTypeShotgun:
        for (int i = 0; i < 5; i++)
        {
            float spread = pi / 6; // 30 grados totales
            float angle = base_angle - spread / 2 + (spread * i) / 4;
            create_bullet(x, y, angle, 0, BulletTypeTurret);
        }
        break;

    case SpreadTypeSpiral:
        for (int i = 0; i < 3; i++)
        {
            float angle = base_angle + spiral_pattern_angle + (2 * pi * i) / 3;
            create_bullet(x, y, angle, 0, BulletTypeTurret);
        }
        spiral_pattern_angle += pi / 8;
        break;
    }
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
                        heli_x, heli_y,
                        HeliFrameWidth * heli_scale * 0.3)) // Radio más pequeño que el sprite
                {
                    bullet_active[i] = 0;
                    heli_health -= TurretDamagePerBullet;
                    trigger_screen_shake(); 
                    health_flash_timer = HealthFlashTime;
                    if (heli_health <= 0)
                    {
                        if (!has_event_happened(GameOver))
                        {
                            queue_dialog(&DW_GameOver);
                            start_dialog_sequence();

                            mark_event_as_happened(GameOver);
                        }
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
        else if (bullet_type[i] == BulletTypePlayer)
        {
            // Comprobar colisión con enemigos
            for (int e = 0; e < MaxEnemies; e++)
            {
                if (!enemy_active[e] || enemy_health[e] <= 0)
                    continue;

                if (check_circle_collision(
                        bullet_x[i], bullet_y[i],
                        enemy_x[e], enemy_y[e],
                        EnemyHoverFrameWidth * 0.3)) // Radio más pequeño que el sprite
                {
                    bullet_active[i] = 0;
                    damage_enemy(e, bullet_damage[i]);
                    break;
                }
            }
        }
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

            bullet_type[i] = type;

            if (type == BulletTypePlayer)
            {
                if (is_player_in_vehicle)
                {
                    bullet_speed[i] = HeliBulletSpeed;
                    bullet_damage[i] = HeliBulletDamage;
                    weapon_range[current_weapon] = HeliBulletRange;
                }
                else
                {
                    bullet_speed[i] = weapon_speed[current_weapon];
                    bullet_damage[i] = weapon_damage[current_weapon];
                }
            }
            else
            {
                // Comprobar primero si es una torreta
                int is_turret_bullet = 1;
                for (int e = 0; e < MaxEnemies; e++)
                {
                    if (enemy_active[e] &&
                        enemy_x[e] == x &&
                        enemy_y[e] == y)
                    {
                        is_turret_bullet = 0;
                        switch (enemy_type[e])
                        {
                        case EnemyTypeSoldier:
                            bullet_speed[i] = SoldierBulletSpeed;
                            break;
                        case EnemyTypeKamikaze:
                            bullet_speed[i] = KamikazeBulletSpeed;
                            break;
                        case EnemyTypeBoss:
                            bullet_speed[i] = BossBulletSpeed;
                            break;
                        default:
                            bullet_speed[i] = NormalEnemyBulletSpeed;
                            break;
                        }
                        break;
                    }
                }

                // Si no coincide con ningún enemigo, es una bala de torreta
                if (is_turret_bullet)
                {
                    bullet_speed[i] = TurretBulletSpeed;
                }

                bullet_damage[i] = TurretDamagePerBullet;
            }

            bullet_distance[i] = 0;
            bullet_active[i] = 1;
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
            float render_x = bullet_x[i], render_y = bullet_y[i];
            tilemap_draw_region(&world_map, bullet_x[i], bullet_y[i]);
        }
    }
}