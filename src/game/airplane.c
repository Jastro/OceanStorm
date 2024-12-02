#include "airplane.h"
#include "island.h"
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

// Variable externa para el estado del juego
extern int game_state;

int is_over_carrier()
{
    float carrier_center_x = StartingX + (CarrierWidth / 2);
    float carrier_bottom = StartingY + CarrierHeight; // Parte inferior del carrier

    // Comprobar si estamos sobre el carrier completo
    float dx = airplane_x - carrier_center_x;
    float dy = airplane_y - StartingY; // Distancia desde la parte superior

    // El área de aterrizaje cubre todo el carrier
    return (dx >= -CarrierWidth / 2 && dx <= CarrierWidth / 2 && // Dentro del ancho del carrier
            dy >= 0 && dy <= CarrierHeight);                     // Dentro del alto del carrier
}

void render_fuel_gauge()
{
    // Fondo de la barra de combustible (barra vacía)
    select_texture(-1); // Usar textura de la BIOS
    set_multiply_color(RedColor);

    // Dibujar texto
    print_at(10, 10, "FUEL:");

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

void initialize_airplane()
{
    is_player_in_vehicle = 1;
    select_texture(TextureAirplane);

    // Los frames están uno al lado del otro, no uno encima del otro
    // Frame 1
    select_region(0);
    define_region(
        0,                      // x inicial del primer frame
        0,                      // y inicial (mismo para ambos)
        AirplaneFrameWidth,     // anchura del frame
        AirplaneFrameHeight,    // altura completa
        AirplaneFrameWidth / 2, // punto central x (mitad del frame individual)
        AirplaneFrameHeight / 2 // punto central y
    );

    // Frame 2
    select_region(1);
    define_region(
        AirplaneFrameWidth,       // x inicial del segundo frame (mitad del sprite)
        0,                        // y inicial
        AirplaneFrameWidth * 2,   // ancho total para el segundo frame
        AirplaneFrameHeight,      // altura completa
        AirplaneFrameWidth * 1.5, // punto central x (mitad del frame individual)
        AirplaneFrameHeight / 2   // punto central y
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

void exit_vehicle() {
 if((is_over_carrier() || is_over_island(airplane_x, airplane_y))) {
        is_player_in_vehicle = 0;
        soldier_x = airplane_x + cos(airplane_angle) * 50;  // Usar el ángulo del avión
        soldier_y = airplane_y + sin(airplane_angle) * 50;
        soldier_state = SoldierStateActive;  // Asegurarse de que está activo
        target_zoom = CameraZoomGround;

        if(!has_event_happened(EventFirstExit)) {
            show_dialog("¡Presiona B para volver al avión cuando estés cerca!", -1);
            //mark_event_as_happened(EventFirstExit);
        }
    }
}

void enter_vehicle()
{
    if(!is_player_in_vehicle) {  // Solo si estamos fuera del avión
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

    // Movimiento hacia adelante
    if (gamepad_up() > 0 && fuel > 0)
    {
        airplane_x += MovementSpeed * sin(airplane_angle);
        airplane_y -= MovementSpeed * cos(airplane_angle);
        airplane_scale = clamp(airplane_scale + DescentSpeed, MinScale, MaxScale);
        fuel -= FuelConsumption;

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
            }
        }
        else if (airplane_scale <= MinScale)
        {
            // Si no estamos sobre ninguna superficie y bajamos demasiado -> Game Over
            game_state = StateGameOver;
        }
    }
    else
    {
        // Si estamos muy alto, seguimos descendiendo normalmente
        airplane_scale = clamp(airplane_scale - DescentSpeed, MinScale, MaxScale);
    }

    // Game over si nos quedamos sin combustible y sin altura
    if (fuel <= 0 && airplane_scale <= MinScale && !is_over_carrier() && !is_over_island(airplane_x, airplane_y))
    {
        game_state = StateGameOver;
    }

    // Mantener el avión dentro del mundo
    airplane_x = clamp(airplane_x, 0, WorldWidth);
    airplane_y = clamp(airplane_y, 0, WorldHeight);

    if(is_player_in_vehicle) {
        camera_x = airplane_x - ScreenCenterX;
        camera_y = airplane_y - ScreenCenterY;
    }

    if((is_over_carrier() || is_over_island(airplane_x, airplane_y)) && gamepad_button_b() == 1) {
            exit_vehicle();
            return;
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
    render_fuel_gauge();
}

void update_camera_zoom() {
    // Interpolar suavemente entre el zoom actual y el objetivo
    if(camera_zoom != target_zoom) {
        float diff = target_zoom - camera_zoom;
        camera_zoom += diff * CameraZoomSpeed;
    }
}