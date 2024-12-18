#include "heli.h"
#include "island.h"
#include "bullet.h"
#include "render_system.h"
#include "../utils/definitions.h"
#include "../utils/utilities.h"
#include "video.h"
#include "input.h"
#include "math.h"
#include "string.h"
#include "soldier.h"
#include "events.h"
#include "weapon.h"

// Variables globales para el estado del avión
float heli_x;
float heli_y;
float heli_angle;
float heli_scale;
float heli_velocity;
float fuel;
int heli_frame;
int anim_timer;
int is_player_in_vehicle = 1;
int heli_current_ammo;
float heli_last_shot_time;
int heli_health;
float health_flash_timer = 0;
int active_cannon = 0;

// Variable externa para el estado del juego
extern int game_state;

void reload_heli()
{
    heli_current_ammo = clamp(
        heli_current_ammo + HeliReloadRate,
        0,
        HeliMaxAmmo);
}

void render_fuel_gauge()
{
    // Fondo de la barra de combustible (barra vacía)
    select_texture(-1); // Usar textura de la BIOS
    set_multiply_color(RedColor);

    // Dibujar texto
    print_at(10, 15, "FUEL:");

    // Calcular ancho de la barra basado en el combustible
    int max_bar_width = 100;
    int bar_height = 10;
    int current_width = (int)((fuel / MaxFuel) * max_bar_width);

    // Dibujar barra de fondo (roja)
    for (int x = 0; x < max_bar_width; x++)
    {
        for (int y = 0; y < bar_height; y++)
        {
            draw_region_at(60 + x, 10 + y);
        }
    }

    // Dibujar barra de combustible actual (blanca)
    set_multiply_color(TextColor);
    for (int x = 0; x < current_width; x++)
    {
        for (int y = 0; y < bar_height; y++)
        {
            draw_region_at(60 + x, 10 + y);
        }
    }

    if (fuel <= (MaxFuel / 2))
    {
        if (!has_event_happened(LowFuel))
        {
            queue_dialog(&DW_FuelHalf);
            queue_dialog(&DW_FuelHalfReply);
            start_dialog_sequence();

            mark_event_as_happened(LowFuel);
        }
    }
}

void shoot_from_heli()
{
    float current_time = get_frame_counter() / 60.0;

    if (heli_current_ammo > 0 &&
        current_time - heli_last_shot_time >= HeliFireRate)
    {
        float shoot_angle;
        float offset_angle;

        switch (active_cannon)
        {
        case 0: // Frontal
            shoot_angle = heli_angle - pi / 2;
            offset_angle = shoot_angle;
            break;
        case 1: // Trasero
            shoot_angle = heli_angle + pi / 2;
            offset_angle = shoot_angle;
            break;
        case 2: // Lateral izquierdo
            shoot_angle = heli_angle - pi;
            offset_angle = shoot_angle;
            break;
        case 3: // Lateral derecho
            shoot_angle = heli_angle;
            offset_angle = shoot_angle;
            break;
        }

        // Calcular punto de origen de la bala usando el offset correcto
        float bullet_x = heli_x + cos(offset_angle) * HeliFrameWidth * heli_scale * 0.5;
        float bullet_y = heli_y + sin(offset_angle) * HeliFrameWidth * heli_scale * 0.5;

        create_bullet(bullet_x, bullet_y, shoot_angle, 0, BulletTypePlayer);

        heli_current_ammo--;
        heli_last_shot_time = current_time;
    }
}

void initialize_heli()
{
    select_texture(TextureHeli);

    // Los frames están uno al lado del otro, no uno encima del otro
    // Frame 1
    select_region(0);
    define_region(
        0,               // x inicial del primer frame
        0,               // y inicial (mismo para ambos)
        HeliFrameWidth,  // anchura del frame
        HeliFrameHeight, // altura completa
        47,              // punto central x (mitad del frame individual)
        39               // punto central y
    );

    // Frame 2
    select_region(1);
    define_region(
        HeliFrameWidth,      // x inicial del segundo frame (mitad del sprite)
        0,                   // y inicial
        HeliFrameWidth * 2,  // ancho total para el segundo frame
        HeliFrameHeight,     // altura completa
        47 + HeliFrameWidth, // punto central x (mitad del frame individual)
        39                   // punto central y
    );

    heli_frame = 0;
    anim_timer = 0;
    reset_heli();
}

void reset_heli()
{
    // El avión empieza en el centro del carrier, pero 40 pixels a la izquierda
    heli_x = StartingX + (CarrierWidth / 2) - 40;
    heli_y = StartingY + 20;
    heli_angle = 0.0;
    heli_scale = InitialScale;
    heli_velocity = 0.0;
    fuel = MaxFuel;
    heli_health = HeliMaxHealth;
    heli_current_ammo = HeliMaxAmmo;
    heli_last_shot_time = 0;

    // Centrar la cámara en el avión
    world_map.camera_position.x = heli_x;
    world_map.camera_position.y = heli_y;
}

void exit_vehicle()
{
    // Si ya estamos fuera, no hacer nada
    if (!is_player_in_vehicle)
        return;

    // Intentar salir del heli por un lado
    soldier_x = heli_x + cos(heli_angle) * 30;
    soldier_y = heli_y + sin(heli_angle) * 30;

    if (is_over_ocean(soldier_x, soldier_y))
    {
        // Intentar salir por el lado contrario
        soldier_x = heli_x - cos(heli_angle) * 30;
        soldier_y = heli_y - sin(heli_angle) * 30;

        // Si no podemos, cancelar la salida
        // (reproducir sonido indicandolo)
        if (is_over_ocean(soldier_x, soldier_y))
            return;
    }

    // Actualizar estado solo si hemos salido
    is_player_in_vehicle = 0;
    soldier_state = SoldierStateActive;
    target_zoom = CameraZoomGround;
}

void enter_vehicle()
{
    // Si ya estamos dentro, no hacer nada
    if (is_player_in_vehicle)
        return;

    is_player_in_vehicle = 1;
    soldier_state = SoldierStateNone;
    target_zoom = CameraZoomAir;
}

void update_heli()
{
    if (!is_player_in_vehicle)
        return;

    // Obtener entrada del control
    int direction_x, direction_y;
    gamepad_direction(&direction_x, &direction_y);

    if (gamepad_button_l() == 1)
        active_cannon = (active_cannon + 1) % 4;

    // Rotar el avión
    if (gamepad_left() > 0)
        heli_angle -= RotationSpeed;
    if (gamepad_right() > 0)
        heli_angle += RotationSpeed;

    if (gamepad_button_a() > 0)
    {
        shoot_from_heli();
    }

    // Movimiento hacia adelante
    if (gamepad_up() > 0 && fuel > 0)
    {
        heli_x += MovementSpeed * sin(heli_angle);
        heli_y -= MovementSpeed * cos(heli_angle);
        fuel -= FuelConsumption;
        heli_scale = clamp(heli_scale + AscendSpeed, MinScale, MaxScale);

        // Actualizar animación
        anim_timer++;
        if (anim_timer >= HeliAnimSpeed)
        {
            anim_timer = 0;
            heli_frame = 1 - heli_frame; // Alternar entre 0 y 1
        }
    }
    else
    {
        heli_scale = clamp(heli_scale - DescentSpeed, MinScale, MaxScale);
        heli_frame = 0; // Frame base cuando no se mueve
        anim_timer = 0;
    }

    // Comprobar aterrizaje
    if (heli_scale <= LandingScale)
    {
        // Si estamos sobre el carrier o una isla, permitir aterrizaje
        if (is_over_carrier(heli_x, heli_y) || is_over_island(heli_x, heli_y))
        {
            // Aterrizaje exitoso
            heli_scale = LandingScale; // Mantenemos esta escala para que se vea bien

            // Solo repostar si estamos en el carrier
            if (is_over_carrier(heli_x, heli_y))
            {
                // Recargar combustible
                fuel = clamp(fuel + RefuelRate, 0, MaxFuel);
                reload_heli();

                // Curar helicóptero
                heli_health = clamp(heli_health + RefuelRate, 0, HeliMaxHealth);

                // Restaurar soldado completamente
                soldier_health = SoldierMaxHealth;
                soldier_armor = MaxArmor;
                soldier_bombs = BombCount;

                // Recargar todas las armas del soldado
                for (int i = 0; i < 3; i++)
                {
                    if (soldier_has_weapon[i])
                    {
                        weapon_current_ammo[i] = weapon_max_ammo[i];
                    }
                }
            }
        }
        else if (heli_scale <= MinScale)
        {
            if (!has_event_happened(GameOver))
            {
                queue_dialog(&DW_GameOver);
                start_dialog_sequence();

                mark_event_as_happened(GameOver);
            }
            game_state = StateGameOver;
        }
    }

    // Game over si nos quedamos sin combustible y sin altura
    if (fuel <= 0 && heli_scale <= MinScale && !is_over_carrier(heli_x, heli_y) && !is_over_island(heli_x, heli_y))
    {
        if (!has_event_happened(GameOver))
        {
            queue_dialog(&DW_GameOver);
            start_dialog_sequence();

            mark_event_as_happened(GameOver);
        }
        game_state = StateGameOver;
    }

    // Mantener el avión dentro del mundo
    heli_x = clamp(heli_x, 0, WorldWidth);
    heli_y = clamp(heli_y, 0, WorldHeight);

    if (is_player_in_vehicle)
    {
        world_map.camera_position.x = heli_x;
        world_map.camera_position.y = heli_y;
    }

    if ((is_over_carrier(heli_x, heli_y) || is_over_island(heli_x, heli_y)) && gamepad_button_b() == 1)
    {
        exit_vehicle();
        return;
    }
}
void render_ui()
{
    render_fuel_gauge();
    set_multiply_color(TextColor);
    int[8] ammo_text;
    print_at(10, 60, "AMMO: ");
    itoa(heli_current_ammo, ammo_text, 10);
    print_at(70, 60, ammo_text);

    if (active_cannon == 0)
        print_at(10, 110, "CANNON: FRONT     [L to switch]");
    else if (active_cannon == 1)
        print_at(10, 110, "CANNON: REAR      [L to switch]");
    else if (active_cannon == 2)
        print_at(10, 110, "CANNON: LEFT SIDE [L to switch]");
    else
        print_at(10, 110, "CANNON: RIGHT SIDE[L to switch]");

    int max_bar_width = 100;
    int bar_height = 10;
    int health_width = (int)((heli_health / (float)HeliMaxHealth) * max_bar_width);

    // Barra de vida
    set_multiply_color(RedColor);
    print_at(10, 35, "HP:");

    set_multiply_color(ShadowColor);
    select_texture(-1);
    select_region(256);
    set_drawing_scale(max_bar_width, bar_height);
    draw_region_zoomed_at(60, 30);

    // Verde normal, rojo cuando recibe daño
    if (health_flash_timer > 0)
        set_multiply_color(RedColor);
    else
        set_multiply_color(GreenColor);

    set_drawing_scale(health_width, bar_height);
    draw_region_zoomed_at(60, 30);

    // Restaurar el color
    set_multiply_color(color_white);
}

void render_heli()
{
    float height_factor = (heli_scale - LandingScale) / (MaxScale - LandingScale);
    float max_shadow_distance = HeliShadowOffset;

    // Evitar que la sombra vaya al lado contrario
    // cuando estamos por debajo de las islas
    float shadow_offset = 2 + max(0, height_factor * max_shadow_distance);

    // Si la sombra esta sobre el mar reducirla para dar efecto
    // de mas distancia
    float shadow_x = heli_x + shadow_offset;
    float shadow_y = heli_y + shadow_offset;
    float shadow_scale = heli_scale;

    if (is_over_ocean(shadow_x, shadow_y))
        shadow_scale *= 0.7;

    // 1. Dibujar la sombra primero
    select_texture(TextureHeli);
    select_region(RegionHeli + heli_frame);
    set_multiply_color(0x80000000); // Negro semi-transparente
    set_drawing_scale(shadow_scale, shadow_scale);
    set_drawing_angle(heli_angle);
    tilemap_draw_region_rotozoomed(&world_map, shadow_x, shadow_y);

    // 2. Dibujar el avión
    set_multiply_color(0xFFFFFFFF);
    select_region(heli_frame);
    set_drawing_scale(heli_scale, heli_scale);
    set_drawing_angle(heli_angle);
    tilemap_draw_region_rotozoomed(&world_map, heli_x, heli_y);

    // Restaurar el color
    set_multiply_color(color_white);

    // 3. Dibujar la interfaz
    if (is_player_in_vehicle)
    {
        render_ui();
    }
    else
    {
        if (heli_scale > LandingScale)
        {
            heli_scale = clamp(heli_scale - DescentSpeed, MinScale, MaxScale);
        }
    }

    if (health_flash_timer > 0)
    {
        health_flash_timer -= 1.0 / 60.0;
    }
}

void update_camera_zoom()
{
    if (camera_zoom != target_zoom)
    {
        float diff = target_zoom - camera_zoom;
        camera_zoom += diff * CameraZoomSpeed;
    }
}