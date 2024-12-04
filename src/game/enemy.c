#include "enemy.h"
#include "../utils/definitions.h"
#include "video.h"
#include "bullet.h"
#include "turret.h"
#include "math.h"

float[MaxEnemies] enemy_x;
float[MaxEnemies] enemy_y;
float[MaxEnemies] enemy_angle;
float[MaxEnemies] enemy_speed;
float[MaxEnemies] enemy_shoot_timer;
int[MaxEnemies] enemy_spread_type;
int[MaxEnemies] enemy_behavior;
int[MaxEnemies] enemy_type;
int[MaxEnemies] enemy_health;
int[MaxEnemies] enemy_active;
int num_active_enemies;
int phase;

void initialize_enemies()
{
    num_active_enemies = 0;
    phase = 0;

    for (int i = 0; i < MaxEnemies; i++)
    {
        enemy_active[i] = 0;
    }
}

void spawn_boss()
{
    spawn_enemy(WorldWidth / 2, WorldHeight / 2, EnemyTypeAirplane, AIBehaviorShootAndRun, SpreadTypeCircle);
}

void spawn_enemy(float x, float y, int type, int behavior, int spread_type)
{
    for (int i = 0; i < MaxEnemies; i++)
    {
        if (!enemy_active[i])
        {
            enemy_x[i] = x;
            enemy_y[i] = y;
            enemy_angle[i] = 0;
            enemy_type[i] = type;
            enemy_behavior[i] = behavior;
            enemy_spread_type[i] = spread_type;
            enemy_shoot_timer[i] = 0;
            enemy_speed[i] = 2.0;
            enemy_health[i] = 100;
            enemy_active[i] = 1;
            num_active_enemies++;
            break;
        }
    }
}

void update_enemy(int index)
{
    float dx, dy, dist;
    float target_x;
    float target_y;

    if (is_player_in_vehicle)
    {
        target_x = airplane_x;
        target_y = airplane_y;
    }
    else
    {
        target_x = soldier_x;
        target_y = soldier_y;
    }

    switch (enemy_behavior[index])
    {
    case AIBehaviorChase:
        dx = target_x - enemy_x[index];
        dy = target_y - enemy_y[index];
        enemy_angle[index] = atan2(dy, dx);
        enemy_x[index] += cos(enemy_angle[index]) * enemy_speed[index];
        enemy_y[index] += sin(enemy_angle[index]) * enemy_speed[index];
        break;

    case AIBehaviorShootAndRun:
        dx = target_x - enemy_x[index];
        dy = target_y - enemy_y[index];
        dist = sqrt(dx * dx + dy * dy);

        if (dist < 200)
        {
            enemy_x[index] -= dx * enemy_speed[index] / dist;
            enemy_y[index] -= dy * enemy_speed[index] / dist;
        }
        enemy_angle[index] = atan2(dy, dx);
        break;

    case AIBehaviorKamikaze:
        dx = target_x - enemy_x[index];
        dy = target_y - enemy_y[index];
        enemy_angle[index] = atan2(dy, dx);
        enemy_x[index] += cos(enemy_angle[index]) * enemy_speed[index] * 1.5;
        enemy_y[index] += sin(enemy_angle[index]) * enemy_speed[index] * 1.5;
        break;

    case AIBehaviorBomber:
        dx = target_x - enemy_x[index];
        dy = target_y - enemy_y[index];
        dist = sqrt(dx * dx + dy * dy);

        if (dist > 300)
        {
            enemy_x[index] += cos(enemy_angle[index]) * enemy_speed[index];
            enemy_y[index] += sin(enemy_angle[index]) * enemy_speed[index];
        }
        if (enemy_shoot_timer[index] <= 0)
        {
            create_bullet(enemy_x[index], enemy_y[index], pi / 2, 0, BulletTypeTurret);
            enemy_shoot_timer[index] = 2.0;
        }
        break;
    }

    // Disparar según el patrón
    if (enemy_behavior[index] != AIBehaviorKamikaze &&
        enemy_shoot_timer[index] <= 0)
    {
        create_spread_pattern(enemy_x[index], enemy_y[index],
                              enemy_angle[index], enemy_spread_type[index]);
        enemy_shoot_timer[index] = 1.0;
    }

    enemy_shoot_timer[index] -= 1.0 / 60.0;
}

void update_enemies()
{
    for (int i = 0; i < MaxEnemies; i++)
    {
        if (enemy_active[i])
        {
            update_enemy(i);
        }
    }
    check_phase_progress();
}

void render_enemies()
{
    for (int i = 0; i < MaxEnemies; i++)
    {
        if (!enemy_active[i])
            continue;

        select_texture(TextureEnemy);

        if (enemy_health[i] <= 0)
        {
            // Frame de destrucción
            select_region(2);
            // Escalar más pequeño a medida que cae
            float scale = enemy_shoot_timer[i]; // Usamos el timer para la animación de caída
            set_drawing_scale(scale, scale);
            if (scale <= 0.1)
            {
                enemy_active[i] = 0;
            }
        }
        else
        {
            // Alternar entre frame 0 y 1 para animación de vuelo
            select_region((get_frame_counter() / 10) % 2);
        }

        select_region(0);
        define_region(
            0,
            0,
            EnemyHoverFrameWidth,
            EnemyHoverFrameHeight,
            EnemyHoverFrameWidth / 2,
            EnemyHoverFrameHeight / 2);

        select_region(1);
        define_region(
            EnemyHoverFrameWidth,
            0,
            EnemyHoverFrameWidth * 2,
            EnemyHoverFrameHeight,
            (EnemyHoverFrameWidth / 2) + EnemyHoverFrameWidth,
            EnemyHoverFrameHeight / 2);

        set_drawing_angle(enemy_angle[i] + 80);
        draw_region_rotozoomed_at(enemy_x[i] - camera_x, enemy_y[i] - camera_y);
    }
}

void damage_enemy(int index, int damage)
{
    enemy_health[index] -= damage;
    if (enemy_health[index] <= 0)
    {
        enemy_active[index] = 0;
        num_active_enemies--;
    }
}

void spawn_wave_of_enemies()
{
    spawn_enemy(WorldWidth / 2, WorldHeight / 2, EnemyTypeAirplane, AIBehaviorShootAndRun, SpreadTypeCircle);
    //spawn_enemy(200, 100, EnemyTypeAirplane, AIBehaviorKamikaze, SpreadTypeNormal);
    //spawn_enemy(300, 100, EnemyTypeAirplane, AIBehaviorBomber, SpreadTypeCross);
}

void check_phase_progress()
{
    switch (phase)
    {
    case 0: // Fase inicial
        if (num_active_turrets() >= MaxTurrets / 2)
        {
            spawn_wave_of_enemies();
            phase = 1;
        }
        break;

    case 1: // Fase de enemigos
        if (num_active_enemies == 0)
        {
            phase = 2;
        }
        break;

    case 2: // Fase final de torretas
        if (num_active_turrets() == 0)
        {
            spawn_boss();
            phase = 3;
        }
        break;
    }
}

// También necesitamos esta función para contar torretas activas
int num_active_turrets()
{
    int count = 0;
    for (int i = 0; i < MaxTurrets; i++)
    {
        if (turret_active[i])
            count++;
    }
    return count;
}