#include "island.h"
#include "../utils/definitions.h"
#include "video.h"
#include "misc.h"
#include "string.h"
#include "time.h"

// Variables globales
float[MaxIslands] island_x;
float[MaxIslands] island_y;
int[MaxIslands][MaxTilesX][MaxTilesY] island_tiles;
int num_islands;

int get_tile(int island_index, int x, int y) {
    if(x < 0 || x >= MaxTilesX || y < 0 || y >= MaxTilesY) {
        return TileEmpty;
    }
    return island_tiles[island_index][x][y];
}

void generate_island_layout(int island_index) {
    int center_x = MaxTilesX / 2;
    int center_y = MaxTilesY / 2;
    int radius = 2 + (rand() % 3);
    
    // Limpiar tiles
    for(int y = 0; y < MaxTilesY; y++) {
        for(int x = 0; x < MaxTilesX; x++) {
            island_tiles[island_index][x][y] = TileEmpty;
        }
    }
    
    // Generar forma base usando solo TileGround
    for(int y = center_y - radius; y <= center_y + radius; y++) {
        for(int x = center_x - radius; x <= center_x + radius; x++) {
            if(x >= 0 && x < MaxTilesX && y >= 0 && y < MaxTilesY) {
                int dx = x - center_x;
                int dy = y - center_y;
                if(dx*dx + dy*dy <= radius*radius + (rand() % 2)) {
                    island_tiles[island_index][x][y] = TileGround;
                }
            }
        }
    }
}

void initialize_islands() {
    srand(get_time());
    
    select_texture(TextureIsland);
    select_region(TileGround);
    define_region(0, 0, TileSize, TileSize, TileSize/2, TileSize/2);
    
    num_islands = MaxIslands;
    
    float safe_radius = TileSize * MaxTilesX * 2; // Radio de seguridad para separación
    float carrier_safe_zone = 500; // Zona segura alrededor del carrier
    
    for(int i = 0; i < num_islands; i++) {
        bool valid_position = false;
        float x, y;
        
        while(!valid_position) {
            x = rand() % (int)(WorldWidth * 0.8);  // Usar solo 80% del mundo
            y = rand() % (int)(WorldHeight * 0.8);
            
            // Comprobar distancia al carrier
            float dx = x - StartingX;
            float dy = y - StartingY;
            float carrier_dist = sqrt(dx*dx + dy*dy);
            
            // Comprobar distancia a otras islas
            bool too_close = false;
            for(int j = 0; j < i; j++) {
                dx = x - island_x[j];
                dy = y - island_y[j];
                float island_dist = sqrt(dx*dx + dy*dy);
                if(island_dist < safe_radius) {
                    too_close = true;
                    break;
                }
            }
            
            // Posición válida si está lejos del carrier y otras islas
            if(carrier_dist > carrier_safe_zone && !too_close) {
                valid_position = true;
                island_x[i] = x;
                island_y[i] = y;
            }
        }
        
        generate_island_layout(i);
    }
}

void render_islands(float camera_x, float camera_y) {
    select_texture(TextureIsland);
    
    for(int i = 0; i < num_islands; i++) {
        float base_x = island_x[i];
        float base_y = island_y[i];
        
        // Comprobar si la isla está en el área visible
        float screen_x = base_x - camera_x;
        float screen_y = base_y - camera_y;
        
        // Solo procesar la isla si está en el área visible
        if(screen_x >= -TileSize * MaxTilesX && 
           screen_x <= ScreenWidth + TileSize * MaxTilesX &&
           screen_y >= -TileSize * MaxTilesY && 
           screen_y <= ScreenHeight + TileSize * MaxTilesY) {
            
            // Dibujar cada tile de la isla
            for(int y = 0; y < MaxTilesY; y++) {
                for(int x = 0; x < MaxTilesX; x++) {
                    if(island_tiles[i][x][y] != TileEmpty) {
                        float tile_x = base_x + (x * TileSize) - camera_x;
                        float tile_y = base_y + (y * TileSize) - camera_y;
                        
                        select_region(TileGround);  // Por ahora solo usamos el tile ground
                        draw_region_at(tile_x, tile_y);
                    }
                }
            }
        }
    }
}

int is_over_island(float x, float y) {
    for(int i = 0; i < num_islands; i++) {
        float local_x = (x - island_x[i]) / TileSize;
        float local_y = (y - island_y[i]) / TileSize;
        
        if(local_x >= 0 && local_x < MaxTilesX && 
           local_y >= 0 && local_y < MaxTilesY) {
            int tile_x = local_x;
            int tile_y = local_y;
            if(island_tiles[i][tile_x][tile_y] != TileEmpty) {
                return 1;
            }
        }
    }
    return 0;
}