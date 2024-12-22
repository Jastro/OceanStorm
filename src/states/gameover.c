#include "gameover.h"
#include "../utils/definitions.h"
#include "../game/heli.h"

extern int game_state;

void update_gameover() {
    // Volver al menú si se presiona START
    if(gamepad_button_start() == 1) {
        reset_heli();  // Reiniciar el estado del avión
        reset_soldier();
        game_state = StateGame;
        
        if (!has_event_happened(GameRestart))
        {
            queue_dialog(&DW_GameRestart);
            queue_dialog(&DW_GameRestartReply);
            start_dialog_sequence();

            mark_event_as_happened(GameRestart);
        }
    }
}

void render_gameover() {
    // Limpiar la pantalla con negro opaco
    clear_screen(BackgroundColor);
    
    // Seleccionar la fuente de la BIOS y color rojo
    select_texture(-1);
    set_multiply_color(RedColor);
    
    // Mostrar mensaje de game over
    print_at(ScreenCenterX - 45, ScreenCenterY - 20, "GAME OVER");
    
    // Hacer parpadear el texto usando el contador de frames
    if((get_frame_counter() / BlinkRate) % 2 == 0) {
        set_multiply_color(TextColor);
        print_at(ScreenCenterX - 115, ScreenCenterY + 20, "PRESS START TO CONTINUE");
    }
    set_multiply_color(color_white);
}