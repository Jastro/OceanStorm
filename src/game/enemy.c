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
int[MaxEnemies] enemy_is_reloading;
float[MaxEnemies] enemy_reload_start;

int num_active_enemies;
int phase;
float phase_time = 0;

int check_enemy_collision(float x, float y, int current_enemy)
{
    for (int i = 0; i < MaxEnemies; i++)
    {
        if (!enemy_active[i] || i == current_enemy)
            continue;

        float dx = x - enemy_x[i];
        float dy = y - enemy_y[i];
        float dist = sqrt(dx * dx + dy * dy);

        if (dist < EnemyCollisionRadius)
            return 1;
    }

    // También comprobar colisión con el jugador
    if (!is_player_in_vehicle)
    {
        float dx = x - soldier_x;
        float dy = y - soldier_y;
        float dist = sqrt(dx * dx + dy * dy);
        if (dist < EnemyCollisionRadius)
            return 1;
    }
    else
    {
        float dx = x - heli_x;
        float dy = y - heli_y;
        float dist = sqrt(dx * dx + dy * dy);
        if (dist < EnemyCollisionRadius)
            return 1;
    }

    return 0;
}

void initialize_enemies()
{
    select_texture(TextureEnemySoldier);
    select_region(RegionSoldier);
    define_region(0, 0, SoldierWidth, SoldierHeight, SoldierWidth / 2, SoldierHeight / 2);
    num_active_enemies = 0;
    phase = 0;

    for (int frame = 0; frame < 3; frame++)
    {
        select_texture(TextureEnemyKamikaze);
        select_region(frame);
        define_region(
            frame * EnemyKamikazeFrameWidth,
            0,
            (frame + 1) * EnemyKamikazeFrameWidth,
            EnemyKamikazeFrameHeight,
            (EnemyKamikazeFrameWidth / 2) + (frame * EnemyKamikazeFrameWidth),
            EnemyKamikazeFrameHeight / 2);

        select_texture(TextureEnemyBoss);
        select_region(frame);
        define_region(
            frame * EnemyBossFrameWidth,
            0,
            (frame + 1) * EnemyBossFrameWidth,
            EnemyBossFrameHeight,
            (EnemyBossFrameWidth / 2) + (frame * EnemyBossFrameWidth),
            EnemyBossFrameHeight / 2);

        select_texture(TextureEnemy);
        select_region(frame);
        define_region(
            frame * EnemyHoverFrameWidth,
            0,
            (frame + 1) * EnemyHoverFrameWidth,
            EnemyHoverFrameHeight,
            (EnemyHoverFrameWidth / 2) + (frame * EnemyHoverFrameWidth),
            EnemyHoverFrameHeight / 2);
    }

    for (int i = 0; i < MaxEnemies; i++)
    {
        enemy_active[i] = 0;
        enemy_blink_timer[i] = 0;
        enemy_is_reloading[i] = 0;
        enemy_reload_start[i] = 0;
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
            enemy_speed[i] = 1.5;
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
            case EnemyTypeSoldier:
                enemy_speed[i] = SoldierEnemySpeed;
                enemy_health[i] = SoldierEnemyHealth;
                break;
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
    float ideal_distance = 300;    // Era 200, ahora el doble
    float separation_margin = 100; // Margen de separación más amplio (era 50)
    float new_x, new_y;
    float offset_angle;

    if (enemy_health[index] <= 0)
    {
        enemy_shoot_timer[index] -= 1.0 / 60.0; // Reducir la escala gradualmente
        // Simular caída añadiendo movimiento hacia abajo
        enemy_y[index] += 2.0; // Velocidad de caída
        return;
    }

    if (enemy_is_reloading[index])
    {
        // Mostrar texto de recarga
        select_texture(-1);
        set_multiply_color(RedColor);
        tilemap_print(
            &world_map,
            enemy_x[index] - 30,
            enemy_y[index] - ReloadTextOffset,
            "RECARGANDO");

        // Si ha pasado el tiempo, completar recarga
        float current_time = get_frame_counter() / 60.0;
        if (current_time - enemy_reload_start[index] >= ReloadTime)
        {
            enemy_is_reloading[index] = 0;
            enemy_shoot_timer[index] = 0;
        }
        return;
    }

    // Determinar objetivo basado en tipo de enemigo
    if (enemy_type[index] == EnemyTypeSoldier)
    {
        // Los soldados enemigos solo atacan cuando el jugador está como soldado
        if (is_player_in_vehicle)
            return;

        target_x = soldier_x;
        target_y = soldier_y;
    }
    else
    {
        // El resto de enemigos solo atacan al avión
        if (!is_player_in_vehicle)
            return;

        target_x = heli_x;
        target_y = heli_y;
    }

    // Calcular distancia al objetivo
    dx = target_x - enemy_x[index];
    dy = target_y - enemy_y[index];
    dist = sqrt(dx * dx + dy * dy);
    enemy_angle[index] = atan2(dy, dx);

    switch (enemy_behavior[index])
    {
    case AIBehaviorChase:
        if (enemy_type[index] == EnemyTypeSoldier)
        {
            // Calcular nueva posición potencial
            float new_x = enemy_x[index] + cos(enemy_angle[index]) * enemy_speed[index];
            float new_y = enemy_y[index] + sin(enemy_angle[index]) * enemy_speed[index];

            // Solo moverse si no hay colisión
            if (!check_enemy_collision(new_x, new_y, index))
            {
                enemy_x[index] = new_x;
                enemy_y[index] = new_y;
            }
            else
            {
                // Si hay colisión, intentar moverse en un ángulo ligeramente diferente
                float offset_angle = enemy_angle[index] + (pi / 4);
                new_x = enemy_x[index] + cos(offset_angle) * enemy_speed[index];
                new_y = enemy_y[index] + sin(offset_angle) * enemy_speed[index];

                if (!check_enemy_collision(new_x, new_y, index))
                {
                    enemy_x[index] = new_x;
                    enemy_y[index] = new_y;
                }
            }

            // Disparar si está lo suficientemente cerca
            if (dist < SoldierEnemyRange && enemy_shoot_timer[index] <= 0)
            {
                switch (enemy_spread_type[index])
                {
                case SpreadTypeSingle:
                    create_bullet(enemy_x[index], enemy_y[index], enemy_angle[index], 0, BulletTypeTurret);
                    enemy_shoot_timer[index] = 1.0;
                    break;

                default:
                    create_spread_pattern(enemy_x[index], enemy_y[index],
                                          enemy_angle[index], enemy_spread_type[index]);
                    enemy_shoot_timer[index] = 1.0;
                    break;
                }
            }
        }
        else // Enemigos voladores
        {
            enemy_x[index] += cos(enemy_angle[index]) * enemy_speed[index];
            enemy_y[index] += sin(enemy_angle[index]) * enemy_speed[index];

            // Disparar cada cierto tiempo
            if (enemy_shoot_timer[index] <= 0)
            {
                create_spread_pattern(enemy_x[index], enemy_y[index],
                                      enemy_angle[index], enemy_spread_type[index]);
                enemy_shoot_timer[index] = 1.0;
            }
        }
        break;

    case AIBehaviorShootAndRun:
        if (dist < ideal_distance)
        {
            // Alejarse más rápido cuanto más cerca esté
            float speed_multiplier = (ideal_distance - dist) / 200; // Factor de escala
            if (speed_multiplier > 2.0)
                speed_multiplier = 2.0; // Limitar el multiplicador
            enemy_x[index] -= cos(enemy_angle[index]) * enemy_speed[index] * speed_multiplier;
            enemy_y[index] -= sin(enemy_angle[index]) * enemy_speed[index] * speed_multiplier;
        }
        else if (dist > ideal_distance + separation_margin)
        {
            // Acercarse muy lentamente cuando está lejos
            enemy_x[index] += cos(enemy_angle[index]) * enemy_speed[index] * 0.5;
            enemy_y[index] += sin(enemy_angle[index]) * enemy_speed[index] * 0.5;
        }

        // Solo disparar si está dentro del rango ideal ± margen
        if (enemy_shoot_timer[index] <= 0 &&
            dist > (ideal_distance - separation_margin) &&
            dist < (ideal_distance + separation_margin))
        {
            create_spread_pattern(enemy_x[index], enemy_y[index],
                                  enemy_angle[index], enemy_spread_type[index]);
            enemy_shoot_timer[index] = 1.0;
        }
        break;

    case AIBehaviorKamikaze:
        // Moverse directamente hacia el objetivo
        enemy_x[index] += cos(enemy_angle[index]) * enemy_speed[index] * 1.5;
        enemy_y[index] += sin(enemy_angle[index]) * enemy_speed[index] * 1.5;

        // Comprobar colisión con el objetivo
        if (dist < 50)
        {
            if (is_player_in_vehicle)
            {
                heli_health -= KamikazeDamage;
                health_flash_timer = HealthFlashTime;
                if (heli_health <= 0)
                {
                    game_state = StateGameOver;
                }
            }
            // Destruir el kamikaze
            enemy_health[index] = 0;
            enemy_shoot_timer[index] = 1.0;
        }
        break;
    }

    enemy_shoot_timer[index] -= 1.0 / 60.0;
}

void update_enemies()
{
    // Incrementar el tiempo de fase una vez por frame
    phase_time += 1.0 / 60.0;

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
        switch (enemy_type[i])
        {
        case EnemyTypeSoldier:
            select_texture(TextureEnemySoldier);
            select_region(RegionSoldier);
            break;
        case EnemyTypeKamikaze:
            select_texture(TextureEnemyKamikaze);
            break;
        case EnemyTypeBoss:
            select_texture(TextureEnemyBoss);
            break;
        default:
            select_texture(TextureEnemy);
            break;
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
            set_drawing_scale(1, 1);
            frame = (get_frame_counter() / 10) % 2; // Alternar entre 0 y 1
        }

        // Elegir región según frame (excepto para soldados)
        if (enemy_type[i] != EnemyTypeSoldier)
            select_region(frame);

        // Color normal o rojo si está recibiendo daño
        if (enemy_blink_timer[i] > 0)
        {
            set_multiply_color(RedColor);
        }
        else
        {
            set_multiply_color(TextColor);
        }

        // Renderizar según tipo
        if (enemy_type[i] == EnemyTypeSoldier)
        {
            set_drawing_angle(enemy_angle[i]);
            tilemap_draw_region_rotated(&world_map, enemy_x[i], enemy_y[i]);
        }
        else
        {
            set_drawing_angle(enemy_angle[i] + 80);
            tilemap_draw_region_rotozoomed(&world_map, enemy_x[i], enemy_y[i]);
        }

        // Mostrar texto de recarga si está recargando
        if (enemy_active[i] && enemy_health[i] > 0 && enemy_is_reloading[i])
        {
            select_texture(-1);
            set_multiply_color(RedColor);
            tilemap_print(
                &world_map,
                enemy_x[i] - 30,
                enemy_y[i] - ReloadTextOffset,
                "RELOADING");
        }
    }
}

void damage_enemy(int index, int damage)
{
    if (enemy_health[index] <= 0)
        return;

    enemy_health[index] -= damage;
    enemy_blink_timer[index] = EnemyBlinkTime;

    if (enemy_health[index] <= 0)
    {
        if (enemy_type[index] == EnemyTypeSoldier)
        {
            spawn_corpse(enemy_x[index], enemy_y[index]);
            spawn_random_pickup(enemy_x[index], enemy_y[index]);
            enemy_active[index] = 0;
        }
        else
        {
            // num_active_enemies--;
            enemy_shoot_timer[index] = 1.0; // Usamos este timer para la escala
        }
    }
}

void spawn_wave_of_enemies()
{
    // spawn_boss();
    spawn_enemy(100, 100, EnemyTypeNormal, AIBehaviorShootAndRun, SpreadTypeShotgun);
    spawn_enemy(300, 100, EnemyTypeKamikaze, AIBehaviorKamikaze, SpreadTypeNormal);
    spawn_enemy(550, 100, EnemyTypeKamikaze, AIBehaviorChase, SpreadTypeCross);
}

void check_phase_progress()
{
    switch (phase)
    {
    case 0: // Fase inicial
        // if (num_active_turrets() <= MaxTurrets / 2)
        if (num_active_turrets() <= 9)
        {
            spawn_wave_of_enemies();
            phase = 1;
            phase_time = 0;
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