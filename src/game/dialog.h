// dialog.h
#ifndef DIALOG_H
#define DIALOG_H

#include "dialogtexts.h"

#define DialogWidth 400
#define DialogHeight 200
#define MaxDialogs 30

extern int dialog_active;
extern int[256] dialog_text;
extern int dialog_region;
extern int current_dialog;
extern DialogWindow*[MaxDialogs] dialog_sequence;
extern int num_dialogs;

void show_dialog(DialogWindow* dialog);
void update_dialog();
void render_dialog();
void queue_dialog(DialogWindow* dialog);
void start_dialog_sequence();

#endif