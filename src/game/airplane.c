#include "airplane.h"
#include "../utils/definitions.h"
#include "../utils/utilities.h"
#include "video.h"
#include "input.h"
#include "math.h"

// Variables globales para el estado del avión
float airplane_x;
float airplane_y;
float camera_x;
float camera_y;
float airplane_angle;
float airplane_scale;
float airplane_velocity;

// Variable externa para el estado del juego
extern int game_state;

// Función para comprobar si estamos sobre el portaaviones
int is_over_carrier() {
    if(airplane_x >= CarrierX - LandingDistance && 
       airplane_x <= CarrierX + LandingDistance &&
       airplane_y >= CarrierY - LandingDistance &&
       airplane_y <= CarrierY + LandingDistance) {
        return 1;
    }
    return 0;
}

void reset_airplane() {
    // Reiniciar posición y estado del avión
    airplane_x = WorldWidth / 2;
    airplane_y = WorldHeight / 2;
    camera_x = airplane_x - ScreenCenterX;
    camera_y = airplane_y - ScreenCenterY;
    airplane_angle = 0.0;
    airplane_scale = MaxScale;
    airplane_velocity = 0.0;
}

void initialize_airplane() {
    // Configurar región para el sprite del avión
    select_texture(0);
    
    // Región del avión (64x64 con centro en 32,32)
    select_region(RegionAirplane);
    define_region(0, 0, 64, 64, 32, 32);
    
    // Región de la sombra (mismas dimensiones)
    select_region(RegionAirplaneShadow);
    define_region(0, 0, 64, 64, 32, 32);
    
    // Configurar región para el portaaviones
    select_region(RegionCarrier);
    define_region(64, 0, 164, 32, 82, 16);
    
    // Inicializar el estado del avión
    reset_airplane();
}

void update_camera() {
    // La cámara sigue al avión
    camera_x = airplane_x - ScreenCenterX;
    camera_y = airplane_y - ScreenCenterY;
}

void update_airplane() {
    // Obtener entrada del control
    int direction_x, direction_y;
    gamepad_direction(&direction_x, &direction_y);
    
    // Rotar el avión
    if(gamepad_left() > 0)
        airplane_angle -= RotationSpeed;
    if(gamepad_right() > 0)
        airplane_angle += RotationSpeed;
    
    // Movimiento hacia adelante
    if(gamepad_up() > 0) {
        // Calcular el vector de dirección basado en el ángulo
        airplane_x += MovementSpeed * sin(airplane_angle);
        airplane_y -= MovementSpeed * cos(airplane_angle);
        
        // Aumentar altitud cuando se mueve
        airplane_scale = clamp(airplane_scale + DescentSpeed, MinScale, MaxScale);
    } else {
        // Descender cuando no se mueve
        airplane_scale = clamp(airplane_scale - DescentSpeed, MinScale, MaxScale);
        
        // Comprobar si estamos descendiendo
        if(airplane_scale <= MinScale) {
            // Si estamos sobre el portaaviones, aterrizaje exitoso
            if(is_over_carrier()) {
                // Aquí podrías añadir lógica adicional para el aterrizaje exitoso
                airplane_scale = MinScale;
            } else {
                // Game over si caemos al mar
                game_state = StateGameOver;
            }
        }
    }
    
    // Mantener el avión dentro del mundo
    airplane_x = clamp(airplane_x, 0, WorldWidth);
    airplane_y = clamp(airplane_y, 0, WorldHeight);
    
    // Actualizar la posición de la cámara
    update_camera();
}

void render_sea() {
    // Color azul oscuro para el mar visto desde arriba (ABGR format)
    clear_screen(SeaColor);
}

void render_airplane() {
    // Dibujar el mar
    render_sea();
    
    // Dibujar el portaaviones (ajustado a la posición de la cámara)
    select_texture(0);
    select_region(RegionCarrier);
    draw_region_at(CarrierX - camera_x, CarrierY - camera_y);
    
    // Calcular posición en pantalla del avión
    float screen_x = airplane_x - camera_x;
    float screen_y = airplane_y - camera_y;
    
    // Dibujar la sombra del avión
    select_region(RegionAirplaneShadow);
    set_multiply_color(ShadowColor);
    set_drawing_scale(airplane_scale, airplane_scale);
    set_drawing_angle(airplane_angle);
    draw_region_rotozoomed_at(screen_x + AirplaneShadowOffset, 
                             screen_y + AirplaneShadowOffset);
    
    // Dibujar el avión
    set_multiply_color(TextColor); // Resetear el color a blanco opaco
    select_region(RegionAirplane);
    set_drawing_scale(airplane_scale, airplane_scale);
    set_drawing_angle(airplane_angle);
    draw_region_rotozoomed_at(screen_x, screen_y);
}