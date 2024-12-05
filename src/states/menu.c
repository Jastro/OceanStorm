#include "menu.h"
#include "../utils/definitions.h"
#include "video.h"
#include "input.h"
#include "time.h"

// Variable externa para el estado del juego
extern int game_state;

void update_menu() {
    // Si se presiona START, cambiar al estado de juego
    if(gamepad_button_start() == 1) {
        game_state = StateGame;
    }
}

void render_menu() {
    // Limpiar la pantalla con negro opaco
    clear_screen(BackgroundColor);
    
    // Seleccionar la fuente de la BIOS
    select_texture(-1);
    set_multiply_color(TextColor);
    
    // Dibujar el título centrado
    print_at(ScreenCenterX - 60, ScreenCenterY - 40, "OCEAN STORM");
    
    // Hacer parpadear el texto usando el contador de frames
    if((get_frame_counter() / BlinkRate) % 2 == 0) {
        print_at(ScreenCenterX - 90, ScreenCenterY + 20, "PRESS START TO BEGIN");
    }
}