#include "menu.h"
#include "../utils/definitions.h"

// Variables externas para el estado del juego
extern int game_state;
extern int game_language;

// Estado de este menu
bool inside_options = false;
int current_option = 0;

void initialize_menu() {
    select_texture(TextureMenu);
    
    select_region(RegionMenuBackground);
    define_region_topleft(0,0,  639,359);
    
    select_region(RegionMenuLogo);
    define_region(342,364,  591,447,  466,364);
    
    select_region(RegionMenuCursor);
    define_region(529,452,  553,483,  553,483);
    
    define_region_matrix
    (
        RegionMenuStartENG,
        165,364,
        337,395,
        165,395,
        1,4,
        1
    );
    
    define_region_matrix
    (
        RegionMenuEnglishENG,
        1,364,
        160,406,
        1,406,
        1,4,
        1
    );
    
    define_region_matrix
    (
        RegionMenuLanguageENG,
        342,452,
        524,483,
        433,483,
        1,2,
        1
    );
}

void update_menu() {
    
    // comun a ambos menus: movimiento cursor
    if(gamepad_up() == 1 && current_option > 0)
      current_option--;
    if(gamepad_down() == 1 && current_option <= 0)
      current_option++;
    
    // caso 1: menu principal
    if(!inside_options)
    {
        // Pulsar start entra en la opcion elegida
        if(gamepad_button_start() == 1)
        {
            if( current_option == 0 )
                game_state = StateGame;
            else
            {
                // por defecto elegir idioma actual
                inside_options = true;
                current_option = game_language;
            }
        }
    }
    
    // caso 2: menu opciones
    else
    {
        // Pulsar start aplica el idioma y vuelve
        if(gamepad_button_start() == 1)
        {
            game_language = current_option;
            inside_options = false;
            current_option = 1;
        }
    }
}

void render_menu() {
    // Dibujar el fondo
    select_texture(TextureMenu);
    select_region(RegionMenuBackground);
    draw_region_at(0,0);
    
    // dibujar menu principal
    if(!inside_options)
    {
        // logo
        select_region(RegionMenuLogo);
        draw_region_at(screen_width/2,100);
        
        // opciones
        select_region(RegionMenuStartENG + 2*game_language);
        draw_region_at(291,287);
        select_region(RegionMenuOptionsENG + 2*game_language);
        draw_region_at(291,333);
        
        // cursor
        select_region(RegionMenuLogo);
    }
    
    // dibujar menu de opciones
    else
    {
        // oscurecer fondo
        clear_screen(ShadowColor);
        
        // titulo en amarillo
        set_multiply_color(color_yellow);
        select_region(RegionMenuLanguageENG + game_language);
        draw_region_at(screen_width/2,150);
        
        // opciones en blanco
        set_multiply_color(color_white);
        select_region(RegionMenuEnglishENG + 2*game_language);
        draw_region_at(291,287);
        select_region(RegionMenuSpanishENG + 2*game_language);
        draw_region_at(291,333);
    }
    
    // dibujar cursor (posicion comun a ambos menus)
    // y hacerlo parpadear usando el contador de frames
    select_region(RegionMenuCursor);
    
    if((get_frame_counter() / BlinkRate) % 2 == 0)
        draw_region_at(280,287 + 46*current_option);
}
