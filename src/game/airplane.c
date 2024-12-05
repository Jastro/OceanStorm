#include "airplane.h"
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

// Variables globales para el estado del avión
float airplane_x;
float airplane_y;
float camera_x;
float camera_y;
float airplane_angle;
float airplane_scale;
float airplane_velocity;
float fuel;
int airplane_frame;
int anim_timer;
int is_player_in_vehicle = 1;
int airplane_current_ammo;
float airplane_last_shot_time;
int airplane_health;
float health_flash_timer = 0;

// Variable externa para el estado del juego
extern int game_state;

void reload_airplane()
{
    airplane_current_ammo = clamp(
        airplane_current_ammo + AirplaneReloadRate,
        0,
        AirplaneMaxAmmo);
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
    float dx = airplane_x - (StartingX - CarrierWidth / 2);
    float dy = airplane_y - (StartingY - CarrierHeight / 2);

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
}

void shoot_from_airplane()
{
    float current_time = get_frame_counter() / 60.0;

    if (airplane_current_ammo > 0 &&
        current_time - airplane_last_shot_time >= AirplaneFireRate)
    {
        float shoot_angle = airplane_angle - pi / 2;
        float bullet_x = airplane_x + cos(shoot_angle) * AirplaneFrameWidth * airplane_scale * 0.5;
        float bullet_y = airplane_y + sin(shoot_angle) * AirplaneFrameWidth * airplane_scale * 0.5;
        create_bullet(bullet_x, bullet_y, shoot_angle, 0, BulletTypePlayer);

        airplane_current_ammo--;
        airplane_last_shot_time = current_time;
    }
}

void initialize_airplane()
{
    airplane_health = AirplaneMaxHealth;
    airplane_last_shot_time = 0;
    airplane_current_ammo = AirplaneMaxAmmo;
    is_player_in_vehicle = 1;
    select_texture(TextureAirplane);

    // Los frames están uno al lado del otro, no uno encima del otro
    // Frame 1
    select_region(0);
    define_region(
        0,                   // x inicial del primer frame
        0,                   // y inicial (mismo para ambos)
        AirplaneFrameWidth,  // anchura del frame
        AirplaneFrameHeight, // altura completa
        39,                  // punto central x (mitad del frame individual)
        39                   // punto central y
    );

    // Frame 2
    select_region(1);
    define_region(
        AirplaneFrameWidth,      // x inicial del segundo frame (mitad del sprite)
        0,                       // y inicial
        AirplaneFrameWidth * 2,  // ancho total para el segundo frame
        AirplaneFrameHeight,     // altura completa
        39 + AirplaneFrameWidth, // punto central x (mitad del frame individual)
        39                       // punto central y
    );

    // Definir regiones de sombra
    // Sombra Frame 1
    select_region(RegionAirplaneShadow);
    define_region(
        0,                      // x inicial del primer frame
        0,                      // y inicial (mismo para ambos)
        AirplaneFrameWidth,     // anchura del frame
        AirplaneFrameHeight,    // altura completa
        AirplaneFrameWidth / 2, // punto central x (mitad del frame individual)
        AirplaneFrameHeight / 2 // punto central y
    );

    // Sombra Frame 2
    select_region(RegionAirplaneShadow + 1);
    define_region(
        AirplaneFrameWidth,       // x inicial del segundo frame (mitad del sprite)
        0,                        // y inicial
        AirplaneFrameWidth * 2,   // ancho total para el segundo frame
        AirplaneFrameHeight,      // altura completa
        AirplaneFrameWidth * 1.5, // punto central x (mitad del frame individual)
        AirplaneFrameHeight / 2   // punto central y
    );

    airplane_frame = 0;
    anim_timer = 0;
    reset_airplane();
}

void exit_vehicle()
{
    is_player_in_vehicle = 0;
    soldier_x = airplane_x + cos(airplane_angle) * 50;
    soldier_y = airplane_y + sin(airplane_angle) * 50;
    soldier_state = SoldierStateActive;
    target_zoom = CameraZoomGround;
    /*if (!has_event_happened(EventFirstExit))
    {
        show_dialog("¡Presiona B para volver al avión cuando estés cerca!", -1);
        // mark_event_as_happened(EventFirstExit);
    }*/
}

void enter_vehicle()
{
    if (!is_player_in_vehicle)
    { // Solo si estamos fuera del avión
        is_player_in_vehicle = 1;
        soldier_state = SoldierStateNone;
        target_zoom = CameraZoomAir;
    }
}

void reset_airplane()
{
    // El avión empieza en el centro del carrier, pero 40 pixels a la izquierda
    airplane_x = StartingX + (CarrierWidth / 2) - 40;
    airplane_y = StartingY + 20;
    airplane_angle = 0.0;
    airplane_scale = InitialScale;
    airplane_velocity = 0.0;
    fuel = MaxFuel;
    airplane_health = AirplaneMaxHealth;
    airplane_current_ammo = AirplaneMaxAmmo;

    // Centrar la cámara en el avión
    camera_x = airplane_x - ScreenCenterX;
    camera_y = airplane_y - ScreenCenterY;
}

void update_airplane()
{
    if (!is_player_in_vehicle)
        return;
    // Obtener entrada del control
    int direction_x, direction_y;
    gamepad_direction(&direction_x, &direction_y);

    // Rotar el avión
    if (gamepad_left() > 0)
        airplane_angle -= RotationSpeed;
    if (gamepad_right() > 0)
        airplane_angle += RotationSpeed;

    if (gamepad_button_a() > 0)
    {
        shoot_from_airplane();
    }

    // Movimiento hacia adelante
    if (gamepad_up() > 0 && fuel > 0)
    {
        airplane_x += MovementSpeed * sin(airplane_angle);
        airplane_y -= MovementSpeed * cos(airplane_angle);
        fuel -= FuelConsumption;
        airplane_scale = clamp(airplane_scale + AscendSpeed, MinScale, MaxScale);

        // Actualizar animación
        anim_timer++;
        if (anim_timer >= AirplaneAnimSpeed)
        {
            anim_timer = 0;
            airplane_frame = 1 - airplane_frame; // Alternar entre 0 y 1
        }
    }
    else
    {
        airplane_scale = clamp(airplane_scale - DescentSpeed, MinScale, MaxScale);
        airplane_frame = 0; // Frame base cuando no se mueve
        anim_timer = 0;
    }

    // Comprobar aterrizaje
    if (airplane_scale <= LandingScale)
    {
        // Si estamos sobre el carrier o una isla, permitir aterrizaje
        if (is_over_carrier() || is_over_island(airplane_x, airplane_y))
        {
            // Aterrizaje exitoso
            airplane_scale = LandingScale; // Mantenemos esta escala para que se vea bien

            // Solo repostar si estamos en el carrier
            if (is_over_carrier())
            {
                fuel = clamp(fuel + RefuelRate, 0, MaxFuel);
                reload_airplane();
            }
        }
        else if (airplane_scale <= MinScale)
        {
            // Si no estamos sobre ninguna superficie y bajamos demasiado -> Game Over
            game_state = StateGameOver;
        }
    }

    // Game over si nos quedamos sin combustible y sin altura
    if (fuel <= 0 && airplane_scale <= MinScale && !is_over_carrier() && !is_over_island(airplane_x, airplane_y))
    {
        game_state = StateGameOver;
    }

    // Mantener el avión dentro del mundo
    airplane_x = clamp(airplane_x, 0, WorldWidth);
    airplane_y = clamp(airplane_y, 0, WorldHeight);

    if (is_player_in_vehicle)
    {
        camera_x = airplane_x - ScreenCenterX;
        camera_y = airplane_y - ScreenCenterY;
    }

    if ((is_over_carrier() || is_over_island(airplane_x, airplane_y)) && gamepad_button_b() == 1)
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
    print_at(10, 40, "AMMO: ");
    itoa(airplane_current_ammo, ammo_text, 10);
    print_at(70, 40, ammo_text);
    int max_bar_width = 100;
    int bar_height = 10;
    int health_width = (int)((airplane_health / (float)AirplaneMaxHealth) * max_bar_width);

    // Barra de vida
    set_multiply_color(RedColor);
    print_at(10, 50, "HP:");
    for (int x = 0; x < max_bar_width; x++)
    {
        for (int y = 0; y < bar_height; y++)
        {
            draw_region_at(60 + x, 50 + y);
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
            draw_region_at(60 + x, 50 + y);
        }
    }
}

void render_airplane()
{
    // 2. Dibujar la sombra primero
    select_texture(TextureAirplane);
    select_region(RegionAirplaneShadow + airplane_frame);
    set_multiply_color(0x80000000); // Negro semi-transparente
    set_drawing_scale(airplane_scale, airplane_scale);
    set_drawing_angle(airplane_angle);
    draw_region_rotozoomed_at(
        airplane_x - camera_x + 20, // Offset más grande para la sombra
        airplane_y - camera_y + 20);

    // 3. Dibujar el avión
    set_multiply_color(0xFFFFFFFF); // Blanco opaco para el avión
    select_region(airplane_frame);
    set_drawing_scale(airplane_scale, airplane_scale);
    set_drawing_angle(airplane_angle);
    draw_region_rotozoomed_at(airplane_x - camera_x, airplane_y - camera_y);

    // 4. Dibujar la interfaz
    if (is_player_in_vehicle)
    {
        render_ui();
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