#include "render_system.h"
#include "../utils/definitions.h"
#include "video.h"
#include "island.h"
#include "string.h"

// Variables externas de island.h
extern float[MaxIslands] island_x;
extern float[MaxIslands] island_y;
extern int[MaxIslands][MaxTilesX][MaxTilesY] island_tiles;
extern int num_islands;

void initialize_battleShip()
{
    select_texture(TextureBattleship);
    select_region(0);
    define_region(0, 0, BattleShipSpriteWidth, BattleShipSpriteHeight, BattleShipSpriteWidth / 2, BattleShipSpriteHeight / 2);
}
void initialize_carrier()
{
    select_texture(TextureCarrier);
    select_region(RegionCarrier);
    define_region(0, 0, CarrierWidth, CarrierHeight, CarrierWidth / 2, CarrierHeight / 2);
}
void initialize_dialog()
{
    select_texture(TextureDialog);
    select_region(0);
    define_region(0, 0, UIDialogFrameWidth, UIDialogFrameHeight, UIDialogFrameWidth / 2, UIDialogFrameHeight / 2);
}
void initialize_portraits()
{
    select_texture(TexturePortraitPlayer);
    select_region(0);
    define_region(0, 0, UIDialogPortraitSize, UIDialogPortraitSize, UIDialogPortraitSize / 2, UIDialogPortraitSize / 2);

    select_texture(TexturePortraitCommander);
    select_region(0);
    define_region(0, 0, UIDialogPortraitSize, UIDialogPortraitSize, UIDialogPortraitSize / 2, UIDialogPortraitSize / 2);

    select_texture(TexturePortraitSoldier);
    select_region(0);
    define_region(0, 0, UIDialogPortraitSize, UIDialogPortraitSize, UIDialogPortraitSize / 2, UIDialogPortraitSize / 2);

    select_texture(TexturePortraitBoss);
    select_region(0);
    define_region(0, 0, UIDialogPortraitSize, UIDialogPortraitSize, UIDialogPortraitSize / 2, UIDialogPortraitSize / 2);
}

void render_objectives()
{
    select_texture(-1);
    set_multiply_color(TextColor);

    // Buffer para números
    int[12] number_buffer;

    // Mostrar objetivos según la fase
    switch (phase)
    {
    case 0: // Fase inicial - Destruir torretas
        print_at(10, ObjectiveY, "TORRETAS: ");
        itoa(num_active_turrets(), number_buffer, 10);
        print_at(100, ObjectiveY, number_buffer);
        print_at(120, ObjectiveY, "/");
        itoa(MaxTurrets, number_buffer, 10);
        print_at(140, ObjectiveY, number_buffer);
        break;

    case 1: // Fase de enemigos aéreos
        print_at(10, ObjectiveY, "ENEMIGOS: ");
        itoa(num_active_enemies, number_buffer, 10);
        print_at(100, ObjectiveY, number_buffer);
        break;

    case 2: // Fase final de torretas
        print_at(10, ObjectiveY, "TORRETAS RESTANTES: ");
        itoa(num_active_turrets(), number_buffer, 10);
        print_at(170, ObjectiveY, number_buffer);
        break;

    case 3:                           // Fase del boss
        set_multiply_color(RedColor); // El objetivo del boss en rojo
        print_at(10, ObjectiveY, "!DERROTA AL BOSS!");
        break;
    }

    if (phase_time < 2.0)
    {
        if ((int)(phase_time * 4) % 2 == 0)
        { // Parpadeo cada 0.25 segundos
            set_multiply_color(RedColor);
        }
    }
}

void starting_dialog()
{
    if (!has_event_happened(StartGame))
    {
        queue_dialog(DT_StartOperation, TexturePortraitCommander);
        queue_dialog(DT_StartOperationReply, TexturePortraitPlayer);
        start_dialog_sequence();

        mark_event_as_happened(StartGame);
    }
}

void render_world(float camera_x, float camera_y)
{
    // 1. Limpiar pantalla con el mar
    clear_screen(SeaColor);
    set_multiply_color(color_white);

    // 2. Dibujar el portaaviones
    select_texture(TextureCarrier);
    select_region(RegionCarrier);
    draw_region_at(StartingX - camera_x, StartingY - camera_y);

    // 3. Dibujar las islas
    render_islands(camera_x, camera_y);

    // 4. Dibujar objetivos actuales
    render_objectives();

    select_texture(TextureBattleship);
    draw_region_at(StartingX + 10, StartingY);
}