#include "island.h"
#include "../utils/definitions.h"

// Variables globales
float[MaxIslands] island_x;
float[MaxIslands] island_y;
float[MaxIslands] island_radius;
int num_islands;

void generate_island_layout(int island_index, bool is_large) {
    
    int island_model = rand() % 8;
    int min_tile_x = island_x[ island_index ] / TileSize;
    int min_tile_y = island_y[ island_index ] / TileSize;
    
    if( is_large )
    {
        for(int y = 0; y < 9; y++)
            for(int x = 0; x < 9; x++)
                world_grid[min_tile_y + y][min_tile_x + x] = large_islands[island_model][y][x] - 1;
    }
    
    else
    {
        for(int y = 0; y < 6; y++)
            for(int x = 0; x < 6; x++)
                world_grid[min_tile_y + y + 1][min_tile_x + x + 1] = small_islands[island_model][y][x] - 1;
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
    float carrier_safe_zone = 600; // Zona segura alrededor del carrier
    
    // Limpiar todo el mapa con tiles de mar
    memset( world_grid, TileEmpty, sizeof(world_grid) );
    
    // Generar cada isla
    for(int i = 0; i < num_islands; i++) {
        bool valid_position = false;
        float min_x, min_y;
        
        // generar 4 islas grandes y 6 pequeñas
        bool is_large = (i < 4);
        
        // esto lo usamos para las separaciones
        if(is_large) island_radius[i] = TileSize * 6;
        else         island_radius[i] = TileSize * 4;
        
        int current_retries = 0;
        
        while(!valid_position) {
            
            current_retries++;
            
            // Despues de 300 reintentos entendemos que no va a encontrar
            // solucion nunca, asi que abandonamos para reiniciar la
            // generacion desde el principio con otras islas iniciales
            if( current_retries > 300 )
              return false;
            
            min_x = TileSize * (rand() % (int)(WorldTilesX - MaxTilesX));
            min_y = TileSize * (rand() % (int)(WorldTilesY - MaxTilesY));
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
