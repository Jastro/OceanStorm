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
#include "game/fx.h"
#include "scenes/menu.h"
#include "scenes/gameover.h"
#include "scenes/ending.h"
#include "scenes/boss_explosion.h"
#include "game/game_control.h"

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
#include "game/fx.c"
#include "game/dialog.c"
#include "game/dialogtexts.c"
#include "game/events.c"
#include "scenes/menu.c"
#include "utils/utilities.c"
#include "scenes/gameover.c"
#include "scenes/ending.c"
#include "scenes/boss_explosion.c"
#include "game/bullet.c"
#include "game/game_control.c"

// Variable global para el estado del juego
int game_scene;
int game_language = LanguageEnglish;
bool blood_enabled = true;
float camera_zoom = 1.0;
float target_zoom = 1.0;

void main()
{
    // Configurar volumen de los canales
    for (int i = 0; i < 15; i++)
    {
        select_channel(i);
        set_channel_volume(0.35);
    }

    select_channel(ChannelMusic);
    set_channel_volume(0.35);
    
    // Activar bucle en todas las musicas
    select_sound(MusicMenu);
    set_sound_loop(true);
    select_sound(MusicTurrets);
    set_sound_loop(true);
    select_sound(MusicPlanes);
    set_sound_loop(true);
    select_sound(MusicBoss);
    set_sound_loop(true);
    select_sound(MusicEnding);
    set_sound_loop(true);
    
    // Inicializar sistemas del juego
    initialize_game();

    // Establecer el estado inicial
    reset_game();
    begin_menu();

    // Bucle principal del juego
    while (true)
    {
        clear_screen(BackgroundColor);

        // Actualizar y renderizar según el estado actual
        switch (game_scene)
        {
        case SceneMenu:
            update_menu();
            render_menu();
            break;

        case SceneGame:
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
                update_fx();
            }

            render_world();
            render_turrets();
            render_corpses();
            render_bullets();
            render_pickups();
            render_enemies();
            render_bombs();
            render_fx();
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

        case SceneBossExplosion:
            update_boss_explosion();
            render_boss_explosion();
            break;
            
        case SceneGameOver:
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

        case SceneEnding:
            update_ending();
            render_ending();
            break;
        }

        // Esperar al siguiente frame
        end_frame();
    }
}