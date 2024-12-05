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
float[MaxEnemies] enemy_blink_timer;
int[MaxEnemies] enemy_spread_type;
int[MaxEnemies] enemy_behavior;
int[MaxEnemies] enemy_type;
int[MaxEnemies] enemy_health;
int[MaxEnemies] enemy_active;
int[MaxEnemies] enemy_pattern_index; // Para controlar el patrón actual
int[MaxEnemies] enemy_pattern_count;

int num_active_enemies;
int phase;

void initialize_enemies()
{
    num_active_enemies = 0;
    phase = 0;

    for (int i = 0; i < MaxEnemies; i++)
    {
        enemy_active[i] = 0;
        enemy_blink_timer[i] = 0;
    }
}

void spawn_boss()
{
    float center_x = WorldWidth;
    float center_y = WorldHeight;

    for (int i = 0; i < MaxEnemies; i++)
    {
        if (!enemy_active[i])
        {
            enemy_pattern_index[i] = 0;
            enemy_pattern_count[i] = 0;
            enemy_x[i] = center_x;
            enemy_y[i] = center_y;
            enemy_type[i] = EnemyTypeBoss;
            enemy_behavior[i] = AIBehaviorShootAndRun;
            enemy_spread_type[i] = SpreadTypeWall;
            enemy_speed[i] = 2.0;
            enemy_health[i] = EnemyHealthBoss;
            enemy_active[i] = 1;
            enemy_blink_timer[i] = 0;
            num_active_enemies++;
            break;
        }
    }
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
            enemy_blink_timer[i] = 0;

            // Asignar velocidad y vida según tipo
            switch (type)
            {
            case EnemyTypeNormal:
                enemy_speed[i] = EnemySpeedNormal;
                enemy_health[i] = EnemyHealthNormal;
                break;

            case EnemyTypeKamikaze:
                enemy_speed[i] = EnemySpeedKamikaze;
                enemy_health[i] = EnemyHealthKamikaze;
                break;

            case EnemyTypeBoss:
                enemy_speed[i] = EnemySpeedBoss;
                enemy_health[i] = EnemyHealthBoss;
                break;
            }

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
        if (is_player_in_vehicle)
        { // Solo persiguen si el jugador está en el helicóptero
            dx = target_x - enemy_x[index];
            dy = target_y - enemy_y[index];
            enemy_angle[index] = atan2(dy, dx);
            enemy_x[index] += cos(enemy_angle[index]) * enemy_speed[index];
            enemy_y[index] += sin(enemy_angle[index]) * enemy_speed[index];
        }
        break;

    case AIBehaviorShootAndRun:
        dx = target_x - enemy_x[index];
        dy = target_y - enemy_y[index];
        dist = sqrt(dx * dx + dy * dy);
        enemy_angle[index] = atan2(dy, dx);

        if (enemy_type[index] == EnemyTypeBoss)
        {
            // El boss siempre actúa independientemente del vehículo
            // Fase 1: Perseguir y disparar desde los lados
            if (enemy_health[index] > BossPhase1Health)
            {
                // Mantener distancia ideal
                if (dist != 225)
                {
                    float direction;
                    if (dist < 225)
                    {
                        direction = -1;
                    }
                    else
                    {
                        direction = 1;
                    }
                    enemy_x[index] += direction * cos(enemy_angle[index]) * enemy_speed[index];
                    enemy_y[index] += direction * sin(enemy_angle[index]) * enemy_speed[index];
                }

                // Sistema de disparo rotativo
                if (enemy_shoot_timer[index] <= 0)
                {
                    switch (enemy_pattern_index[index])
                    {
                    case 0: // Metralleta
                        create_bullet(enemy_x[index], enemy_y[index], enemy_angle[index], 0, BulletTypeTurret);
                        enemy_shoot_timer[index] = 0.1;
                        break;

                    case 1: // Cruz
                        for (int i = 0; i < 4; i++)
                        {
                            float angle = enemy_angle[index] + (pi * i) / 2;
                            create_bullet(enemy_x[index], enemy_y[index], angle, 0, BulletTypeTurret);
                        }
                        enemy_shoot_timer[index] = 0.5;
                        break;

                    case 2: // Muro
                        for (int i = 0; i < 5; i++)
                        {
                            if (i % 2 == 0)
                            {
                                create_bullet(enemy_x[index] + (i * 30), enemy_y[index], enemy_angle[index], 0, BulletTypeTurret);
                            }
                        }
                        enemy_shoot_timer[index] = 0.7;
                        break;
                    }

                    // Contar disparos y cambiar patrón
                    enemy_pattern_count[index]++;
                    if (enemy_pattern_count[index] >= 10)
                    {
                        enemy_pattern_index[index] = (enemy_pattern_index[index] + 1) % 3;
                        enemy_pattern_count[index] = 0;
                    }
                }
            }
            // Fase 2: Persecución agresiva con disparos de escopeta
            else if (enemy_health[index] > BossPhase2Health)
            {
                enemy_x[index] += cos(enemy_angle[index]) * enemy_speed[index] * 1.5;
                enemy_y[index] += sin(enemy_angle[index]) * enemy_speed[index] * 1.5;

                if (enemy_shoot_timer[index] <= 0)
                {
                    // Disparo tipo escopeta
                    float spread = pi / 6; // 30 grados
                    for (int i = 0; i < 5; i++)
                    {
                        float angle = enemy_angle[index] - spread / 2 + (spread * i) / 4;
                        create_bullet(enemy_x[index], enemy_y[index], angle, 0, BulletTypeTurret);
                    }
                    enemy_shoot_timer[index] = BossPhase2FireRate;
                }
            }
            // Fase 3: Huir y lluvia de balas
            else
            {
                // Invertir el ángulo para huir
                enemy_x[index] -= cos(enemy_angle[index]) * enemy_speed[index] * 0.8;
                enemy_y[index] -= sin(enemy_angle[index]) * enemy_speed[index] * 0.8;

                if (enemy_shoot_timer[index] <= 0)
                {
                    // Lluvia de balas en patrón circular
                    float angle_step = (2 * pi) / 16; // 16 balas en círculo
                    for (int i = 0; i < 16; i++)
                    {
                        float angle = enemy_angle[index] + i * angle_step;
                        create_bullet(enemy_x[index], enemy_y[index], angle, 10, BulletTypeTurret);
                    }
                    enemy_shoot_timer[index] = BossPhase3FireRate;
                }
            }
        }
        else
        {
            // Enemigos normales solo actúan si el jugador está en el helicóptero
            if (is_player_in_vehicle)
            {
                enemy_x[index] -= dx * enemy_speed[index] / dist;
                enemy_y[index] -= dy * enemy_speed[index] / dist;

                // Disparar según el patrón
                if (enemy_shoot_timer[index] <= 0)
                {
                    create_spread_pattern(enemy_x[index], enemy_y[index],
                                          enemy_angle[index], enemy_spread_type[index]);
                    enemy_shoot_timer[index] = 1.0;
                }
            }
        }

        enemy_shoot_timer[index] -= 1.0 / 60.0;
        break;

    case AIBehaviorKamikaze:
        if (is_player_in_vehicle)
        { // Solo actúa si el jugador está en el helicóptero
            if (enemy_health[index] > 0)
            {
                // Comportamiento normal cuando está vivo
                dx = target_x - enemy_x[index];
                dy = target_y - enemy_y[index];
                enemy_angle[index] = atan2(dy, dx);
                enemy_x[index] += cos(enemy_angle[index]) * enemy_speed[index] * 1.5;
                enemy_y[index] += sin(enemy_angle[index]) * enemy_speed[index] * 1.5;

                // Comprobar colisión si está vivo
                dist = sqrt(dx * dx + dy * dy);
                if (dist < 50)
                {
                    if (is_player_in_vehicle)
                    {
                        airplane_health -= KamikazeDamage;
                        health_flash_timer = HealthFlashTime;
                        if (airplane_health <= 0)
                        {
                            game_state = StateGameOver;
                        }
                    }
                    else
                    {
                        soldier_take_damage();
                    }
                    // Destruir el kamikaze
                    enemy_health[index] = 0;
                    enemy_shoot_timer[index] = 1.0; // Iniciar animación de caída
                }
            }
        }
        // Cuando está destruido, solo cae
        break;

    case AIBehaviorBomber:
        if (is_player_in_vehicle)
        { // Solo actúa si el jugador está en el helicóptero
            dx = target_x - enemy_x[index];
            dy = target_y - enemy_y[index];
            dist = sqrt(dx * dx + dy * dy);

            // Quitada la restricción de distancia
            enemy_x[index] += cos(enemy_angle[index]) * enemy_speed[index];
            enemy_y[index] += sin(enemy_angle[index]) * enemy_speed[index];

            if (enemy_shoot_timer[index] <= 0)
            {
                create_bullet(enemy_x[index], enemy_y[index], pi / 2, 0, BulletTypeTurret);
                enemy_shoot_timer[index] = 2.0;
            }
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

            // Actualizar timer de parpadeo
            if (enemy_blink_timer[i] > 0)
            {
                enemy_blink_timer[i] -= 1.0 / 60.0;
            }
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

        // Seleccionar textura según tipo
        if (enemy_type[i] == EnemyTypeKamikaze)
        {
            select_texture(TextureEnemyKamikaze);
        }
        else if (enemy_type[i] == EnemyTypeBoss)
        {
            select_texture(TextureEnemyBoss);
        }
        else
        {
            select_texture(TextureEnemy);
        }

        // Determinar el frame a usar
        int frame;
        if (enemy_health[i] <= 0)
        {
            frame = 2; // Frame de destrucción
            float scale = enemy_shoot_timer[i];
            set_drawing_scale(scale, scale);
            if (scale <= 0.1)
            {
                enemy_active[i] = 0;
                num_active_enemies--;
                continue;
            }
        }
        else
        {
            frame = (get_frame_counter() / 10) % 2; // Alternar entre 0 y 1
        }

        if (enemy_type[i] == EnemyTypeKamikaze)
        {
            select_region(frame);
            define_region(
                frame * EnemyKamikazeFrameWidth,
                0,
                (frame + 1) * EnemyKamikazeFrameWidth,
                EnemyKamikazeFrameHeight,
                (EnemyKamikazeFrameWidth / 2) + (frame * EnemyKamikazeFrameWidth),
                EnemyKamikazeFrameHeight / 2);
        }
        else if (enemy_type[i] == EnemyTypeBoss)
        {
            select_region(frame);
            define_region(
                frame * EnemyBossFrameWidth,
                0,
                (frame + 1) * EnemyBossFrameWidth,
                EnemyBossFrameHeight,
                (EnemyBossFrameWidth / 2) + (frame * EnemyBossFrameWidth),
                EnemyBossFrameHeight / 2);
        }
        else
        {
            select_region(frame);
            define_region(
                frame * EnemyHoverFrameWidth,
                0,
                (frame + 1) * EnemyHoverFrameWidth,
                EnemyHoverFrameHeight,
                (EnemyHoverFrameWidth / 2) + (frame * EnemyHoverFrameWidth),
                EnemyHoverFrameHeight / 2);
        }

        // Color normal o rojo si está recibiendo daño
        if (enemy_blink_timer[i] > 0)
        {
            set_multiply_color(RedColor);
        }
        else
        {
            set_multiply_color(TextColor);
        }

        set_drawing_angle(enemy_angle[i] + 80);
        draw_region_rotozoomed_at(enemy_x[i] - camera_x, enemy_y[i] - camera_y);
    }
}

void damage_enemy(int index, int damage)
{
    enemy_health[index] -= damage;
    enemy_blink_timer[index] = EnemyBlinkTime; // Iniciar parpadeo

    if (enemy_health[index] <= 0)
    {
        enemy_shoot_timer[index] = 1.0; // Para animación de caída
    }
}

void spawn_wave_of_enemies()
{
    spawn_boss();
    // spawn_enemy(WorldWidth / 2, WorldHeight / 2, EnemyTypeKamikaze, AIBehaviorChase, SpreadTypeShotgun);
    //  spawn_enemy(200, 100, EnemyTypeKamikaze, AIBehaviorKamikaze, SpreadTypeNormal);
    //  spawn_enemy(300, 100, EnemyTypeNormal, AIBehaviorBomber, SpreadTypeCross);
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