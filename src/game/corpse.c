#include "corpse.h"

float[MaxCorpses] corpse_x;
float[MaxCorpses] corpse_y;
float[MaxCorpses] corpse_blood_scale;
float[MaxCorpses] corpse_anim_timer;
int[MaxCorpses] corpse_frame;
int[MaxCorpses] corpse_state;
int[MaxCorpses] corpse_active;
float[MaxCorpses] final_blood_scale;  // Declaración de la nueva variable

void initialize_corpses() {
    // Configurar los sprites de muerte
    select_texture(TextureEnemySoldierDeath);
    define_region_matrix(
        0,              // ID primera región
        0, 0,          // Punto inicial
        63, 63,        // Punto final (64x64 - 1)
        32, 32,        // Punto de referencia
        4, 1,          // 4 frames en horizontal
        0              // Sin separación
    );

    // Configurar sprite de sangre
    select_texture(TextureEnemySoldierBlood);
    select_region(0);
    define_region(0, 0, 79, 79, 40, 40);  // Centro de la sangre

    // Inicializar arrays
    for(int i = 0; i < MaxCorpses; i++) {
        corpse_active[i] = 0;
    }
}

/*void spawn_corpse(float x, float y) {
    for(int i = 0; i < MaxCorpses; i++) {
        if(!corpse_active[i]) {
            corpse_x[i] = x;
            corpse_y[i] = y;
            corpse_frame[i] = 0;
            corpse_anim_timer[i] = CorpseFrameTime;
            corpse_blood_scale[i] = 0.1;
            // Generar un tamaño final aleatorio entre 0.4 y 0.8
            final_blood_scale[i] = 0.4 + (rand() % 400) / 1000.0;  // Genera entre 0.4 y 0.8
            corpse_state[i] = CorpseStateAnimating;
            corpse_active[i] = 1;
            break;
        }
    }
}*/

void spawn_corpse(float x, float y) {
    int free_slot = -1;
    int oldest_offscreen = -1;
    int oldest_onscreen = -1;
    float farthest_distance = 0;
    
    // Primero buscar un slot libre
    for(int i = 0; i < MaxCorpses; i++) {
        if(!corpse_active[i]) {
            free_slot = i;
            break;
        }
        
        // Si está fuera de pantalla, considerar para reemplazar
        float screen_x = corpse_x[i], screen_y = corpse_y[i];
        tilemap_convert_position_to_screen(&world_map, &screen_x, &screen_y);
        
        if(screen_x < -64 || screen_x > ScreenWidth + 64 ||
           screen_y < -64 || screen_y > ScreenHeight + 64) {
            // Guardar el más antiguo (más tiempo en estado estático)
            if(corpse_state[i] == CorpseStateStatic) {
                if(oldest_offscreen == -1) oldest_offscreen = i;
            }
        } else {
            // Guardar el más antiguo en pantalla por si lo necesitamos
            if(corpse_state[i] == CorpseStateStatic) {
                if(oldest_onscreen == -1) oldest_onscreen = i;
            }
        }
    }
    
    // Decidir qué slot usar
    int slot_to_use;
    if(free_slot != -1) {
        slot_to_use = free_slot;
    }
    else if(oldest_offscreen != -1) {
        slot_to_use = oldest_offscreen;
    }
    else if(oldest_onscreen != -1) {
        slot_to_use = oldest_onscreen;
    }
    else {
        // No deberíamos llegar aquí, pero por si acaso
        slot_to_use = 0;
    }
    
    // Crear el nuevo cadáver en el slot elegido
    corpse_x[slot_to_use] = x;
    corpse_y[slot_to_use] = y;
    corpse_frame[slot_to_use] = 0;
    corpse_anim_timer[slot_to_use] = CorpseFrameTime;
    corpse_blood_scale[slot_to_use] = 0.1;
    final_blood_scale[slot_to_use] = 0.4 + (rand() % 400) / 1000.0;
    corpse_state[slot_to_use] = CorpseStateAnimating;
    corpse_active[slot_to_use] = 1;
}

void update_corpses() {
    float delta_time = 1.0/60.0;
    
    for(int i = 0; i < MaxCorpses; i++) {
        if(!corpse_active[i]) continue;

        switch(corpse_state[i]) {
            case CorpseStateAnimating:
                // Aumentar escala del charco durante la animación
                if(corpse_blood_scale[i] < final_blood_scale[i]) {
                    corpse_blood_scale[i] += (final_blood_scale[i] / CorpseFrames) * delta_time / CorpseFrameTime;
                }

                corpse_anim_timer[i] -= delta_time;
                if(corpse_anim_timer[i] <= 0) {
                    corpse_frame[i]++;
                    if(corpse_frame[i] >= CorpseFrames) {
                        corpse_state[i] = CorpseStateStatic;
                        // Asegurar que el charco alcanza su tamaño final
                        corpse_blood_scale[i] = final_blood_scale[i];
                    } else {
                        corpse_anim_timer[i] = CorpseFrameTime;
                    }
                }
                break;
        }

        // Comprobar si está en pantalla
        float screen_x = corpse_x[i], screen_y = corpse_y[i];
        tilemap_convert_position_to_screen(&world_map, &screen_x, &screen_y);
        
        if(screen_x < -64 || screen_x > ScreenWidth + 64 ||
           screen_y < -64 || screen_y > ScreenHeight + 64) {
            corpse_state[i] = CorpseStateHidden;
        } else if(corpse_state[i] == CorpseStateHidden) {
            corpse_state[i] = CorpseStateStatic;
        }
    }
}

void render_corpses() {
    for(int i = 0; i < MaxCorpses; i++) {
        if(!corpse_active[i] || corpse_state[i] == CorpseStateHidden) 
            continue;

        // Dibujar siempre el charco de sangre primero
        select_texture(TextureEnemySoldierBlood);
        select_region(0);
        set_drawing_scale(corpse_blood_scale[i], corpse_blood_scale[i]);
        tilemap_draw_region_zoomed(&world_map, corpse_x[i], corpse_y[i]);

        // Dibujar el cadáver encima
        select_texture(TextureEnemySoldierDeath);
        // Usar el último frame si está en estado estático, si no usar el frame actual
        int frame_to_use;
        if(corpse_state[i] == CorpseStateStatic) {
            frame_to_use = 3;  // Último frame (0,1,2,3)
        } else {
            frame_to_use = corpse_frame[i];
        }
        select_region(frame_to_use);
        tilemap_draw_region(&world_map, corpse_x[i], corpse_y[i]);
    }
}