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
    
    // Fondo semi-transparente
    set_multiply_color(0x80000000);
    clear_screen(0xFF000000);
    
    // Ventana de diálogo
    int dialog_x = (screen_width - DialogWidth) / 2;
    int dialog_y = (screen_height - DialogHeight) / 2;
    
    // Dibujar marco
    set_multiply_color(0xFFFFFFFF);
    select_texture(-1);
    
    // Dibujar la imagen si hay una
    if(dialog_texture >= 0) {
        select_texture(dialog_texture);
        select_region(0);
        draw_region_at(dialog_x + 10, dialog_y + 10);
    }
    
    // Dibujar texto
    print_at(dialog_x + 10, dialog_y + DialogHeight - 40, dialog_text);
    print_at(dialog_x + 10, dialog_y + DialogHeight - 20, "Press any button to continue");
}