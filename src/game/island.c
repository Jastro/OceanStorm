#include "island.h"
#include "../utils/definitions.h"
#include "video.h"
#include "misc.h"
#include "string.h"
#include "time.h"

// Variables globales
float[MaxIslands] island_x;
float[MaxIslands] island_y;
float[MaxIslands] island_radius;
int[MaxIslands][MaxTilesX][MaxTilesY] island_tiles;
int num_islands;

int get_tile(int island_index, int x, int y) {
    if(x < 0 || x >= MaxTilesX || y < 0 || y >= MaxTilesY) {
        return TileEmpty;
    }
    return island_tiles[island_index][x][y];
}

void generate_island_layout(int island_index, bool is_large) {
    
    // Limpiar tiles
    for(int y = 0; y < MaxTilesY; y++) {
        for(int x = 0; x < MaxTilesX; x++) {
            island_tiles[island_index][x][y] = TileEmpty;
        }
    }
    
    int island_model = rand() % 8;
    
    if( is_large )
    {
        for(int y = 0; y < 6; y++)
            for(int x = 0; x < 6; x++)
                if(large_islands[island_model][y][x])
                    island_tiles[island_index][x][y] = large_islands[island_model][y][x] - 1;  // invertir x e y en arrays
    }
    
    else
    {
        for(int y = 0; y < 4; y++)
            for(int x = 0; x < 4; x++)
                if(small_islands[island_model][y][x])
                    island_tiles[island_index][x+1][y+1] = small_islands[island_model][y][x] - 1;  // invertir x e y en arrays
    }
}

// Devuelve true si tuvo exito
bool initialize_islands() {
    srand(get_time());
    
    // definir el tileset
    select_texture(TextureIsland);
    
    define_region_matrix
    (
        TileEmpty,                // ID de la primera region
        0, 0,                     // X e Y minimas de la primera region
        TileSize-1, TileSize-1,   // X e Y maximas de la primera region
        0, 0,                     // X e Y de referencia de la primera region
        7, 6,                     // dimensiones de la matriz, en tiles
        0                         // pixels de separacion entre regiones
    );
    
    num_islands = MaxIslands;
    float carrier_safe_zone = 500; // Zona segura alrededor del carrier
    
    for(int i = 0; i < num_islands; i++) {
        bool valid_position = false;
        float min_x, min_y;
        
        // generar 4 islas grandes y 6 pequeñas
        bool is_large = (i < 4);
        
        // esto lo usamos para las separaciones
        if(is_large) island_radius[i] = TileSize * 4;
        else         island_radius[i] = TileSize * 3;
        
        int current_retries = 0;
        
        while(!valid_position) {
            
            current_retries++;
            
            // Despues de 300 reintentos entendemos que no va a encontrar
            // solucion nunca, asi que abandonamos para reiniciar la
            // generacion desde el principio con otras islas iniciales
            if( current_retries > 300 )
              return false;
            
            min_x = rand() % (int)(WorldWidth  - MaxTilesX * TileSize);
            min_y = rand() % (int)(WorldHeight - MaxTilesY * TileSize);
            float center_x = min_x + (MaxTilesX * TileSize / 2);
            float center_y = min_y + (MaxTilesY * TileSize / 2);
            
            // Comprobar distancia al carrier
            float dx = center_x - StartingX;
            float dy = center_y - StartingY;
            float carrier_dist = sqrt(dx*dx + dy*dy);
            
            // Comprobar distancia a otras islas
            bool too_close = false;
            
            for(int j = 0; j < i; j++) {
                dx = center_x - (island_x[j] + (MaxTilesX * TileSize / 2));
                dy = center_y - (island_y[j] + (MaxTilesY * TileSize / 2));
                float island_dist = sqrt(dx*dx + dy*dy);
                float safe_radius = island_radius[i] + island_radius[j] + TileSize;
                
                if(island_dist < safe_radius) {
                    too_close = true;
                    break;
                }
            }
            
            // Posición válida si está lejos del carrier y otras islas
            if(carrier_dist > carrier_safe_zone && !too_close) {
                valid_position = true;
                island_x[i] = min_x;
                island_y[i] = min_y;
            }
        }
        
        generate_island_layout(i, is_large);
    }
    
    return true;
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
                        
                        select_region(island_tiles[i][x][y]);
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