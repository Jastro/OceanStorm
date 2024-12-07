#include "ship.h"
#include "../utils/definitions.h"
#include "video.h"
#include "island.h"
#include "math.h"

float[MaxShips] ship_x;
float[MaxShips] ship_y;
float[MaxShips] ship_angle;
float[MaxShips] ship_speed;
int[MaxShips] ship_type;
int[MaxShips] ship_state;
int[MaxShips] ship_health;
int[MaxShips] ship_active;
float[MaxShips][3] ship_weapon_offset_x;
float[MaxShips][3] ship_weapon_offset_y;
int[MaxShips][3] ship_weapon_type;

void initialize_ships()
{
    for (int i = 0; i < MaxShips; i++)
    {
        ship_active[i] = 0;
        ship_state[i] = ShipStateActive;

        // Inicializar offsets por defecto
        for (int j = 0; j < 3; j++)
        {
            ship_weapon_offset_x[i][j] = 0;
            ship_weapon_offset_y[i][j] = DefaultTurretOffset * j;
            ship_weapon_type[i][j] = 0;
        }
    }
    spawn_ships();
}

// Función auxiliar para comprobar si un punto está en el agua
int is_in_water(float x, float y)
{
    // No está sobre ninguna isla y está dentro del mundo
    return !is_over_island(x, y) &&
           x >= 0 && x < WorldWidth &&
           y >= 0 && y < WorldHeight;
}

int check_ship_collision(float x, float y, float width, float height, int current_ship)
{
    for (int i = 0; i < MaxShips; i++)
    {
        if (!ship_active[i] || i == current_ship)
            continue;

        // Obtener dimensiones según el tipo
        float other_width, other_height;
        switch (ship_type[i])
        {
        case ShipTypeLarge:
            other_width = LargeShipWidth;
            other_height = LargeShipHeight;
            break;
        case ShipTypeMedium:
            other_width = MediumShipWidth;
            other_height = MediumShipHeight;
            break;
        default:
            other_width = SmallShipWidth;
            other_height = SmallShipHeight;
            break;
        }

        // Comprobar colisión rectangular
        if (x < ship_x[i] + other_width &&
            x + width > ship_x[i] &&
            y < ship_y[i] + other_height &&
            y + height > ship_y[i])
        {
            return 1;
        }
    }
    return 0;
}

int find_valid_ship_position(float *out_x, float *out_y)
{
    int max_attempts = 100;
    int attempt = 0;

    while (attempt < max_attempts)
    {
        float test_x = rand() % WorldWidth;
        float test_y = rand() % WorldHeight;

        // Comprobar un área grande alrededor del punto para asegurar que hay espacio
        int valid = 1;
        for (float dx = -LargeShipWidth; dx <= LargeShipWidth; dx += LargeShipWidth / 2)
        {
            for (float dy = -LargeShipHeight; dy <= LargeShipHeight; dy += LargeShipHeight / 2)
            {
                if (!is_in_water(test_x + dx, test_y + dy))
                {
                    valid = 0;
                    break;
                }
            }
            if (!valid)
                break;
        }

        if (valid)
        {
            *out_x = test_x;
            *out_y = test_y;
            return 1;
        }

        attempt++;
    }

    return 0;
}

void spawn_ships()
{
    int num_large = 0;

    // Primero spawneamos los barcos grandes obligatorios
    for (int i = 0; i < MinLargeShips; i++)
    {
        float spawn_x, spawn_y;
        if (find_valid_ship_position(&spawn_x, &spawn_y))
        {
            for (int j = 0; j < MaxShips; j++)
            {
                if (!ship_active[j])
                {
                    ship_x[j] = spawn_x;
                    ship_y[j] = spawn_y;
                    ship_type[j] = ShipTypeLarge;
                    ship_active[j] = 1;
                    ship_health[j] = 100;
                    ship_speed[j] = 1.0;
                    ship_angle[j] = 0;
                    num_large++;
                    break;
                }
            }
        }
    }

    // Luego llenamos el resto con barcos aleatorios
    while (num_large < MaxShips)
    {
        float spawn_x, spawn_y;
        if (find_valid_ship_position(&spawn_x, &spawn_y))
        {
            for (int j = 0; j < MaxShips; j++)
            {
                if (!ship_active[j])
                {
                    ship_x[j] = spawn_x;
                    ship_y[j] = spawn_y;
                    ship_type[j] = (rand() % 2) + 1; // Medium o Small
                    ship_active[j] = 1;

                    // Establecer vida según tipo
                    if (ship_type[j] == ShipTypeMedium)
                    {
                        ship_health[j] = 75; // 50 + 25
                    }
                    else
                    {
                        ship_health[j] = 50; // Barco pequeño
                    }

                    // Establecer velocidad según tipo
                    if (ship_type[j] == ShipTypeMedium)
                    {
                        ship_speed[j] = 1.2; // 1.5 - 0.3
                    }
                    else
                    {
                        ship_speed[j] = 1.5; // Barco pequeño
                    }

                    ship_angle[j] = 0;
                    break;
                }
            }
        }
        num_large++;
    }
}

void update_ships()
{
    for (int i = 0; i < MaxShips; i++)
    {
        if (!ship_active[i] || ship_state[i] == ShipStateDestroyed)
            continue;

        // Obtener dimensiones del barco actual
        float width, height;
        switch (ship_type[i])
        {
        case ShipTypeLarge:
            width = LargeShipWidth;
            height = LargeShipHeight;
            break;
        case ShipTypeMedium:
            width = MediumShipWidth;
            height = MediumShipHeight;
            break;
        default:
            width = SmallShipWidth;
            height = SmallShipHeight;
            break;
        }

        // Calcular nueva posición potencial
        float new_x = ship_x[i] + cos(ship_angle[i]) * ship_speed[i];
        float new_y = ship_y[i] + sin(ship_angle[i]) * ship_speed[i];

        // Verificar colisiones
        if (is_in_water(new_x, new_y) &&
            is_in_water(new_x + width, new_y) &&
            is_in_water(new_x, new_y + height) &&
            is_in_water(new_x + width, new_y + height) &&
            !check_ship_collision(new_x, new_y, width, height, i))
        {

            ship_x[i] = new_x;
            ship_y[i] = new_y;
        }
        else
        {
            // Si hay colisión, intentar cambiar de dirección
            int random_direction = rand() % 2;
            if (random_direction == 0)
            {
                ship_angle[i] += pi / 4;
            }
            else
            {
                ship_angle[i] -= pi / 4;
            }
        }
    }
}

void render_ships()
{
    for (int i = 0; i < MaxShips; i++)
    {
        if (!ship_active[i])
            continue;

        // Definir región según tipo primero
        float width, height;
        switch (ship_type[i])
        {
        case ShipTypeLarge:
            width = LargeShipWidth;
            height = LargeShipHeight;
            break;
        case ShipTypeMedium:
            width = MediumShipWidth;
            height = MediumShipHeight;
            break;
        default:
            width = SmallShipWidth;
            height = SmallShipHeight;
            break;
        }

        // Seleccionar textura según tipo
        switch (ship_type[i])
        {
        case ShipTypeLarge:
            select_texture(TextureLargeShip);
            break;
        case ShipTypeMedium:
            select_texture(TextureMediumShip);
            break;
        case ShipTypeSmall:
            select_texture(TextureSmallShip);
            break;
        }
        set_multiply_color(TextColor);
        // Calcular el offset X basado en si está destruido
        int x_offset = 0;
        if (ship_state[i] == ShipStateDestroyed)
        {
            x_offset = width;
        }

        // Seleccionar si es normal o destruido
        select_region(ship_state[i]);

        define_region(
            x_offset,         // X inicial
            0,                // Y inicial
            x_offset + width, // X final
            height,           // Y final
            width / 2,        // Punto central X
            height / 2        // Punto central Y
        );

        // Dibujar barco
        set_drawing_angle(ship_angle[i] - pi/2);
        draw_region_rotated_at(ship_x[i] - camera_x, ship_y[i] - camera_y);

        // Dibujar armas para barcos grandes
        if (ship_type[i] == ShipTypeLarge && ship_state[i] != ShipStateDestroyed)
        {
            for (int j = 0; j < 3; j++)
            {
                float weapon_x = ship_x[i] + ship_weapon_offset_x[i][j];
                float weapon_y = ship_y[i] + ship_weapon_offset_y[i][j];

                if (ship_weapon_type[i][j] == 0)
                { // Torreta
                    select_texture(TextureShipTurret);
                    select_region(0);
                    define_region(0, 0, LargeTurretWidth, LargeTurretHeight,
                                  LargeTurretWidth / 2, LargeTurretHeight / 2);
                }
                else
                { // Lanzacohetes
                    select_texture(TextureShipRocket);
                    select_region(0);
                    define_region(0, 0, MediumRocketWidth, MediumRocketHeight,
                                  MediumRocketWidth / 2, MediumRocketHeight / 2);
                }

                draw_region_rotated_at(
                    weapon_x - camera_x,
                    weapon_y - camera_y);
            }
        }
    }
}

void damage_ship(int index, int damage)
{
    if (ship_active[index] && ship_state[index] != ShipStateDestroyed)
    {
        ship_health[index] -= damage;
        if (ship_health[index] <= 0)
        {
            ship_state[index] = ShipStateDestroyed;
        }
    }
}