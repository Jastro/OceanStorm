#include "ship.h"

void initialize_ships()
{
    for (int i = 0; i < MaxShips; i++)
    {
        ship_active[i] = 0;
        for (int t = 0; t < 3; t++)
        {
            ship_turret_active[i][t] = 0;
            ship_turret_last_shot[i][t] = 0;
        }
    }

    for (int i = 0; i < MaxMissiles; i++)
    {
        missile_active[i] = 0;
    }
}

void update_ship_turrets(int ship_index)
{
    float current_time = get_frame_counter() / 60.0;

    int num_turrets = (ship_type[ship_index] == ShipTypeLarge) ? 3 : (ship_type[ship_index] == ShipTypeMedium) ? 2
                                                                                                               : 1;

    for (int t = 0; t < num_turrets; t++)
    {
        if (!ship_turret_active[ship_index][t])
            continue;

        // Calcular ángulo hacia el objetivo
        float target_x = is_player_in_vehicle ? airplane_x : soldier_x;
        float target_y = is_player_in_vehicle ? airplane_y : soldier_y;

        float turret_x = ship_x[ship_index];
        float turret_y = ship_y[ship_index];

        // Ajustar posición según tipo de barco
        if (ship_type[ship_index] == ShipTypeLarge)
        {
            if (t == 0)
            {
                turret_x += LargeShipFrontTurretX;
                turret_y += LargeShipFrontTurretY;
            }
            else if (t == 1)
            {
                turret_x += LargeShipMidTurretX;
                turret_y += LargeShipMidTurretY;
            }
            else
            {
                turret_x += LargeShipBackTurretX;
                turret_y += LargeShipBackTurretY;
            }
        }
        // Similar para otros tipos...

        float dx = target_x - turret_x;
        float dy = target_y - turret_y;
        float angle = atan2(dy, dx);
        ship_turret_angle[ship_index][t] = angle;

        // Disparar
        if (current_time - ship_turret_last_shot[ship_index][t] >= TurretFireRate)
        {
            if (ship_type[ship_index] == ShipTypeMedium && t == 0)
            {
                // Lanzacohetes
                launch_missile(turret_x, turret_y, angle);
            }
            else
            {
                // Torreta normal
                create_bullet(turret_x, turret_y, angle, 0, BulletTypeTurret);
            }
            ship_turret_last_shot[ship_index][t] = current_time;
        }
    }
}

void update_missiles()
{
    for (int i = 0; i < MaxMissiles; i++)
    {
        if (!missile_active[i])
            continue;

        missile_lifetime[i] -= 1.0 / 60.0;
        if (missile_lifetime[i] <= 0)
        {
            missile_active[i] = 0;
            continue;
        }

        // Perseguir al objetivo
        float target_x = is_player_in_vehicle ? airplane_x : soldier_x;
        float target_y = is_player_in_vehicle ? airplane_y : soldier_y;

        float dx = target_x - missile_x[i];
        float dy = target_y - missile_y[i];
        float target_angle = atan2(dy, dx);

        // Girar suavemente hacia el objetivo
        float angle_diff = target_angle - missile_angle[i];
        while (angle_diff > pi)
            angle_diff -= 2 * pi;
        while (angle_diff < -pi)
            angle_diff += 2 * pi;

        missile_angle[i] += angle_diff * MissileTurnSpeed;

        // Mover
        missile_x[i] += cos(missile_angle[i]) * missile_speed[i];
        missile_y[i] += sin(missile_angle[i]) * missile_speed[i];
    }
}

void render_ships()
{
    for (int i = 0; i < MaxShips; i++)
    {
        if (!ship_active[i])
            continue;

        // Seleccionar textura según tipo
        select_texture(ship_type[i] == ShipTypeLarge ? TextureLargeShip : ship_type[i] == ShipTypeMedium ? TextureMediumShip
                                                                                                         : TextureSmallShip);

        // Seleccionar frame normal o destruido
        select_region(ship_health[i] <= 0 ? RegionShipDestroyed : RegionShipNormal);

        // Dibujar barco
        draw_region_at(ship_x[i] - camera_x, ship_y[i] - camera_y);

        // Dibujar torretas
        int num_turrets = (ship_type[i] == ShipTypeLarge) ? 3 : (ship_type[i] == ShipTypeMedium) ? 2
                                                                                                 : 1;

        for (int t = 0; t < num_turrets; t++)
        {
            if (!ship_turret_active[i][t])
                continue;

            // ... Código para dibujar cada torreta con su rotación ...
        }
    }
}

void launch_missile(float x, float y, float angle)
{
    for (int i = 0; i < MaxMissiles; i++)
    {
        if (missile_active[i])
            continue;

        missile_x[i] = x;
        missile_y[i] = y;
        missile_angle[i] = angle;
        missile_speed[i] = MissileSpeed;
        missile_lifetime[i] = MissileLifetime;
        missile_active[i] = 1;
        break;
    }
}