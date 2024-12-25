#include "game_control.h"

// Se llama solo una vez, al iniciar el juego
void initialize_game()
{
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
    initialize_enemies();
    initialize_corpses();
    initialize_portraits();
    initialize_dialog();
    initialize_minimap();
    initialize_gui();
    initialize_fx();
    // Inicializar escenas concretas
    initialize_menu();
    initialize_ending();
}

// Se llama al inicio y tras cada reinicio del juego
void reset_game()
{
    // Detener cualquier sonido previo
    stop_all_channels();

    // reiniciar las variables globales
    num_active_enemies = 0;
    num_total_enemies = 0;
    phase = 0;
    phase_time = 0;

    // reiniciar cada elemento del juego
    reset_soldier();
    reset_heli();
    reset_dialog();
    reset_bullets();
    reset_enemies();
    reset_turrets();
    reset_corpses();
    reset_bombs();
    reset_weapons();
    reset_pickups();
    reset_fx();
    reset_screen_shake();

    // NO reiniciamos los eventos (para no
    // volver a mostrar los mismos diálogos)
}

void kill_player()
{
    // Sonido de muerte, sin musica
    stop_all_channels();
    play_sound(SoundDeath);
    
    // Dialogo tras la primera muerte
    if (!has_event_happened(GameOver))
    {
        queue_dialog(&DW_GameOver);
        start_dialog_sequence();

        mark_event_as_happened(GameOver);
    }

    // Ir a escena game over
    game_scene = SceneGameOver;
}

void begin_game()
{
    // Musica del inicio del juego
    stop_all_channels();
    play_sound_in_channel(MusicTurrets, ChannelMusic);
    
    // Ir a escena juego
    game_scene = SceneGame;
}
void begin_planes()
{
    // Musica del inicio del juego
    stop_all_channels();
    play_sound_in_channel(MusicPlanes, ChannelMusic);
    
}
void begin_boss()
{
    // Musica del inicio del juego
    stop_all_channels();
    play_sound_in_channel(MusicBoss, ChannelMusic);
    
}

void begin_menu()
{
    // Musica del menu
    stop_all_channels();
    play_sound_in_channel(MusicMenu, ChannelMusic);

    // Ir a escena menu
    game_scene = SceneMenu;
}

void begin_ending()
{
    // Musica del ending
    stop_all_channels();
    play_sound_in_channel(MusicEnding, ChannelMusic);

    // Ir a escena ending
    game_scene = SceneEnding;
}
