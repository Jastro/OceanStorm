#include "video.h"
#include "input.h"
#include "time.h"
#include "utils/definitions.h"
#include "game/airplane.h"
#include "states/menu.h"
#include "states/gameover.h"

// Incluir las implementaciones
#include "game/airplane.c"
#include "states/menu.c"
#include "utils/utilities.c"
#include "states/gameover.c"

// Variable global para el estado del juego
int game_state;

void main() {
    // Inicializar el juego
    initialize_airplane();
    game_state = StateMenu;
    
    // Bucle principal del juego
    while(true) {
        // Actualizar y renderizar según el estado actual
        switch(game_state) {
            case StateMenu:
                update_menu();
                render_menu();
                break;
            
            case StateGame:
                update_airplane();
                render_airplane();
                break;
            
            case StateGameOver:
                update_gameover();
                render_gameover();
                break;
        }
        
        // Esperar al siguiente frame
        end_frame();
    }
}