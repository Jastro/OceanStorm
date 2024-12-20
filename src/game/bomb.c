#include "bomb.h"
#include "turret.h"

// Definición de los arrays globales
float[MaxActiveBombs] bomb_x;
float[MaxActiveBombs] bomb_y;
float[MaxActiveBombs] bomb_timer;
int[MaxActiveBombs] bomb_active;

void spawn_soldier_enemies(float x, float y, int count)
{
    for (int i = 0; i < count; i++)
    {
        float spawn_x, spawn_y;
        int valid_position = 0;
        int max_attempts = 10;  // Número máximo de intentos para encontrar posición válida
        int attempts = 0;
        
        // Intentar encontrar una posición válida
        while(!valid_position && attempts < max_attempts)
        {
            float angle = (rand() % 360) * pi / 180.0;
            float distance = 50 + (rand() % SoldierSpawnRadius);
            spawn_x = x + cos(angle) * distance;
            spawn_y = y + sin(angle) * distance;
            
            // Comprobar si la posición está sobre terreno válido
            if(is_over_island(spawn_x, spawn_y))
            {
                valid_position = 1;
            }
            
            attempts++;
        }
        
        // Solo crear el enemigo si encontramos una posición válida
        if(valid_position)
        {
            // Elegir patrón de disparo aleatorio
            int random_type = rand() % 100;
            int spread_type;

            if (random_type < 40)
            {
                spread_type = SpreadTypeSingle;
            }
            else if (random_type < 70)
            {
                spread_type = SpreadTypeShotgun;
            }
            else if (random_type < 90)
            {
                spread_type = SpreadTypeSingle;
            }
            else
            {
                spread_type = SpreadTypeCircle;
            }

            spawn_enemy(spawn_x, spawn_y, EnemyTypeSoldier, AIBehaviorChase, spread_type);
        }
    }
}

void initialize_bombs()
{
    for (int i = 0; i < MaxActiveBombs; i++)
    {
        bomb_active[i] = 0;
    }
}

int is_bomb_at_position(float x, float y) {
    for(int i = 0; i < MaxActiveBombs; i++) {
        if(!bomb_active[i]) continue;
        
        // Comprobar si hay una bomba dentro de un radio cercano
        float dx = x - bomb_x[i];
        float dy = y - bomb_y[i];
        float distance = sqrt(dx*dx + dy*dy);
        
        // Si hay una bomba a menos de la mitad del radio de explosión
        if(distance < BombExplosionRadius/2) {
            return 1;
        }
    }
    return 0;
}

int can_plant_bomb(float x, float y) {
    for(int i = 0; i < MaxTurrets; i++) {
        if(!turret_active[i]) continue;
        
        float dx = x - turret_x[i];
        float dy = y - turret_y[i];
        float distance = sqrt(dx*dx + dy*dy);
        
        if(distance < TurretBaseWidth) {
            return !is_bomb_at_position(x, y);
        }
    }
    
    return 0;  // No hay torreta cerca
}

void plant_bomb(float x, float y)
{
    for (int i = 0; i < MaxActiveBombs; i++)
    {
        if (!bomb_active[i])
        {
            bomb_x[i] = x;
            bomb_y[i] = y;
            bomb_timer[i] = BombTimerSeconds;
            bomb_active[i] = 1;
            return;
        }
    }
}

void check_bomb_explosions()
{
    for (int b = 0; b < MaxActiveBombs; b++)
    {
        if (!bomb_active[b])
            continue;

        if (bomb_timer[b] <= 0)
        {
            for (int t = 0; t < MaxTurrets; t++)
            {
                if (!turret_active[t])
                    continue;

                float dx = bomb_x[b] - turret_x[t];
                float dy = bomb_y[b] - turret_y[t];
                float distance = sqrt(dx * dx + dy * dy);

                if (distance <= BombExplosionRadius)
                {
                    // Generar soldados antes de desactivar la torreta
                    int soldier_count = MinSoldiersPerTurret +
                                        (rand() % (MaxSoldiersPerTurret - MinSoldiersPerTurret + 1));
                    spawn_soldier_enemies(turret_x[t], turret_y[t], soldier_count);

                    turret_active[t] = 0;
                    spawn_random_pickup(turret_x[t], turret_y[t]);
                }
            }
            bomb_active[b] = 0;
        }
    }
}

void update_bombs()
{
    float delta_time = 1.0 / 60.0;

    for (int i = 0; i < MaxActiveBombs; i++)
    {
        if (!bomb_active[i])
            continue;

        // Reproducir 1 tick de bomba cada segundo (salvo al llegar a 0)
        if((int)(bomb_timer[i]*60) % 60 == 0)
        {
            if(bomb_timer[i] >= 1.0)
                play_sound(SoundBomb);
        }
        
        // Cuenta atrás para explotar
        bomb_timer[i] -= delta_time;
        
        if (bomb_timer[i] <= 0)
        {
            // sonido de explosion
            play_sound(SoundExplosion);
            
            if (!has_event_happened(TurretDestroyed))
            {
                queue_dialog(&DW_TurretDestroyed);
                queue_dialog(&DW_EnemyDetected);
                queue_dialog(&DW_EnemyPenetration);
                start_dialog_sequence();

                mark_event_as_happened(TurretDestroyed);
            }
            check_bomb_explosions();
        }
    }
}

void render_bombs()
{
    select_texture(-1);

    for (int i = 0; i < MaxActiveBombs; i++)
    {
        if (!bomb_active[i])
            continue;

        // Dibujar la bomba
        set_multiply_color(RedColor);
        tilemap_draw_region(&world_map, bomb_x[i], bomb_y[i]);

        // Mostrar el temporizador
        set_multiply_color(RedColor);
        int[8] timer_text;
        itoa((int)bomb_timer[i] + 1, timer_text, 10);
        tilemap_print(&world_map, bomb_x[i] - 5, bomb_y[i] - 20, timer_text);
    }
    
    // Restaurar el color
    set_multiply_color(color_white);
}
