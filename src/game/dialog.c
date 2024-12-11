// dialog.c
#include "dialog.h"

int dialog_active = 0;
int[256] dialog_text;
int dialog_region;

int current_dialog = 0;
int[MaxDialogs][256] dialog_sequence;
int[MaxDialogs] dialog_portraits;
int num_dialogs = 0;


void queue_dialog(int* text, int portrait) {
    if(num_dialogs < MaxDialogs) {
        strcpy(dialog_sequence[num_dialogs], text);
        dialog_portraits[num_dialogs] = portrait;
        num_dialogs++;
    }
}

void start_dialog_sequence() {
    if(num_dialogs > 0) {
        current_dialog = 0;
        show_dialog(dialog_sequence[0], dialog_portraits[0]);
    }
}

void show_dialog(int* text, int region_id) {
    dialog_active = 1;
    strcpy(dialog_text, text);
    dialog_region = region_id;
}

void update_dialog() {
    if(!dialog_active) return;
    
    if(gamepad_button_a() == 1 || gamepad_button_b() == 1 || 
       gamepad_button_x() == 1 || gamepad_button_y() == 1 ||
       gamepad_button_start() == 1) {
        current_dialog++;
        if(current_dialog < num_dialogs) {
            show_dialog(dialog_sequence[current_dialog], dialog_portraits[current_dialog]);
        } else {
            dialog_active = 0;
            num_dialogs = 0;
        }
    }
}

void render_dialog() {
    if(!dialog_active) return;
    
    // Fondo semi-transparente
    //set_multiply_color(0x80000000);
    //clear_screen(0xFF000000);
    
    // Ventana de diálogo
    int dialog_x = 300;
    int dialog_y = 270;
    
    // Dibujar marco
    set_multiply_color(0xFFFFFFFF);
    select_texture(TextureDialog);
    select_region(0);
    draw_region_at(dialog_x + 10, dialog_y + 10);
    
    // Dibujar la imagen si hay una
    if(dialog_region >= 0) {
        select_texture(TexturePortraits);
        select_region(dialog_region);
        draw_region_at(dialog_x - 205, dialog_y + 10);
    }
    
    // Dibujar texto
    print_at(dialog_x - 140, dialog_y - 42, dialog_text);
    print_at(dialog_x + 10, dialog_y + UIDialogFrameHeight - 20, "Press any button to continue");
}