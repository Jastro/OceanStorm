#include "audio.h"

#include "utils/definitions.h"
#include "game/worldmap.h"
#include "game/heli.h"
#include "game/island.h"
#include "game/render_system.h"
#include "game/soldier.h"
#include "game/corpse.h"
#include "game/events.h"
#include "game/turret.h"
#include "game/dialog.h"
#include "game/dialogtexts.h"
#include "game/bomb.h"
#include "game/enemy.h"
#include "states/menu.h"
#include "states/gameover.h"

// Incluir las implementaciones
#include "game/worldmap.c"
#include "game/heli.c"
#include "game/island.c"
#include "game/island_maps.c"
#include "game/render_system.c"
#include "game/soldier.c"
#include "game/corpse.c"
#include "game/weapon.c"
#include "game/pickup.c"
#include "game/turret.c"
#include "game/bomb.c"
#include "game/enemy.c"
#include "game/dialog.c"
#include "game/dialogtexts.c"
#include "game/events.c"
#include "states/menu.c"
#include "utils/utilities.c"
#include "states/gameover.c"
#include "game/bullet.c"

// Variable global para el estado del juego
int game_state;
int game_language = LanguageEnglish;
float camera_zoom = 1.0;
float target_zoom = 1.0;

void main()
{

    for (int i = 0; i < 15; i++)
    {
        select_channel(i);
        set_channel_volume(0.3);
    }

    select_channel(ChannelMusic);
    set_channel_volume(0.4);

    // Inicializar sistemas en orden
    initialize_world();   // Crear nuestro tileset y tilemap
    initialize_carrier(); // Inicializar el carrier primero

    // La generacion de islas puede no encontrar
    // solucion y atascarse; si es asi la reiniciamos
    // para que pruebe distintas soluciones
    while (!initialize_islands())
    { /* bucle vacio */
    };

    initialize_heli();    // El avión
    initialize_soldier(); // El soldado
    initialize_bullets();
    initialize_weapons(); // Las armas
    initialize_pickups(); // Los pickups
    initialize_events();
    initialize_turrets();
    initialize_bombs();
    initialize_enemies();
    initialize_corpses();
    initialize_portraits();
    initialize_dialog();
    initialize_minimap();
    initialize_gui();

    // Establecer el estado inicial
    initialize_menu();
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
                    update_heli();
                }

                update_screen_shake();
                update_turrets();
                update_pickups();
                update_camera_zoom();
                update_bullets();
                update_enemies();
                update_corpses();
                update_bombs();
            }

            render_world();
            render_turrets();
            render_corpses();
            render_bullets();
            render_pickups();
            render_enemies();
            render_bombs();
            render_heli();
            render_gui();
            update_dialog();
            starting_dialog();

            if (!is_player_in_vehicle)
            {
                render_soldier();
            }

            if (dialog_active)
            {
                render_dialog();
            }
            break;

        case StateGameOver:
            update_dialog();
            starting_dialog();
            update_gameover();
            if (dialog_active)
            {
                render_dialog();
            }
            else
            {
                render_gameover();
            }

            break;
        }

        // Esperar al siguiente frame
        end_frame();
    }
}