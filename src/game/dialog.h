// dialog.h
#ifndef DIALOG_H
#define DIALOG_H

#define DialogWidth 400
#define DialogHeight 200
#define MaxDialogs 30

extern int dialog_active;
extern int[256] dialog_text;
extern int dialog_region;
extern int current_dialog;
extern int[MaxDialogs][256] dialog_sequence;
extern int[MaxDialogs] dialog_portraits;
extern int num_dialogs;

void show_dialog(int* text, int region_id);
void update_dialog();
void render_dialog();
void queue_dialog(int* text, int portrait);
void start_dialog_sequence();

#endif