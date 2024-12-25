#include "enemy.h"
#include "../utils/definitions.h"
#include "video.h"
#include "bullet.h"
#include "turret.h"
#include "math.h"

int[MaxEnemies] enemy_phase;              // Para saber en qué fase está
float[MaxEnemies] phase_health_threshold; // Para saber cuándo cambiar de fase
int[MaxEnemies] enemy_state;
float[MaxEnemies] enemy_spawn_timer;
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
float[MaxEnemies] enemy_spawn_animation_timer;
int last_frame_hit_sound = 0;

int num_active_enemies;
int num_total_enemies;
int phase;
float phase_time = 0;
float boss_center_x = 0;
float boss_center_y = 0;

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

int get_current_phase()
{
    return phase;
}

void initialize_enemies()
{
    select_texture(TextureEnemySoldier);
    select_region(RegionSoldier);
    define_region(0, 0, SoldierWidth, SoldierHeight, SoldierWidth / 2, SoldierHeight / 2);

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

        select_texture(TextureEnemyHelicopter);
        select_region(frame);
        define_region(
            frame * EnemyHelicopterFrameWidth,
            0,
            (frame + 1) * EnemyHelicopterFrameWidth,
            EnemyHelicopterFrameHeight,
            (EnemyHelicopterFrameWidth / 2) + (frame * EnemyHelicopterFrameWidth),
            EnemyHelicopterFrameHeight / 2);
    }
}

void reset_enemies()
{
    num_active_enemies = 0;
    num_total_enemies = 0;
    phase = 0;

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
    /*
    float center_x = WorldWidth;
    float center_y = WorldHeight;
    */
    float center_x = WorldWidth - 200;
    float center_y = WorldHeight / 2;

    for (int i = 0; i < MaxEnemies; i++)
    {
        if (!enemy_active[i])
        {
            // Configuración básica
            enemy_active[i] = 1;
            enemy_health[i] = BossHealth1;
            enemy_x[i] = boss_center_x;
            enemy_y[i] = boss_center_y;
            enemy_type[i] = EnemyTypeBoss;
            // Configuración inicial (Fase 1)
            enemy_phase[i] = BossPhaseOne;
            enemy_behavior[i] = AIBehaviorShootAndRun;
            enemy_spread_type[i] = SpreadTypeCircle;
            enemy_speed[i] = BossSpeed1;
            phase_health_threshold[i] = BossHealth1;

            // Calculamos los umbrales de salud para cambios de fase
            float health_phase2 = BossHealth1 * 0.66; // A 66% de vida cambia a fase 2
            float health_phase3 = BossHealth1 * 0.33; // A 33% de vida cambia a fase 3

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
            // Configuración básica igual que antes
            enemy_x[i] = x;
            enemy_y[i] = y;
            enemy_angle[i] = 0;
            enemy_type[i] = type;
            enemy_behavior[i] = behavior;
            enemy_spread_type[i] = spread_type;
            enemy_shoot_timer[i] = 0;
            enemy_blink_timer[i] = 0;

            switch (type)
            {
            case EnemyTypeSoldier:
                enemy_speed[i] = SoldierEnemySpeed;
                enemy_health[i] = SoldierEnemyHealth;
                // Añadir inicialización del estado de spawn
                enemy_state[i] = SoldierStateSpawning;
                enemy_spawn_animation_timer[i] = SpawnAnimationTime;
                break;

            case EnemyTypeNormal:
                enemy_speed[i] = EnemySpeedNormal;
                enemy_health[i] = EnemyHealthNormal;
                enemy_state[i] = SoldierStateActive;
                break;

            case EnemyTypeKamikaze:
                enemy_speed[i] = EnemySpeedKamikaze;
                enemy_health[i] = EnemyHealthKamikaze;
                enemy_state[i] = SoldierStateActive;
                break;

            case EnemyTypePlane:
                enemy_speed[i] = EnemySpeedPlane;
                enemy_health[i] = EnemyHealthPlane;
                enemy_state[i] = SoldierStateActive;
                break;

            case EnemyTypeBoss:
                enemy_speed[i] = EnemySpeedBoss;
                enemy_health[i] = EnemyHealthBoss;
                enemy_state[i] = SoldierStateActive;
                break;
            }

            enemy_active[i] = 1;
            num_active_enemies++;
            break;
        }
    }
}

void update_boss(int index)
{
    float dx = heli_x - enemy_x[index];
    float dy = heli_y - enemy_y[index];
    float dist = sqrt(dx * dx + dy * dy);
    enemy_angle[index] = atan2(dy, dx);

    // Variables que necesitamos para los patrones de movimiento
    float shoot_rate;
    float orbit_angle;
    float orbit_radius = 200;
    float side_movement;
    float perpendicular_x;
    float perpendicular_y;

    // Comprobar cambios de fase basados en salud
    if (enemy_phase[index] == BossPhaseOne && enemy_health[index] <= BossHealth1 * 0.66)
    {
        enemy_phase[index] = BossPhaseTwo;
        enemy_behavior[index] = AIBehaviorChase;
        enemy_spread_type[index] = SpreadTypeWall;
        enemy_speed[index] = BossSpeed2;
    }
    else if (enemy_phase[index] == BossPhaseTwo && enemy_health[index] <= BossHealth1 * 0.33)
    {
        enemy_phase[index] = BossPhaseThree;
        enemy_behavior[index] = AIBehaviorKamikaze;
        enemy_spread_type[index] = SpreadTypeSpiral;
        enemy_speed[index] = BossSpeed3;
    }

    // Comportamiento específico según la fase
    switch (enemy_phase[index])
    {
    case BossPhaseOne:
        shoot_rate = 1.5;
        // Fase 1: Si está lejos, acercarse al jugador, cuando está cerca orbitar
        if (dist > 300) // Si está lejos
        {
            // Moverse hacia el jugador
            enemy_x[index] += cos(enemy_angle[index]) * enemy_speed[index];
            enemy_y[index] += sin(enemy_angle[index]) * enemy_speed[index];
        }
        else // Si está lo suficientemente cerca, orbitar
        {
            orbit_angle = get_frame_counter() * 0.02;
            float orbit_radius = 200;
            enemy_x[index] = heli_x + cos(orbit_angle) * orbit_radius;
            enemy_y[index] = heli_y + sin(orbit_angle) * orbit_radius;
        }
        enemy_angle[index] = atan2(dy, dx); // Siempre mirar al jugador
        break;

    case BossPhaseTwo:
        shoot_rate = 1.0;
        // Fase 2: Perseguir agresivamente al jugador
        enemy_x[index] += cos(enemy_angle[index]) * enemy_speed[index] * 1.5;
        enemy_y[index] += sin(enemy_angle[index]) * enemy_speed[index] * 1.5;
        break;

    case BossPhaseThree:
        shoot_rate = 0.5;
        // Fase 3: Mantener distancia y zigzag
        if (dist < 300) // Si está demasiado cerca, alejarse
        {
            enemy_x[index] -= cos(enemy_angle[index]) * enemy_speed[index];
            enemy_y[index] -= sin(enemy_angle[index]) * enemy_speed[index];
        }
        else if (dist > 300) // Si está muy lejos, acercarse un poco
        {
            enemy_x[index] += cos(enemy_angle[index]) * enemy_speed[index] * 0.5;
            enemy_y[index] += sin(enemy_angle[index]) * enemy_speed[index] * 0.5;
        }
        else // En rango óptimo, hacer zigzag lateral
        {
            orbit_angle = get_frame_counter() * 0.1;
            float side_movement = cos(orbit_angle) * 100;
            float perpendicular_x = -dy / dist; // Vector perpendicular al jugador
            float perpendicular_y = dx / dist;
            enemy_x[index] += perpendicular_x * side_movement;
            enemy_y[index] += perpendicular_y * side_movement;
        }
        break;
    }

    // Control de disparos
    if (enemy_shoot_timer[index] <= 0)
    {
        create_spread_pattern(enemy_x[index], enemy_y[index],
                              enemy_angle[index], enemy_spread_type[index]);
        enemy_shoot_timer[index] = shoot_rate;
    }

    enemy_shoot_timer[index] -= 1.0 / 60.0;
}

void update_enemy(int index)
{
    float dx, dy, dist;
    float target_x;
    float target_y;
    float ideal_distance = 100;    // Era 200, ahora el doble
    float separation_margin = 100; // Margen de separación más amplio (era 50)
    float new_x, new_y;
    float offset_angle;

    if (enemy_state[index] == SoldierStateSpawning)
    {
        enemy_spawn_animation_timer[index] -= 1.0 / 60.0;

        // Cuando termina la animación, cambiar a estado normal
        if (enemy_spawn_animation_timer[index] <= 0)
        {
            enemy_state[index] = SoldierStateActive;
        }
        return; // No procesar nada más mientras está spawning
    }

    if (enemy_type[index] == EnemyTypeBoss)
    {
        update_boss(index);
        return;
    }

    if (enemy_health[index] <= 0)
    {
        enemy_shoot_timer[index] -= 1.0 / 60.0;
        enemy_y[index] += 2.0;
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
        {
            // Comportamiento de patrulla cuando el jugador está a pie
            float patrol_time = get_frame_counter() * 0.005; // Reducido para movimiento más lento
            float patrol_radius = 150.0;                     // Radio más pequeño

            // Offset individual para cada enemigo
            float individual_offset = index * (2 * pi / 3);
            float target_angle = patrol_time + individual_offset;

            // Calcular punto objetivo del patrullaje
            float target_patrol_x = enemy_x[index] + cos(target_angle) * patrol_radius;
            float target_patrol_y = enemy_y[index] + sin(target_angle) * patrol_radius;

            // Mantener dentro de los límites del mundo
            target_patrol_x = clamp(target_patrol_x, patrol_radius, WorldWidth - patrol_radius);
            target_patrol_y = clamp(target_patrol_y, patrol_radius, WorldHeight - patrol_radius);

            // Calcular dirección hacia el punto objetivo
            float dx = target_patrol_x - enemy_x[index];
            float dy = target_patrol_y - enemy_y[index];
            float patrol_angle = atan2(dy, dx);

            // Mover suavemente hacia el punto usando la velocidad normal del enemigo
            enemy_x[index] += cos(patrol_angle) * enemy_speed[index] * 0.5; // Velocidad reducida
            enemy_y[index] += sin(patrol_angle) * enemy_speed[index] * 0.5;

            // Actualizar el ángulo para que mire hacia donde va
            enemy_angle[index] = patrol_angle;

            return;
        }

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
                    create_spread_pattern(enemy_x[index], enemy_y[index],
                                          enemy_angle[index], SpreadTypeSingle);
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
            float minimum_distance = 70.0;
            if (dist < minimum_distance)
            {
                enemy_x[index] -= cos(enemy_angle[index]) * enemy_speed[index];
                enemy_y[index] -= sin(enemy_angle[index]) * enemy_speed[index];
            }
            else if (dist > minimum_distance * 1.5) // Un poco más lejos para evitar zigzag
            {
                enemy_x[index] += cos(enemy_angle[index]) * enemy_speed[index];
                enemy_y[index] += sin(enemy_angle[index]) * enemy_speed[index];
            }

            // Disparar cada cierto tiempo (mantenemos esta parte igual)
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
                trigger_screen_shake();
                if (heli_health <= 0)
                {
                    kill_player();
                }
            }
            // Destruir el kamikaze
            damage_enemy(index, enemy_health[index]);
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

        if (enemy_type[i] == EnemyTypeSoldier)
        {
            // Si está spawning, aplicar efecto visual
            if (enemy_state[i] == SoldierStateSpawning)
            {
                // Hacer que parpadee
                if ((get_frame_counter() / SpawnBlinkRate) % 2 == 0)
                {
                    set_multiply_color(0x80FFFFFF); // Semi-transparente
                }
                else
                {
                    continue; // No dibujar en frames alternos
                }
            }
        }

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
            select_texture(TextureEnemy);
            break;
        case EnemyTypePlane:
            select_texture(TextureEnemyKamikaze);
            break;
        default:
            select_texture(TextureEnemyHelicopter);
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
                // Explosión si cae en tierra, splash en agua
                if (is_over_ocean(enemy_x[i], enemy_y[i]))
                    spawn_fx(enemy_x[i], enemy_y[i], Splash);
                else
                    spawn_fx(enemy_x[i], enemy_y[i], Explosion);

                enemy_active[i] = 0;
                num_active_enemies--;
                continue;
            }
        }
        else
        {
            if (enemy_type[i] == EnemyTypeKamikaze)
            {
                set_drawing_scale(0.6, 0.6);
            }
            else
            {
                set_drawing_scale(1.0, 1.0);
            }
            frame = (get_frame_counter() / 10) % 2; // Alternar entre 0 y 1
        }

        // Elegir región según frame (excepto para soldados)
        if (enemy_type[i] != EnemyTypeSoldier)
            select_region(frame);

        if (enemy_blink_timer[i] > 0)
        {
            set_multiply_color(RedColor);
        }
        else if (enemy_type[i] == EnemyTypeKamikaze)
        {
            // Dibujar los kamikazes de otro color
            set_multiply_color(0xFFFF40FF);
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

        // Mostrar texto de recarga
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
    if (enemy_state[index] == SoldierStateSpawning)
        return;

    if (enemy_health[index] <= 0)
        return;

    enemy_health[index] -= damage;
    enemy_blink_timer[index] = EnemyBlinkTime;

    // Reproducir sonido una única vez por frame,
    // para no saturar con armas como la escopeta
    if (last_frame_hit_sound != get_frame_counter())
    {
        play_sound(SoundEnemyHit);
        last_frame_hit_sound = get_frame_counter();
    }

    if (enemy_health[index] <= 0)
    {
        if (enemy_type[index] == EnemyTypeSoldier)
        {
            spawn_corpse(enemy_x[index], enemy_y[index]);
            spawn_random_pickup(enemy_x[index], enemy_y[index]);
            enemy_active[index] = 0;
            num_active_enemies--;
        }
        else
        {
            // al morir los aviones normales explotan y caen
            if (enemy_type[index] != EnemyTypeBoss)
            {
                spawn_fx(enemy_x[index], enemy_y[index], Explosion);
                play_sound(SoundFall);
            }

            enemy_shoot_timer[index] = 1.0; // Usamos este timer para la escala
        }
    }
}

void spawn_wave_of_enemies()
{
    // spawn_boss();
    spawn_enemy(WorldHeight - 100, WorldWidth - 100, EnemyTypeNormal, AIBehaviorShootAndRun, SpreadTypeCircle);
    spawn_enemy(WorldWidth, WorldHeight / 2, EnemyTypeKamikaze, AIBehaviorKamikaze, SpreadTypeNormal);
    spawn_enemy(550, 640, EnemyTypePlane, AIBehaviorChase, SpreadTypeCross);
}

void check_phase_progress()
{
    switch (phase)
    {
    case 0: // Fase inicial
        // if (num_active_turrets() <= MaxTurrets / 2)
        if (num_active_turrets() <= 4)
        {
            if (!has_event_happened(SpawnFlyingEnemies))
            {
                queue_dialog(&DW_EnemyFighters);
                queue_dialog(&DW_EnemyFightersReply);
                queue_dialog(&DW_AttackHeli);
                start_dialog_sequence();

                mark_event_as_happened(SpawnFlyingEnemies);
            }

            spawn_wave_of_enemies();
            begin_planes();
            phase = 1;
            phase_time = 0;
        }
        break;

    case 1: // Fase de enemigos
        if (num_active_enemies == 0)
        {
            if (!has_event_happened(FlyingEnemiesDestroyed))
            {
                queue_dialog(&DW_GoHome);
                queue_dialog(&DW_TurretsRemain);
                queue_dialog(&DW_TurretsRemainReply);
                start_dialog_sequence();

                mark_event_as_happened(FlyingEnemiesDestroyed);
            }
            phase = 2;
            begin_game();
        }
        break;

    case 2: // Fase final de torretas
        if (num_active_turrets() == 0)
        {
            if (!has_event_happened(SpawnBoss))
            {
                queue_dialog(&DW_MaxAlert);
                queue_dialog(&DW_MaxAlertReply);
                queue_dialog(&DW_DarkPhoenix);
                queue_dialog(&DW_DarkPhoenix_Reply);
                start_dialog_sequence();

                mark_event_as_happened(SpawnBoss);
            }
            spawn_boss();
            begin_boss();
            phase = 3;
        }
        break;
    case 3:
        if (num_active_enemies == 0)
        {
            phase = 4;
        }
        break;
    case 4:
        begin_ending();
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