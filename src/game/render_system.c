#include "render_system.h"
#include "../utils/definitions.h"
#include "island.h"

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
    select_texture(TexturePortraits);
    
    define_region_matrix
    (
        RegionPortraitPlayer,                               // ID de la primera region
        0, 0,                                               // X e Y minimas primera region
        UIDialogPortraitSize-1, UIDialogPortraitSize-1,     // X e Y maximas primera region
        UIDialogPortraitSize/2, UIDialogPortraitSize/2,     // X e Y de referencia primera region
        4, 10,                                              // dimensiones matrix (columnas, filas)
        0                                                   // separacion entre regiones en pixels
    );
}

void initialize_gui() 
{
    select_texture( TextureGui );
    select_region( RegionHeliFuel );
    define_region( 1,1,  127,21,  1,1 );
    select_region( RegionHeliHealth );
    define_region( 1,25,  127,45,  1,25 );
    select_region( RegionHeliAmmo );
    define_region( 1,49,  127,88,  1,49 );
    select_region( RegionSoldierAmmo );
    define_region( 1,92,  107,131,  1,92 );
    select_region( RegionSoldierBombs );
    define_region( 1,135,  64,173,  1,135 );
    select_region( RegionSoldierArmor );
    define_region( 1,177,  64,215,  1,177 );
    select_region( RegionObjectiveTurrets );
    define_region( 131,1,  247,42,  189,1 );
    select_region( RegionObjectivePlanes );
    define_region( 131,46,  247,87,  189,46 );
    select_region( RegionObjectiveBoss );
    define_region( 131,91,  247,132,  189,91 );
    select_region( RegionEnterSign );
    define_region( 68,136,  134,161,  102,148 );
    select_region( RegionExitSign );
    define_region( 68,165,  134,190,  103,177 );
    select_region( RegionSoldierHealth );
    define_region( 138,136,  197,147,  138,136 );
    select_region( RegionSoldierReloading );
    define_region( 138,151,  197,162,  138,151 );
}

void render_objectives()
{
    // Parpadeo inicial cada 0.25 segundos
    if (phase_time < 2.0)
        if ((int)(phase_time * 4) % 2 == 0)
            return;
    
    select_texture(TextureGui);
    
    // Mostrar objetivos según la fase
    switch (phase)
    {
        // Estos 2 casos se dibujan igual
        case 0: // Fase inicial - Destruir torretas
        case 2: // Fase final de torretas
            select_region(RegionObjectiveTurrets);
            draw_region_at(screen_width/2, 5);
            print_1digit_at(319, 9, num_active_turrets());
            print_1digit_at(352, 9, MaxTurrets);
            break;

        case 1: // Fase de enemigos aéreos
            select_region(RegionObjectivePlanes);
            draw_region_at(screen_width/2, 5);
            print_1digit_at(319, 9, num_active_enemies);
            print_1digit_at(352, 9, num_total_enemies);
            break;

        case 3: // Fase del boss
            select_region(RegionObjectiveBoss);
            draw_region_at(screen_width/2, 5);
            break;
    }
}

void starting_dialog()
{
    if (!has_event_happened(StartGame))
    {
        queue_dialog(&DW_StartOperation);
        queue_dialog(&DW_StartOperationReply);
        start_dialog_sequence();

        mark_event_as_happened(StartGame);
    }
}

void initialize_minimap()
{
    select_texture(TextureMinimap);
    select_region(RegionMapFrame);
    define_region_topleft(1, 1, 102, 102);
    select_region(RegionMapSmallIsland);
    define_region_topleft(107, 1, 124, 18);
    select_region(RegionMapLargeIsland);
    define_region_topleft(107, 23, 124, 40);
    select_region(RegionMapCarrier);
    define_region_center(107, 45, 112, 56);
    select_region(RegionMapHeli);
    define_region_center(117, 45, 123, 54);
    select_region(RegionMapSoldier);
    define_region_center(107, 61, 113, 67);
    select_region(RegionMapEnemyPlane);
    define_region_center(107, 72, 113, 81);
}

// funciones auxiliares para convertir coordenadas
// del mundo a coordenadad de minimapa para dibujar
#define minimap_scale_factor ((float)MinimapTileSize / TileSize)

int minimap_x(float world_x)
{
    return 539 + minimap_scale_factor * world_x;
}

int minimap_y(float world_y)
{
    return 6 + minimap_scale_factor * world_y;
}

void render_minimap()
{
    select_texture(TextureMinimap);
    select_region(RegionMapFrame);
    draw_region_at(535, 3);

    // Factor de escala para transformar las
    // coordenadas del mundo en coordenadas de minimapa
    float Scale = (float)MinimapTileSize / TileSize;
    int MapMinX = 538;
    int MapMinY = 6;

    select_region(RegionMapCarrier);
    draw_region_at(minimap_x(StartingX), minimap_y(StartingY));

    // Dibujar todas las islas, distinguiendo solo
    // si son grandes o pequeñas, pero no su forma
    for (int i = 0; i < num_islands; i++)
    {
        bool is_large = (island_radius[i] > TileSize * 4);
        select_region(RegionMapSmallIsland + is_large);
        draw_region_at(minimap_x(island_x[i]), minimap_y(island_y[i]));
    }

    // Dibujar todos los enemigos antes para que nunca nos tapen
    select_region(RegionMapEnemyPlane);
    
    for (int i = 0; i < MaxEnemies; i++)
    {
        if(!enemy_active[i])
            continue;
        
        // No dibujamos soldados, solo vehiculos
        if(enemy_type[i] == EnemyTypeSoldier)
            continue;
        
        // Los vehiculos los dibujamos mostrando a donde miran
        set_drawing_angle(enemy_angle[i] + pi/2);
        draw_region_rotated_at(minimap_x(enemy_x[i]), minimap_y(enemy_y[i]));
    }
    
    // difuminar el heli si estamos fuera para que no nos estorbe
    if(!is_player_in_vehicle)
        set_multiply_color(0x80FFFFFF);
    
    // Al dibujar el heli mostramos la direccion done mira
    select_region(RegionMapHeli);
    set_drawing_angle(heli_angle);
    draw_region_rotated_at(minimap_x(heli_x), minimap_y(heli_y));
    set_multiply_color(color_white);
    
    // Dibujar nuestro soldado si vamos a pie
    if(!is_player_in_vehicle)
    {
        select_region(RegionMapSoldier);
        draw_region_at(minimap_x(soldier_x), minimap_y(soldier_y));
    }
}

void render_world()
{
    // restringimos la posicion de la camara para que,
    // incluso si el heli esta cerca del borde del mapa,
    // la pantalla nunca muestre nada fuera de los limites
    tilemap_clip_camera_position( &world_map );
    
    // dibujar el tilemap teniendo en cuenta la camara
    tilemap_draw_from_camera( &world_map );
    
    // 2. Dibujar el portaaviones
    select_texture(TextureCarrier);
    select_region(RegionCarrier);
    tilemap_draw_region(&world_map, StartingX, StartingY);
}

// El GUI tiene que dibujarse por separado para
// asegurar que está encima de los demás elementos
void render_gui()
{
    // 1. Dibujar el minimapa
    render_minimap();

    // 2. Dibujar objetivos actuales
    render_objectives();
    
    // 3. Dibujar estado del soldado o heli según proceda
    if (is_player_in_vehicle)
        render_heli_gui();
    else
        render_soldier_gui();
}
