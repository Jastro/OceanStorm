#include "video.h"
#include "input.h"
#include "time.h"
#include "utils/definitions.h"
#include "game/airplane.h"
#include "game/island.h"
#include "game/render_system.h"
#include "game/soldier.h"
#include "states/menu.h"
#include "states/gameover.h"

// Incluir las implementaciones
#include "game/airplane.c"
#include "game/island.c"
#include "game/render_system.c"
#include "game/soldier.c"
#include "game/weapon.c"
#include "game/pickup.c"
#include "states/menu.c"
#include "utils/utilities.c"
#include "states/gameover.c"

// Variable global para el estado del juego
int game_state;
float camera_zoom = 1.0;
float target_zoom = 1.0;

void main() {
    // Inicializar sistemas en orden
    initialize_carrier();      // Inicializar el carrier primero
    initialize_islands();      // Luego las islas
    initialize_airplane();     // El avión
    initialize_soldier();      // El soldado
    initialize_weapons();      // Las armas
    initialize_pickups();      // Los pickups
    
    // Establecer el estado inicial
    game_state = StateMenu;
    
    // Bucle principal del juego
    while(true) {
        clear_screen(BackgroundColor);

        // Actualizar y renderizar según el estado actual
        switch(game_state) {
            case StateMenu:
                update_menu();
                render_menu();
                break;
            
            case StateGame:
                // Actualización
                update_airplane();
                update_soldier();
                update_pickups();
                update_camera_zoom();
                
                // Renderizado
                render_world(camera_x, camera_y);
                
                if(is_player_in_vehicle) {
                    render_airplane();
                } else {
                    render_soldier();
                    render_soldier_ui();  // Solo UI cuando es soldado
                    render_pickups();
                }
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