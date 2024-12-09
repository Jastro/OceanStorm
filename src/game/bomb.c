#include "bomb.h"
#include "video.h"
#include "turret.h"
#include "time.h"
#include "string.h"

// Definición de los arrays globales
float[MaxActiveBombs] bomb_x;
float[MaxActiveBombs] bomb_y;
float[MaxActiveBombs] bomb_timer;
int[MaxActiveBombs] bomb_active;
void spawn_soldier_enemies(float x, float y, int count)
{
    for (int i = 0; i < count; i++)
    {
        float angle = (rand() % 360) * pi / 180.0;
        float distance = 50 + (rand() % SoldierSpawnRadius);
        float spawn_x = x + cos(angle) * distance;
        float spawn_y = y + sin(angle) * distance;

        // Elegir patrón de disparo aleatorio
        int random_type = rand() % 100; // Número entre 0 y 99
        int spread_type;

        if (random_type < 40)
        {                                   // 40% probabilidad
            spread_type = SpreadTypeSingle; // Soldado con pistola
        }
        else if (random_type < 70)
        {                                    // 30% probabilidad
            spread_type = SpreadTypeShotgun; // Soldado con escopeta
        }
        else if (random_type < 90)
        {                                 // 20% probabilidad
            spread_type = SpreadTypeWall; // Soldado con patrón en Wall
        }
        else
        {                                   // 10% probabilidad
            spread_type = SpreadTypeCircle; // Soldado con disparo circular
        }

        spawn_enemy(spawn_x, spawn_y, EnemyTypeSoldier, AIBehaviorChase, spread_type);
    }
}

void initialize_bombs()
{
    for (int i = 0; i < MaxActiveBombs; i++)
    {
        bomb_active[i] = 0;
    }
}

int can_plant_bomb(float x, float y)
{
    for (int i = 0; i < MaxTurrets; i++)
    {
        if (!turret_active[i])
            continue;

        float dx = x - turret_x[i];
        float dy = y - turret_y[i];
        float distance = sqrt(dx * dx + dy * dy);

        if (distance < TurretBaseWidth)
        {
            return 1;
        }
    }
    return 0;
}

void plant_bomb(float x, float y)
{
    for (int i = 0; i < MaxActiveBombs; i++)
    {
        if (!bomb_active[i])
        {
            bomb_x[i] = x;
            bomb_y[i] = y;
            bomb_timer[i] = BombTimerSeconds;
            bomb_active[i] = 1;
            return;
        }
    }
}

void check_bomb_explosions()
{
    for (int b = 0; b < MaxActiveBombs; b++)
    {
        if (!bomb_active[b])
            continue;

        if (bomb_timer[b] <= 0)
        {
            for (int t = 0; t < MaxTurrets; t++)
            {
                if (!turret_active[t])
                    continue;

                float dx = bomb_x[b] - turret_x[t];
                float dy = bomb_y[b] - turret_y[t];
                float distance = sqrt(dx * dx + dy * dy);

                if (distance <= BombExplosionRadius)
                {
                    // Generar soldados antes de desactivar la torreta
                    int soldier_count = MinSoldiersPerTurret +
                                        (rand() % (MaxSoldiersPerTurret - MinSoldiersPerTurret + 1));
                    spawn_soldier_enemies(turret_x[t], turret_y[t], soldier_count);

                    turret_active[t] = 0;
                    spawn_pickup(turret_x[t], turret_y[t], PickupWeapon, WeaponTypeShotgun);
                }
            }
            bomb_active[b] = 0;
        }
    }
}

void update_bombs()
{
    float delta_time = 1.0 / 60.0;

    for (int i = 0; i < MaxActiveBombs; i++)
    {
        if (!bomb_active[i])
            continue;

        bomb_timer[i] -= delta_time;

        if (bomb_timer[i] <= 0)
        {
            if (!has_event_happened(TurretDestroyed))
            {
                queue_dialog(DT_TurretDestroyed, RegionPortraitPlayer);
                queue_dialog(DT_EnemyDetected, RegionPortraitCommander);
                queue_dialog(DT_EnemyPenetration, RegionPortraitSoldier);
                start_dialog_sequence();

                mark_event_as_happened(TurretDestroyed);
            }
            check_bomb_explosions();
        }
    }
}

void render_bombs()
{
    select_texture(-1);

    for (int i = 0; i < MaxActiveBombs; i++)
    {
        if (!bomb_active[i])
            continue;

        // Dibujar la bomba
        set_multiply_color(RedColor);
        draw_region_at(bomb_x[i] - camera_x, bomb_y[i] - camera_y);

        // Mostrar el temporizador
        set_multiply_color(RedColor);
        int[8] timer_text;
        itoa((int)bomb_timer[i] + 1, timer_text, 10);
        print_at(
            (int)(bomb_x[i] - camera_x) - 5,
            (int)(bomb_y[i] - camera_y) - 20,
            timer_text);
    }
}