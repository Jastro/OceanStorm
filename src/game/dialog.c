// dialog.c
#include "dialog.h"
#include "video.h"
#include "input.h"

int dialog_active = 0;
int[256] dialog_text;
int dialog_texture;

void show_dialog(int* text, int texture_id) {
    dialog_active = 1;
    strcpy(dialog_text, text);
    dialog_texture = texture_id;
}

void update_dialog() {
    if(!dialog_active) return;
    
    // Cerrar con cualquier botón
    if(gamepad_button_a() == 1 || 
       gamepad_button_b() == 1 || 
       gamepad_button_x() == 1 || 
       gamepad_button_y() == 1 ||
       gamepad_button_start() == 1) {
        dialog_active = 0;
    }
}

void render_dialog() {
    if(!dialog_active) return;
    
    // Fondo semi-transparente para toda la pantalla
    set_multiply_color(0x80000000);
    clear_screen(0xFF000000);
    
    // Calcular posición del diálogo (en la parte inferior de la pantalla)
    int dialog_x = (screen_width - DialogWidth) / 2;
    int dialog_y = screen_height - DialogHeight - 20;  // 20 pixels desde abajo
    
    // Dibujar marco del diálogo
    set_multiply_color(0xC0000000);  // Fondo del diálogo
    select_texture(-1);
    select_region(0);
    
    // Dibujar el fondo del diálogo
    for(int y = 0; y < DialogHeight; y++) {
        for(int x = 0; x < DialogWidth; x++) {
            draw_region_at(dialog_x + x, dialog_y + y);
        }
    }
    
    // Dibujar borde
    set_multiply_color(0xFFFFFFFF);
    for(int i = 0; i < DialogWidth; i++) {
        draw_region_at(dialog_x + i, dialog_y);  // Borde superior
        draw_region_at(dialog_x + i, dialog_y + DialogHeight - 1);  // Borde inferior
    }
    for(int i = 0; i < DialogHeight; i++) {
        draw_region_at(dialog_x, dialog_y + i);  // Borde izquierdo
        draw_region_at(dialog_x + DialogWidth - 1, dialog_y + i);  // Borde derecho
    }
    
    // Dibujar la imagen si hay una
    if(dialog_texture >= 0) {
        select_texture(dialog_texture);
        select_region(0);
        draw_region_at(dialog_x + DialogMargin, 
                      dialog_y + DialogMargin);
    }
    
    // Dibujar texto
    set_multiply_color(0xFFFFFFFF);
    int text_x = dialog_x + DialogMargin;
    int text_y = dialog_y + DialogMargin;
    
    if(dialog_texture >= 0) {
        text_x += 40;  // Dejar espacio para la imagen
    }
    
    print_at(text_x, text_y, dialog_text);
    print_at(text_x, text_y + DialogHeight - 30, "Press any button to continue");
}