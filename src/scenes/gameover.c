#include "gameover.h"
#include "../utils/definitions.h"
#include "../game/heli.h"

extern int game_scene;

void update_gameover() {
    // Volver al menú si se presiona START
    if(gamepad_button_start() == 1) {
        reset_heli();  // Reiniciar el estado del avión
        reset_soldier();
        game_scene = SceneGame;
        
        if (!has_event_happened(GameRestart))
        {
            queue_dialog(&DW_GameRestart);
            queue_dialog(&DW_GameRestartReply);
            start_dialog_sequence();

            mark_event_as_happened(GameRestart);
        }
    }
}

int[2][15] TextsGameOver =
{
    "  GAME OVER",
    "FIN DEL JUEGO"
};

int[2][30] TextsContinue =
{
    " PRESS START TO CONTINUE",
    "PULSA START PARA CONTINUAR"
};


void render_gameover() {
    // Limpiar la pantalla con negro opaco
    clear_screen(BackgroundColor);
    
    // Seleccionar la fuente de la BIOS y color rojo
    select_texture(-1);
    set_multiply_color(RedColor);
    
    // Mostrar mensaje de game over
    print_at(ScreenCenterX - 65, ScreenCenterY - 20, TextsGameOver[game_language]);
    
    // Hacer parpadear el texto usando el contador de frames
    if((get_frame_counter() / BlinkRate) % 2 == 0) {
        set_multiply_color(TextColor);
        print_at(ScreenCenterX - 125, ScreenCenterY + 20, TextsContinue[game_language]);
    }
    set_multiply_color(color_white);
}