#ifndef FX_H
#define FX_H

extern float[MaxFX] fx_x;
extern float[MaxFX] fx_y;
extern int[MaxFX] fx_type;   // Explosion o Splash
extern int[MaxFX] fx_active; // 1 si está activo, 0 si no
extern float[MaxFX] fx_anim_timer;
extern float[MaxFX] fx_frame;

void initialize_fx();
void reset_fx();
void spawn_fx(float x, float y, int type);
void update_fx();
void render_fx();

#endif