// dialog.h
#ifndef DIALOG_H
#define DIALOG_H

#define DialogWidth 400
#define DialogHeight 200

extern int dialog_active;
extern int[256] dialog_text;  // Buffer para el texto
extern int dialog_texture;    // ID de la textura a mostrar

void show_dialog(int* text, int texture_id);
void update_dialog();
void render_dialog();

#endif