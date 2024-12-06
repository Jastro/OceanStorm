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
#include "game/dialogtexts.h"
#include "game/bomb.h"
#include "game/enemy.h"
#include "states/menu.h"
#include "states/gameover.h"

// Incluir las implementaciones
#include "game/airplane.c"
#include "game/island.c"
#include "game/island_maps.c"
#include "game/render_system.c"
#include "game/soldier.c"
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
    initialize_minimap();

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
                    queue_dialog(DT_StartOperation, TexturePortraitCommander);
                    queue_dialog(DT_StartOperationReply, TexturePortraitPlayer);
                    start_dialog_sequence();

                    mark_event_as_happened(0);
                }

                if (!has_event_happened(1))
                {
                    queue_dialog(DT_TurretDestroyed, TexturePortraitPlayer);
                    queue_dialog(DT_EnemyDetected, TexturePortraitCommander);
                    queue_dialog(DT_EnemyPenetration, TexturePortraitSoldier);
                    start_dialog_sequence();

                    mark_event_as_happened(1);
                }
                if (!has_event_happened(2))
                {
                    queue_dialog(DT_EnemyFighters, TexturePortraitCommander);
                    queue_dialog(DT_EnemyFightersReply, TexturePortraitPlayer);
                    queue_dialog(DT_AttackHeli, TexturePortraitSoldier);
                    start_dialog_sequence();

                    mark_event_as_happened(2);
                }
                if (!has_event_happened(3))
                {
                    queue_dialog(DT_GoHome, TexturePortraitPlayer);
                    queue_dialog(DT_TurretsRemain, TexturePortraitCommander);
                    queue_dialog(DT_TurretsRemainReply, TexturePortraitPlayer);
                    start_dialog_sequence();

                    mark_event_as_happened(3);
                }

                if (!has_event_happened(4))
                {
                    queue_dialog(DT_MaxAlert, TexturePortraitCommander);
                    queue_dialog(DT_MaxAlertReply, TexturePortraitPlayer);
                    queue_dialog(DT_DarkPhoenix, TexturePortraitBoss);
                    queue_dialog(DT_DarkPhoenix_Reply, TexturePortraitPlayer);
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