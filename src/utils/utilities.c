#include "utilities.h"
#include "definitions.h"
#include "../game/island.h"

float clamp(float value, float min_value, float max_value) {
    if(value < min_value) return min_value;
    if(value > max_value) return max_value;
    return value;
}

bool is_over_carrier(float x, float y) {
    // Comprobar si estamos sobre el carrier completo
    float dx = x - (StartingX - CarrierWidth / 2);
    float dy = y - (StartingY - CarrierHeight / 2);

    // El área de aterrizaje cubre todo el carrier
    return (dx >= 0 && dx <= CarrierWidth) && (dy >= 0 && dy <= CarrierHeight);
}

bool is_over_island(float x, float y) {
    
    int tile_x = x / TileSize;
    int tile_y = y / TileSize;
    return (world_grid[tile_y][tile_x] != TileEmpty);
}

bool is_over_ocean(float x, float y) {
    return !is_over_carrier(x, y) && !is_over_island(x, y );
}

// Funciones auxiliares para dibujar numeros siempre con los mismos
// digitos (ejemplo: con 2 digitos 9 se escribe 09). Ademas son mas
// sencillas y rapidas que convertir numero a string y usar print
void print_1digit_at(int x, int y, int number)
{
    int[2] text;
    text[0] = '0' + number % 10;
    text[1] = 0;
    print_at(x, y, text);
}

void print_2digits_at(int x, int y, int number)
{
    int[3] text;
    text[0] = '0' + number / 10;
    text[1] = '0' + number % 10;
    text[2] = 0;
    print_at(x, y, text);
}

void print_3digits_at(int x, int y, int number)
{
    int[4] text;
    text[0] = '0' + number / 100;
    text[1] = '0' + (number / 10) % 10;
    text[2] = '0' + number % 10;
    text[3] = 0;
    print_at(x, y, text);
}

void draw_rectangle(int minx, int miny, int width, int height, int color)
{
    set_multiply_color(color);
    select_texture(-1);
    select_region(256);
    set_drawing_scale(width, height);
    draw_region_zoomed_at(minx, miny);
}