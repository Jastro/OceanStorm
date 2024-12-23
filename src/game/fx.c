#include "../utils/definitions.h"
#include "fx.h"

float[MaxFX] fx_x;
float[MaxFX] fx_y;
int[MaxFX] fx_type;
int[MaxFX] fx_active;
float[MaxFX] fx_anim_timer;
float[MaxFX] fx_frame;

void initialize_fx()
{
    select_texture(TextureExplosionFX);
    define_region_matrix(
        0,      // ID primera región
        0, 0,   // Punto inicial
        31, 31, // Punto final (64x64 - 1)
        16, 16, // Punto de referencia
        7, 1,   // 4 frames en horizontal
        0       // Sin separación
    );

    select_texture(TextureSplashFX);
    define_region_matrix(
        0,      // ID primera región
        0, 0,   // Punto inicial
        31, 31, // Punto final (64x64 - 1)
        16, 16, // Punto de referencia
        4, 1,   // 4 frames en horizontal
        0       // Sin separación
    );
}

void reset_fx()
{
    for (int i = 0; i < MaxFX; i++)
    {
        fx_active[i] = 0;
    }
}

void spawn_fx(float x, float y, int type)
{
    for (int i = 0; i < MaxFX; i++)
    {
        if (!fx_active[i])
        {
            fx_x[i] = x;
            fx_y[i] = y;
            fx_type[i] = type;
            fx_active[i] = 1;
            break;
        }
    }
}

void update_fx()
{
    float delta_time = 1.0 / 60.0;
    for (int i = 0; i < MaxFX; i++)
    {
        if (!fx_active[i])
            continue;

        fx_anim_timer[i] -= delta_time;
        if (fx_anim_timer[i] <= 0)
        {
            fx_frame[i]++;

            if (fx_type[i] == Explosion)
            {
                if (fx_frame[i] >= 7)
                {
                    fx_active[i] = 0;
                }
                else
                {
                    fx_anim_timer[i] = FXFrameTime;
                }
            }
            if (fx_type[i] == Splash)
            {
                if (fx_frame[i] >= 4)
                {
                    fx_active[i] = 0;
                }
                else
                {
                    fx_anim_timer[i] = FXFrameTime;
                }
            }

            if (fx_frame[i] >= 7)
            {
                fx_active[i] = 0;
            }
            else
            {
                fx_anim_timer[i] = FXFrameTime;
            }
        }
    }
}

void render_fx()
{

    for (int i = 0; i < MaxFX; i++)
    {
        if (!fx_active[i])
            continue;

        switch (fx_type[i])
        {
        case Explosion:
            select_texture(TextureExplosionFX);
            break;
        case Splash:
            select_texture(TextureSplashFX);
            break;
        }
        select_region(fx_frame[i]); // Ahora el tipo coincide con el frame
        set_drawing_scale(2, 2);
        tilemap_draw_region(&world_map, fx_x[i], fx_y[i]);
    }
}