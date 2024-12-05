#include "video.h"
#include "input.h"
#include "time.h"
#include "utils/definitions.h"
#include "game/airplane.h"
#include "game/island.h"
#include "game/render_system.h"
#include "game/soldier.h"
#include "game/events.h"
#include "game/turret.h"
#include "game/dialog.h"
#include "game/bomb.h"
#include "game/enemy.h"
#include "states/menu.h"
#include "states/gameover.h"

// Incluir las implementaciones
#include "game/airplane.c"
#include "game/island.c"
#include "game/render_system.c"
#include "game/soldier.c"
#include "game/weapon.c"
#include "game/pickup.c"
#include "game/turret.c"
#include "game/bomb.c"
#include "game/enemy.c"
#include "game/dialog.c"
#include "game/events.c"
#include "states/menu.c"
#include "utils/utilities.c"
#include "states/gameover.c"
#include "game/bullet.c"

// Variable global para el estado del juego
int game_state;
float camera_zoom = 1.0;
float target_zoom = 1.0;

void main()
{
    // Inicializar sistemas en orden
    initialize_carrier();  // Inicializar el carrier primero
    initialize_islands();  // Luego las islas
    initialize_airplane(); // El avión
    initialize_soldier();  // El soldado
    initialize_weapons();  // Las armas
    initialize_pickups();  // Los pickups
    initialize_events();
    initialize_turrets();
    initialize_bombs();
    initialize_enemies();
    initialize_portraits();
    initialize_dialog();

    // Establecer el estado inicial
    game_state = StateMenu;

    // Bucle principal del juego
    while (true)
    {
        clear_screen(BackgroundColor);

        // Actualizar y renderizar según el estado actual
        switch (game_state)
        {
        case StateMenu:
            update_menu();
            render_menu();
            break;

        case StateGame:
            if (!dialog_active)
            {
                if (!is_player_in_vehicle)
                {
                    update_soldier();
                }
                else
                {
                    update_airplane();
                }

                update_turrets();
                update_pickups();
                update_camera_zoom();
                update_bullets();
                update_enemies();
                update_bombs();
            }

            render_world(camera_x, camera_y);
            render_turrets();
            render_bullets();
            render_airplane();
            render_pickups();
            render_enemies();
            render_bombs();

            update_dialog();

            if (!is_player_in_vehicle)
            {
                render_soldier();
                render_soldier_ui();

                /*if (!has_event_happened(0))
                {
                    queue_dialog("Teniente Rykov, inicie operacion 'Nemesis'\nSu mision es clara: \n- Elimine las torretas de defensa\nLa Mano Negra no debe activar su prototipo\nBuena suerte.", TexturePortraitCommander);
                    queue_dialog("Entendido, comandante. Entrando en zona \nhostil. Es hora de demostrarles quien \ndomina estos cielos.", TexturePortraitPlayer);
                    start_dialog_sequence();

                    mark_event_as_happened(0);
                }

                if (!has_event_happened(1))
                {
                    queue_dialog("Primera torreta destruida. Esto va a ser \npan comido... ¿o no?", TexturePortraitPlayer);
                    queue_dialog("Buen trabajo, Rykov. Pero cuidado, hemos \ndetectado actividad enemiga. Prepárese \npara recibir resistencia", TexturePortraitCommander);
                    queue_dialog("¡El enemigo ha penetrado las defensas! \n¡No dejen que avance!", TexturePortraitSoldier);
                    start_dialog_sequence();

                    mark_event_as_happened(1);
                }
                if (!has_event_happened(2))
                {
                    queue_dialog("Atención, Rykov. Tenemos cazas enemigos \nsaliendo de la base aérea. \n¡Prepárese para un combate intenso!", TexturePortraitCommander);
                    queue_dialog("Lo sabía, no podían quedarse quietos por \nmucho tiempo. Vamos, pongan a prueba mi \npuntería.", TexturePortraitPlayer);
                    queue_dialog("¡Caza Alpha 1 en posición! Objetivo \nvisualizado, todos los cazas: \n¡ataquen al helicóptero!", TexturePortraitSoldier);
                    start_dialog_sequence();

                    mark_event_as_happened(2);
                }
                if (!has_event_happened(3))
                {
                    queue_dialog("Eso es todo, chicos. Vuelvan a casa... o \nlo que quede de ella", TexturePortraitPlayer);
                    queue_dialog("Buen trabajo, Rykov. Pero no baje la \nguardia. Las torretas restantes todavía \nestán activas y el enemigo no se \nrendirá sin pelear", TexturePortraitCommander);
                    queue_dialog("Nunca lo hacen. Vamos a \nterminar con esto.", TexturePortraitPlayer);
                    start_dialog_sequence();

                    mark_event_as_happened(3);
                }

                if (!has_event_happened(4))
                {
                    queue_dialog("¡Alerta máxima, Rykov! El prototipo está \nen el aire. Repito: ¡el prototipo ha \ndespegado!", TexturePortraitCommander);
                    queue_dialog("Así que aquí estás... el gran monstruo \nque todos temían. Espero que valgas la \npena.", TexturePortraitPlayer);
                    queue_dialog("Aquí Capitán Kross, piloto del Fénix \nOscuro. Este será tu último vuelo, \nintruso.", TexturePortraitBoss);
                    queue_dialog("Eso ya lo veremos, Kross. Solo un piloto \nsaldrá vivo de esto, y no serás tú.", TexturePortraitPlayer);
                    start_dialog_sequence();

                    mark_event_as_happened(4);
                }*/
            }

            if (dialog_active)
            {
                render_dialog();
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