// turret.c
#include "turret.h"
#include "../utils/definitions.h"
#include "video.h"
#include "math.h"

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
        float turret_x = island_x[i] + (center_x * TileSize);
        float turret_y = island_y[i] + (center_y * TileSize);
        spawn_turret(turret_x, turret_y);
    }
}

void render_turrets() {
    set_multiply_color(TextColor);
    
    for(int i = 0; i < MaxTurrets; i++) {
        if(!turret_active[i]) continue;
        
        // Dibujar base
        select_texture(TextureTurretBase);
        select_region(RegionTurretBase);
        draw_region_at(turret_x[i] - camera_x, turret_y[i] - camera_y);
        
        // Dibujar cañón
        select_texture(TextureTurretGun);
        select_region(RegionTurretGun);
        set_drawing_angle(turret_angle[i]);
        draw_region_rotated_at(turret_x[i] - camera_x, turret_y[i] - camera_y);
    }
}

void create_turret_bullet(float x, float y, float angle)
{
    for (int i = 0; i < MaxBullets; i++)
    {
        if (!bullet_active[i])
        {
            bullet_x[i] = x;
            bullet_y[i] = y;
            bullet_angle[i] = angle;
            bullet_speed[i] = TurretBulletSpeed;
            bullet_damage[i] = TurretDamage;
            bullet_distance[i] = 0;
            bullet_active[i] = 1;
            bullet_type[i] = BulletTypeTurret;
            break;
        }
    }
}

void spawn_turret(float x, float y)
{
    for (int i = 0; i < MaxTurrets; i++)
    {
        if (!turret_active[i])
        {
            turret_x[i] = x;
            turret_y[i] = y;
            turret_angle[i] = 0;     // Corregido: asignar al elemento del array
            turret_last_shot[i] = 0; // Corregido: asignar al elemento del array
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

        // Solo disparar si el jugador está en el avión
        if (!is_player_in_vehicle)
            continue;

        // Calcular distancia al avión
        float dx = airplane_x - turret_x[i];
        float dy = airplane_y - turret_y[i];
        float distance = sqrt(dx * dx + dy * dy);

        // Comprobar si está en rango
        if (distance <= TurretVisionRange)
        {
            // Rotar hacia el avión
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