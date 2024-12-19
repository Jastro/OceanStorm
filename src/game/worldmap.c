#include "worldmap.h"

// Variables para manejar el mapa
int[WorldTilesY][WorldTilesX] world_grid;
tileset world_tiles;
tilemap world_map;
float screen_shake_time = 0;
float screen_shake_angle = 0;

void update_screen_shake()
{
    if (screen_shake_time > 0)
    {
        // Actualizar tiempo
        screen_shake_time -= 1.0 / 60.0;

        // Calcular offset basado en seno
        screen_shake_angle += pi / 4; // Velocidad de oscilación

        // La intensidad se reduce conforme pasa el tiempo
        float current_intensity = ShakeIntensity * (screen_shake_time / ShakeTimeOnDamage);

        // Aplicar offset a la cámara
        float offset = sin(screen_shake_angle) * current_intensity;
        world_map.camera_position.x += offset;
        world_map.camera_position.y += offset / 2; // Menos movimiento vertical

        // Si el tiempo se acabó, asegurar que la cámara vuelve a su posición
        if (screen_shake_time <= 0)
        {
            screen_shake_time = 0;
            screen_shake_angle = 0;
        }
    }
}

void trigger_screen_shake()
{
    screen_shake_time = ShakeTimeOnDamage;
    screen_shake_angle = 0;
    play_sound(SoundHurt);
}

void initialize_world()
{
    // - - - - - - - - - - - - - - - - - - -
    // definir nuestro tileset

    // define dimensions for our tiles
    world_tiles.width = TileSize;
    world_tiles.height = TileSize;

    // adjacent tiles have neither space nor overlap
    world_tiles.gap_x = 0;
    world_tiles.gap_y = 0;

    // define texture and regions for our tiles
    world_tiles.texture_id = TextureIsland;
    world_tiles.tile_zero_region_id = TileEmpty;

    // this particular tile set makes use of its
    // first tile (it's not discarded as transparent)
    world_tiles.draw_tile_zero = true;

    // - - - - - - - - - - - - - - - - - - -
    // 2.2: definir nuestro tilemap

    // assign our tiles to the map
    world_map.tiles = &world_tiles;

    // define the map array
    world_map.map = &world_grid[0][0];
    world_map.map_width = WorldTilesX;
    world_map.map_height = WorldTilesY;
}
