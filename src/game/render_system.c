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
        print_at(10, ObjectiveY, "TORRETAS: ");
        itoa(num_active_turrets(), number_buffer, 10);
        print_at(100, ObjectiveY, number_buffer);
        print_at(110, ObjectiveY, "/");
        itoa(MaxTurrets, number_buffer, 10);
        print_at(120, ObjectiveY, number_buffer);
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

    // Al dibujar el heli mostramos la direccion done mira
    select_region(RegionMapHeli);
    set_drawing_angle(heli_angle);
    draw_region_rotated_at(minimap_x(heli_x), minimap_y(heli_y));
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

    // 3. Dibujar el minimapa
    render_minimap();

    // 4. Dibujar objetivos actuales
    render_objectives();
}
