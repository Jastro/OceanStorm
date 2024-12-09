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
#include "dialogTexts.h"

// Variables globales para el estado del avión
float heli_x;
float heli_y;
float camera_x;
float camera_y;
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

// Variable externa para el estado del juego
extern int game_state;

void reload_heli()
{
    heli_current_ammo = clamp(
        heli_current_ammo + HeliReloadRate,
        0,
        HeliMaxAmmo);
}

int soldier_is_over_carrier(float x, float y)
{
    float dx = x - (StartingX - CarrierWidth / 2);
    float dy = y - (StartingY - CarrierHeight / 2);

    // El área de aterrizaje cubre todo el carrier
    return (dx >= 0 && dx <= CarrierWidth) && (dy >= 0 && dy <= CarrierHeight);
}

int is_over_carrier()
{
    // Comprobar si estamos sobre el carrier completo
    float dx = heli_x - (StartingX - CarrierWidth / 2);
    float dy = heli_y - (StartingY - CarrierHeight / 2);

    // El área de aterrizaje cubre todo el carrier
    return (dx >= 0 && dx <= CarrierWidth) && (dy >= 0 && dy <= CarrierHeight);
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

    if (fuel <= (MaxFuel / 2)) {
        if (!has_event_happened(LowFuel))
            {
                queue_dialog(DT_FuelHalf, RegionPortraitCommander);
                queue_dialog(DT_FuelHalfReply, RegionPortraitPlayer);
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
        float shoot_angle = heli_angle - pi / 2;
        float bullet_x = heli_x + cos(shoot_angle) * HeliFrameWidth * heli_scale * 0.5;
        float bullet_y = heli_y + sin(shoot_angle) * HeliFrameWidth * heli_scale * 0.5;
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
        0,                   // x inicial del primer frame
        0,                   // y inicial (mismo para ambos)
        HeliFrameWidth,  // anchura del frame
        HeliFrameHeight, // altura completa
        39,                  // punto central x (mitad del frame individual)
        39                   // punto central y
    );

    // Frame 2
    select_region(1);
    define_region(
        HeliFrameWidth,      // x inicial del segundo frame (mitad del sprite)
        0,                       // y inicial
        HeliFrameWidth * 2,  // ancho total para el segundo frame
        HeliFrameHeight,     // altura completa
        39 + HeliFrameWidth, // punto central x (mitad del frame individual)
        39                       // punto central y
    );

    // Definir regiones de sombra
    // Sombra Frame 1
    select_region(RegionHeliShadow);
    define_region(
        0,                      // x inicial del primer frame
        0,                      // y inicial (mismo para ambos)
        HeliFrameWidth,     // anchura del frame
        HeliFrameHeight,    // altura completa
        HeliFrameWidth / 2, // punto central x (mitad del frame individual)
        HeliFrameHeight / 2 // punto central y
    );

    // Sombra Frame 2
    select_region(RegionHeliShadow + 1);
    define_region(
        HeliFrameWidth,       // x inicial del segundo frame (mitad del sprite)
        0,                        // y inicial
        HeliFrameWidth * 2,   // ancho total para el segundo frame
        HeliFrameHeight,      // altura completa
        HeliFrameWidth * 1.5, // punto central x (mitad del frame individual)
        HeliFrameHeight / 2   // punto central y
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
    camera_x = heli_x - ScreenCenterX;
    camera_y = heli_y - ScreenCenterY;
}

void exit_vehicle()
{
    // Si ya estamos fuera, no hacer nada
    if (!is_player_in_vehicle)
        return;
    
    // Intentar salir del heli por un lado
    soldier_x = heli_x + cos(heli_angle) * 30;
    soldier_y = heli_y + sin(heli_angle) * 30;

    if (!is_over_island(soldier_x, soldier_y) && !soldier_is_over_carrier(soldier_x, soldier_y))
    {
        // Intentar salir por el lado contrario
        soldier_x = heli_x - cos(heli_angle) * 30;
        soldier_y = heli_y - sin(heli_angle) * 30;
        
        // Si no podemos, cancelar la salida
        // (reproducir sonido indicandolo)
        if (!is_over_island(soldier_x, soldier_y) && !soldier_is_over_carrier(soldier_x, soldier_y))
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
        if (is_over_carrier() || is_over_island(heli_x, heli_y))
        {
            // Aterrizaje exitoso
            heli_scale = LandingScale; // Mantenemos esta escala para que se vea bien

            // Solo repostar si estamos en el carrier
            if (is_over_carrier())
            {
                fuel = clamp(fuel + RefuelRate, 0, MaxFuel);
                reload_heli();
            }
        }
        else if (heli_scale <= MinScale)
        {
            // Si no estamos sobre ninguna superficie y bajamos demasiado -> Game Over
            game_state = StateGameOver;
        }
    }

    // Game over si nos quedamos sin combustible y sin altura
    if (fuel <= 0 && heli_scale <= MinScale && !is_over_carrier() && !is_over_island(heli_x, heli_y))
    {
        game_state = StateGameOver;
    }

    // Mantener el avión dentro del mundo
    heli_x = clamp(heli_x, 0, WorldWidth);
    heli_y = clamp(heli_y, 0, WorldHeight);

    if (is_player_in_vehicle)
    {
        camera_x = heli_x - ScreenCenterX;
        camera_y = heli_y - ScreenCenterY;
    }

    if ((is_over_carrier() || is_over_island(heli_x, heli_y)) && gamepad_button_b() == 1)
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
    int max_bar_width = 100;
    int bar_height = 10;
    int health_width = (int)((heli_health / (float)HeliMaxHealth) * max_bar_width);

    // Barra de vida
    set_multiply_color(RedColor);
    print_at(10, 35, "HP:");
    for (int x = 0; x < max_bar_width; x++)
    {
        for (int y = 0; y < bar_height; y++)
        {
            draw_region_at(60 + x, 30 + y);
        }
    }

    // Verde normal, rojo cuando recibe daño
    if (health_flash_timer > 0)
    {
        set_multiply_color(RedColor);
    }
    else
    {
        set_multiply_color(GreenColor);
    }

    for (int x = 0; x < health_width; x++)
    {
        for (int y = 0; y < bar_height; y++)
        {
            draw_region_at(60 + x, 30 + y);
        }
    }
}

void render_heli()
{
    float height_factor = (heli_scale - LandingScale) / (MaxScale - LandingScale);
    float max_shadow_distance = HeliShadowOffset;
    float shadow_offset = height_factor * max_shadow_distance;

    // 1. Dibujar la sombra primero
    select_texture(TextureHeli);
    select_region(RegionHeliShadow + heli_frame);
    set_multiply_color(0x80000000); // Negro semi-transparente
    set_drawing_scale(heli_scale, heli_scale);
    set_drawing_angle(heli_angle);
    draw_region_rotozoomed_at(
        heli_x - camera_x + shadow_offset,
        heli_y - camera_y + shadow_offset);

    // 2. Dibujar el avión
    set_multiply_color(0xFFFFFFFF);
    select_region(heli_frame);
    set_drawing_scale(heli_scale, heli_scale);
    set_drawing_angle(heli_angle);
    draw_region_rotozoomed_at(heli_x - camera_x, heli_y - camera_y);

    // 3. Dibujar la interfaz
    if (is_player_in_vehicle)
    {
        render_ui();
    } else {
        if (heli_scale > LandingScale) {
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
    // Interpolar suavemente entre el zoom actual y el objetivo
    if (camera_zoom != target_zoom)
    {
        float diff = target_zoom - camera_zoom;
        camera_zoom += diff * CameraZoomSpeed;
    }
}