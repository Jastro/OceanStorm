// ship.c
#include "ship.h"
#include "../utils/definitions.h"
#include "island.h"
#include "video.h"
#include "math.h"

// Variables globales
float[MaxShips] ship_x;
float[MaxShips] ship_y;
float[MaxShips] ship_angle;
float[MaxShips] ship_target_angle;
int[MaxShips] ship_health;
int[MaxShips] ship_active;
float[MaxShips] ship_blink_timer;
int[MaxShips] ship_frame;

float[MaxShips] front_turret_angle;
float[MaxShips] back_turret_angle;
float[MaxShips] rocket_turret_angle;
float[MaxShips] turret_fire_timer;
float[MaxShips] missile_fire_timer;

float[MaxShips][MaxMissiles] missile_x;
float[MaxShips][MaxMissiles] missile_y;
float[MaxShips][MaxMissiles] missile_angle;
float[MaxShips][MaxMissiles] missile_lifetime;
int[MaxShips][MaxMissiles] missile_active;

extern float heli_x;
extern float heli_y;
extern int is_player_in_vehicle;
extern float camera_x;
extern float camera_y;

void get_turret_position(int ship_index, float offset_x, float offset_y, float *out_x, float *out_y)
{
    float cos_angle = cos(ship_angle[ship_index]);
    float sin_angle = sin(ship_angle[ship_index]);

    *out_x = ship_x[ship_index] + (offset_x * cos_angle - offset_y * sin_angle);
    *out_y = ship_y[ship_index] + (offset_x * sin_angle + offset_y * cos_angle);
}

void update_ship_weapons(int index)
{
    if (!ship_active[index] || ship_health[index] <= 0)
        return;

    float dx = heli_x - ship_x[index];
    float dy = heli_y - ship_y[index];
    float distance = sqrt(dx * dx + dy * dy);

    if (distance <= ShipTurretRange && is_player_in_vehicle)
    {
        // Calcular ángulos deseados para todas las torretas
        float target_angle = atan2(dy, dx);

        // Actualizar ángulos gradualmente
        front_turret_angle[index] = get_target_angle(front_turret_angle[index], target_angle);
        back_turret_angle[index] = get_target_angle(back_turret_angle[index], target_angle);
        rocket_turret_angle[index] = get_target_angle(rocket_turret_angle[index], target_angle);

        // Actualizar timers
        turret_fire_timer[index] -= 1.0 / 60.0;
        missile_fire_timer[index] -= 1.0 / 60.0;

        // Disparar balas si es tiempo
        if (turret_fire_timer[index] <= 0)
        {
            float front_x, front_y, back_x, back_y;

            // Disparar desde torreta frontal
            get_turret_position(index, FrontTurretOffsetX, FrontTurretOffsetY, &front_x, &front_y);
            create_bullet(front_x, front_y, front_turret_angle[index], 0, BulletTypeTurret);

            // Disparar desde torreta trasera
            get_turret_position(index, BackTurretOffsetX, BackTurretOffsetY, &back_x, &back_y);
            create_bullet(back_x, back_y, back_turret_angle[index], 0, BulletTypeTurret);

            turret_fire_timer[index] = ShipTurretFireRate;
        }

        // Disparar misil si es tiempo
        if (missile_fire_timer[index] <= 0)
        {
            float rocket_x, rocket_y;
            get_turret_position(index, RocketTurretOffsetX, RocketTurretOffsetY, &rocket_x, &rocket_y);

            // Buscar slot libre para misil
            for (int m = 0; m < MaxMissiles; m++)
            {
                if (!missile_active[index][m])
                {
                    missile_x[index][m] = rocket_x;
                    missile_y[index][m] = rocket_y;
                    missile_angle[index][m] = rocket_turret_angle[index];
                    missile_lifetime[index][m] = MissileLifetime;
                    missile_active[index][m] = 1;
                    break;
                }
            }

            missile_fire_timer[index] = ShipMissileFireRate;
        }
    }
}

void update_missiles()
{
    float missile_rotation_speed = ShipRotationSpeed * 2; // Hacer que gire más rápido que el barco

    for (int i = 0; i < MaxShips; i++)
    {
        for (int m = 0; m < MaxMissiles; m++)
        {
            if (!missile_active[i][m])
                continue;

            missile_lifetime[i][m] -= 1.0 / 60.0;
            if (missile_lifetime[i][m] <= 0)
            {
                missile_active[i][m] = 0;
                continue;
            }

            // Calcular dirección al jugador
            float dx = heli_x - missile_x[i][m];
            float dy = heli_y - missile_y[i][m];
            float target_angle = atan2(dy, dx);

            // Rotar más suavemente
            float angle_diff = target_angle - missile_angle[i][m];

            // Normalizar la diferencia de ángulo
            while (angle_diff > pi)
                angle_diff -= 2 * pi;
            while (angle_diff < -pi)
                angle_diff += 2 * pi;

            // Aplicar rotación
            if (angle_diff > 0)
            {
                missile_angle[i][m] += missile_rotation_speed;
            }
            else if (angle_diff < 0)
            {
                missile_angle[i][m] -= missile_rotation_speed;
            }

            // Mover en la dirección actual
            missile_x[i][m] += cos(missile_angle[i][m]) * ShipMissileSpeed;
            missile_y[i][m] += sin(missile_angle[i][m]) * ShipMissileSpeed;

            // Comprobar colisión
            if (is_player_in_vehicle)
            {
                dx = missile_x[i][m] - heli_x;
                dy = missile_y[i][m] - heli_y;
                if (sqrt(dx * dx + dy * dy) < HeliFrameWidth * heli_scale * 0.3)
                {
                    missile_active[i][m] = 0;
                    heli_health -= ShipMissileDamage;
                    health_flash_timer = HealthFlashTime;
                }
            }
        }
    }
}

float get_target_angle(float current_angle, float desired_angle)
{
    float diff = desired_angle - current_angle;

    // Normalizar la diferencia al rango [-pi, pi]
    while (diff > pi)
        diff -= 2 * pi;
    while (diff < -pi)
        diff += 2 * pi;

    // Aplicar rotación suave
    if (diff > 0)
    {
        return current_angle + ShipRotationSpeed;
    }
    if (diff < 0)
    {
        return current_angle - ShipRotationSpeed;
    }
    return current_angle;
}

void spawn_wave_of_ships(int count)
{
    // Limitar el número de barcos al máximo permitido
    if (count > MaxShips)
        count = MaxShips;

    // Calcular posiciones iniciales distribuidas en la parte superior
    float start_x = WorldWidth / (count + 1);

    for (int i = 0; i < count; i++)
    {
        // Intentar hacer spawn con separación
        float spawn_x = start_x * (i + 1);
        float spawn_y = WorldHeight - LargeShipHeight; // Justo fuera de la pantalla

        // Verificar que no hay colisión en el punto de spawn
        if (!check_ship_collision(spawn_x, spawn_y, LargeShipWidth))
        {
            spawn_ship(spawn_x, spawn_y);
        }
    }
}

int check_ship_collision(float x, float y, float radius)
{
    // Comprobar colisión con carrier
    float dx = x - StartingX;
    float dy = y - StartingY;
    float carrier_dist = sqrt(dx * dx + dy * dy);
    if (carrier_dist < CarrierWidth + radius)
        return 1;

    // Comprobar colisión con islas
    for (int i = 0; i < num_islands; i++)
    {
        float island_center_x = island_x[i] + (MaxTilesX * TileSize / 2);
        float island_center_y = island_y[i] + (MaxTilesY * TileSize / 2);
        dx = x - island_center_x;
        dy = y - island_center_y;
        float island_dist = sqrt(dx * dx + dy * dy);
        if (island_dist < island_radius[i] + radius)
            return 1;
    }

    // Comprobar colisión con otros barcos
    for (int i = 0; i < MaxShips; i++)
    {
        if (!ship_active[i] || ship_health[i] <= 0)
            continue;

        dx = x - ship_x[i];
        dy = y - ship_y[i];
        float ship_dist = sqrt(dx * dx + dy * dy);

        if (ship_dist < (LargeShipWidth + radius))
            return 1;
    }

    return 0;
}

void initialize_ships()
{
    // Inicializar todos los barcos como inactivos primero
    for (int i = 0; i < MaxShips; i++)
    {
        ship_active[i] = 0;
        ship_blink_timer[i] = 0;
        ship_frame[i] = 0;

        // Inicializar ángulos de torretas y timers
        front_turret_angle[i] = 0;
        back_turret_angle[i] = 0;
        rocket_turret_angle[i] = 0;
        turret_fire_timer[i] = 0;
        missile_fire_timer[i] = 0;

        // Inicializar misiles
        for (int m = 0; m < MaxMissiles; m++)
        {
            missile_active[i][m] = 0;
        }
    }

    // Hacer spawn de la primera ola
    spawn_wave_of_ships(3); // O el número que prefieras
}

void spawn_ship(float x, float y)
{
    for (int i = 0; i < MaxShips; i++)
    {
        if (!ship_active[i])
        {
            ship_x[i] = x;
            ship_y[i] = y;
            ship_angle[i] = pi / 2;
            ship_target_angle[i] = ship_angle[i];
            ship_health[i] = ShipHealth;
            ship_active[i] = 1;
            ship_blink_timer[i] = 0;
            ship_frame[i] = 0;

            // Añadir inicialización de torretas
            front_turret_angle[i] = ship_angle[i];
            back_turret_angle[i] = ship_angle[i];
            rocket_turret_angle[i] = ship_angle[i];
            turret_fire_timer[i] = 0;
            missile_fire_timer[i] = 0;

            for (int m = 0; m < MaxMissiles; m++)
            {
                missile_active[i][m] = 0;
            }
            break;
        }
    }
}

void damage_ship(int index, int damage)
{
    if (ship_health[index] <= 0)
        return;

    ship_health[index] -= damage;
    ship_blink_timer[index] = 0.5;

    if (ship_health[index] <= 0)
    {
        ship_frame[index] = 1; // Cambiar al frame de destrucción
    }
}

void update_ships() {
   for(int i = 0; i < MaxShips; i++) {
       if(!ship_active[i] || ship_health[i] <= 0) continue;

       if(ship_blink_timer[i] > 0) {
           ship_blink_timer[i] -= 1.0/60.0;
       }

       float dx = heli_x - ship_x[i];
       float dy = heli_y - ship_y[i];
       float distance = sqrt(dx*dx + dy*dy);

       // Determinar el comportamiento
       float desired_angle;

       // Siempre comprobar obstáculos primero, independientemente del modo
       float look_ahead_x = ship_x[i] + cos(ship_angle[i]) * LargeShipWidth * 2;
       float look_ahead_y = ship_y[i] + sin(ship_angle[i]) * LargeShipWidth * 2;

       if(check_ship_collision(look_ahead_x, look_ahead_y, LargeShipWidth)) {
           // Si hay obstáculo, girar para evitarlo
           if(rand() % 2 == 0) {
               desired_angle = ship_angle[i] + pi/4;
           } else {
               desired_angle = ship_angle[i] - pi/4;
           }
       } else if(distance < ShipDetectionRange && is_player_in_vehicle) {
           // Si no hay obstáculo y el jugador está en rango, perseguirlo
           desired_angle = atan2(dy, dx);
       } else {
           // Si no hay obstáculo ni jugador, mantener rumbo
           desired_angle = ship_angle[i];
       }

       // Rotar suavemente
       ship_angle[i] = get_target_angle(ship_angle[i], desired_angle);

       // Mover siempre en la dirección actual
       float new_x = ship_x[i] + cos(ship_angle[i]) * ShipBaseSpeed;
       float new_y = ship_y[i] + sin(ship_angle[i]) * ShipBaseSpeed;

       // Teletransportar al otro lado del mapa
       if(new_y < -LargeShipHeight) {
           new_y = WorldHeight;
       } else if(new_y > WorldHeight) {
           new_y = -LargeShipHeight;
       }

       if(new_x < -LargeShipWidth) {
           new_x = WorldWidth;
       } else if(new_x > WorldWidth) {
           new_x = -LargeShipWidth;
       }

       ship_x[i] = new_x;
       ship_y[i] = new_y;

       // Actualizar armas
       update_ship_weapons(i);
   }

   // Actualizar misiles una sola vez fuera del bucle
   update_missiles();
}

void render_ships()
{
    for (int i = 0; i < MaxShips; i++)
    {
        if (!ship_active[i])
            continue;

        // Color según estado de parpadeo
        if (ship_blink_timer[i] > 0 && ((int)(ship_blink_timer[i] * 60) % 2))
        {
            set_multiply_color(RedColor);
        }
        else
        {
            set_multiply_color(TextColor);
        }

        // Dibujar barco
        select_texture(TextureLargeShip);
        select_region(ship_frame[i]);
        define_region(
            ship_frame[i] * LargeShipWidth,
            0,
            (ship_frame[i] + 1) * LargeShipWidth,
            LargeShipHeight,
            LargeShipWidth / 2,
            LargeShipHeight / 2);

        set_drawing_angle(ship_angle[i] - pi / 2);
        draw_region_rotated_at(
            ship_x[i] - camera_x,
            ship_y[i] - camera_y);

        // Renderizar torretas solo si el barco no está destruido
        if (ship_health[i] > 0)
        {
            float turret_x, turret_y;
            set_multiply_color(TextColor);

            // Torreta frontal
            get_turret_position(i, FrontTurretOffsetX, FrontTurretOffsetY, &turret_x, &turret_y);
            select_texture(TextureShipTurret);
            select_region(0);
            define_region(0, 0, ShipTurretWidth, ShipTurretHeight,
                          ShipTurretWidth / 2, ShipTurretHeight / 2);
            set_drawing_angle(front_turret_angle[i]);
            draw_region_rotated_at(turret_x - camera_x, turret_y - camera_y);

            // Torreta trasera
            get_turret_position(i, BackTurretOffsetX, BackTurretOffsetY, &turret_x, &turret_y);
            select_texture(TextureShipBackTurret);
            select_region(0);
            define_region(0, 0, ShipTurretWidth, ShipTurretHeight,
                          ShipTurretWidth / 2, ShipTurretHeight / 2);
            set_drawing_angle(back_turret_angle[i]);
            draw_region_rotated_at(turret_x - camera_x, turret_y - camera_y);

            // Lanzamisiles
            get_turret_position(i, RocketTurretOffsetX, RocketTurretOffsetY, &turret_x, &turret_y);
            select_texture(TextureShipRocketTurret);
            select_region(0);
            define_region(0, 0, ShipRocketTurretWidth, ShipRocketTurretHeight,
                          ShipRocketTurretWidth / 2, ShipRocketTurretHeight / 2);
            set_drawing_angle(rocket_turret_angle[i]);
            draw_region_rotated_at(turret_x - camera_x, turret_y - camera_y);
        }

        // Renderizar misiles activos
        for (int m = 0; m < MaxMissiles; m++)
        {
            if (!missile_active[i][m])
                continue;

            select_texture(TextureShipRocket);

            // Calcular el frame de la animación (4 frames)
            int missile_frame = (get_frame_counter() / 6) % 4; // Cambiar cada 6 frames
            select_region(missile_frame);
            define_region(
                missile_frame * 9, // Cada frame mide 9 de ancho
                0,
                (missile_frame + 1) * 9,
                20,
                9 / 2, // Punto central horizontal
                10     // Punto central vertical
            );

            set_drawing_angle(missile_angle[i][m] - pi / 2);
            draw_region_rotated_at(missile_x[i][m] - camera_x, missile_y[i][m] - camera_y);
        }
    }
}