// dialog.h
#ifndef DIALOG_H
#define DIALOG_H

#define DialogWidth 300    // Más pequeño
#define DialogHeight 150   // Más pequeño
#define DialogMargin 10    // Margen interior

extern int dialog_active;
extern int[256] dialog_text;  // Buffer para el texto
extern int dialog_texture;    // ID de la textura a mostrar

void show_dialog(int* text, int texture_id);
void update_dialog();
void render_dialog();

#endif