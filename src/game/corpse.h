#ifndef CORPSE_H
#define CORPSE_H

#include "../utils/definitions.h"

extern float[MaxCorpses] corpse_x;
extern float[MaxCorpses] corpse_y;
extern float[MaxCorpses] corpse_blood_scale;
extern float[MaxCorpses] corpse_anim_timer;
extern int[MaxCorpses] corpse_frame;
extern int[MaxCorpses] corpse_state;
extern int[MaxCorpses] corpse_active;
extern float[MaxCorpses] final_blood_scale;

void initialize_corpses();
void reset_corpses();
void spawn_corpse(float x, float y);
void update_corpses();
void render_corpses();

#endif