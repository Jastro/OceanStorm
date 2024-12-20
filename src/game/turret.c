// turret.c
#include "../utils/definitions.h"
#include "turret.h"
#include "island.h"
#include "heli.h"

float[MaxTurrets] turret_x;
float[MaxTurrets] turret_y;
float[MaxTurrets] turret_angle;
float[MaxTurrets] turret_last_shot;
int[MaxTurrets] turret_active;

void initialize_turrets()
{
    // Configurar sprite de la base
    select_texture(TextureTurretBase);
    select_region(RegionTurretBase);
    define_region(0, 0, TurretBaseWidth, TurretBaseHeight,
                  TurretBaseWidth / 2, TurretBaseHeight / 2);

    // Configurar sprite del cañón
    select_texture(TextureTurretGun);
    select_region(RegionTurretGun);
    define_region(0, 0, TurretGunWidth, TurretGunHeight,
                  TurretGunWidth / 2, TurretGunHeight / 2);

    // Inicializar arrays
    for (int i = 0; i < MaxTurrets; i++)
    {
        turret_active[i] = 0;
    }

    // Generar torretas en las islas
    for (int i = 0; i < num_islands; i++)
    {
        // Colocar una torreta en el centro de cada isla
        int center_x = MaxTilesX / 2;
        int center_y = MaxTilesY / 2;
        float turret_x = island_x[i] + (center_x * TileSize) + TileSize / 2;
        float turret_y = island_y[i] + (center_y * TileSize) + TileSize / 2;
        spawn_turret(turret_x, turret_y);
    }
}

void render_turrets()
{
    set_multiply_color(TextColor);

    for (int i = 0; i < MaxTurrets; i++)
    {
        if (!turret_active[i])
            continue;

        // Dibujar base
        select_texture(TextureTurretBase);
        select_region(RegionTurretBase);
        tilemap_draw_region(&world_map, turret_x[i], turret_y[i]);

        // Dibujar cañón
        select_texture(TextureTurretGun);
        select_region(RegionTurretGun);
        set_drawing_angle(turret_angle[i]);
        tilemap_draw_region_rotated(&world_map, turret_x[i], turret_y[i]);
        
        // Dibujar indicacion de poner bomba si
        // el jugador lo puede hacer en este momento
        if(is_player_in_vehicle || soldier_bombs <= 0)
            continue;
        
        // No indicar si ya hay alguna bomba puesta
        bool there_are_bombs = false;
        
        for(int j = 0; j < MaxActiveBombs; j++)
           if(bomb_active[j])
               there_are_bombs = true;
        
        if(there_are_bombs)
            continue;
        
        float dx = soldier_x - turret_x[i];
        float dy = soldier_y - turret_y[i];
        float distance = sqrt(dx*dx + dy*dy);
        
        // Indicar solo cuando estamos cerca
        if(distance < (2*TurretBaseWidth))
        {
            // Dibujar con parpadeo para que se vea mejor
            if(get_frame_counter() % 40 > 8)
            {
                select_texture(TextureGui);
                select_region(RegionBombSign);
                tilemap_draw_region(&world_map, turret_x[i]-5, turret_y[i]+35);
            }
        }
    }
}

void create_turret_bullet(float x, float y, float angle)
{
    int num_bullets = TurretMinSpread + (rand() % (TurretMaxSpread - TurretMinSpread + 1));
    float spread = pi / 6; // 30 grados totales de spread

    for (int i = 0; i < num_bullets; i++)
    {
        float bullet_angle = angle - spread / 2 + (spread * i) / (num_bullets - 1);
        create_bullet(x, y, bullet_angle, 0, BulletTypeTurret);
    }
}

void spawn_turret(float x, float y)
{
    for (int i = 0; i < MaxTurrets; i++)
    {
        if (!turret_active[i])
        {
            turret_x[i] = x; // Usar los arrays globales
            turret_y[i] = y;
            turret_angle[i] = 0;
            turret_last_shot[i] = 0;
            turret_active[i] = 1;
            break;
        }
    }
}

void update_turrets()
{
    float current_time = get_frame_counter() / 60.0;

    for (int i = 0; i < MaxTurrets; i++)
    {
        if (!turret_active[i])
            continue;

        if (is_player_in_vehicle)
        {
            float dx = heli_x - turret_x[i];
            float dy = heli_y - turret_y[i];
            float distance = sqrt(dx * dx + dy * dy);

            // Comprobar si está en rango
            if (distance <= TurretVisionRange)
            {
                turret_angle[i] = atan2(dy, dx);

                // Disparar si ha pasado suficiente tiempo
                if (current_time - turret_last_shot[i] >= TurretFireRate)
                {
                    create_turret_bullet(turret_x[i], turret_y[i], turret_angle[i]);
                    turret_last_shot[i] = current_time;
                }
            }
        }
    }
}