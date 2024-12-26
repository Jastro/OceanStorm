#include "boss_explosion.h"

int boss_explosion_elapsed_frames;
int boss_enemy_index;

void update_boss_explosion()
{
    // crear explosiones en puntos random
    if(boss_explosion_elapsed_frames < 120)
    {
        if(boss_explosion_elapsed_frames % 10 == 0)
        {
            float x = enemy_x[boss_enemy_index] - 50 + rand()%100;
            float y = enemy_y[boss_enemy_index] - 50 + rand()%100;
            spawn_fx(x, y, Explosion);
        }
    }
    
    update_fx();
    
    // ir al ending cuando terminemos
    if(boss_explosion_elapsed_frames >= 180)
        begin_ending();
    
    boss_explosion_elapsed_frames++;
}

void render_boss_explosion()
{
    // dibujar solo elementos basicos de la escena
    render_world();
    render_heli();
    render_enemies();
    render_fx();
    
    // a partir de 2 segundos aplicar fundido a negro
    if(boss_explosion_elapsed_frames >= 120)
    {
        int blackness_level = max( 0, 255 * (boss_explosion_elapsed_frames-120) / 60 );
        clear_screen( make_color_rgba( 0,0,0,blackness_level ) );
    }
}