#include "pickup.h"
#include "../utils/definitions.h"
#include "weapon.h"
#include "soldier.h"

// Variables globales para los pickups
float[MaxPickups] pickup_x;
float[MaxPickups] pickup_y;
int[MaxPickups] pickup_type;
int[MaxPickups] pickup_active;
int[MaxPickups] pickup_value;

void initialize_pickups() {
    // Inicializar todos los pickups como inactivos
    for(int i = 0; i < MaxPickups; i++) {
        pickup_active[i] = 0;
    }
}

void spawn_pickup(float x, float y, int type, int value) {
    // Buscar un slot libre
    for(int i = 0; i < MaxPickups; i++) {
        if(!pickup_active[i]) {
            pickup_x[i] = x;
            pickup_y[i] = y;
            pickup_type[i] = type;
            pickup_value[i] = value;
            pickup_active[i] = 1;
            break;
        }
    }
}

void update_pickups() {
    // Aquí podríamos añadir efectos de flotación, rotación, etc.
}

void render_pickups() {
    select_texture(-1);  // Usar textura de la BIOS para dibujar formas simples
    
    for(int i = 0; i < MaxPickups; i++) {
        if(pickup_active[i]) {
            float screen_x = pickup_x[i];
            float screen_y = pickup_y[i];
            tilemap_convert_position_to_screen(&world_map, &screen_x, &screen_y);
            
            // Solo renderizar si está en pantalla
            if(screen_x >= 0 && screen_x <= ScreenWidth &&
               screen_y >= 0 && screen_y <= ScreenHeight) {
                
                // Color según tipo
                if(pickup_type[i] == PickupWeapon) {
                    set_multiply_color(0xFF0000FF); // Rojo para armas
                } else if(pickup_type[i] == PickupAmmo) {
                    set_multiply_color(0xFF00FF00); // Verde para munición
                } else {
                    set_multiply_color(0xFFFFFF00); // Amarillo para armadura
                }
                
                // Por ahora, un simple círculo
                draw_region_at(screen_x, screen_y);
            }
        }
    }
}

void collect_pickup(int pickup_index) {
    if(!pickup_active[pickup_index]) return;
    
    // Procesar el pickup según su tipo
    switch(pickup_type[pickup_index]) {
        case PickupWeapon:
            give_weapon(pickup_value[pickup_index]);
            break;
            
        case PickupAmmo:
            weapon_current_ammo[current_weapon] += pickup_value[pickup_index];
            if(weapon_current_ammo[current_weapon] > weapon_max_ammo[current_weapon])
                weapon_current_ammo[current_weapon] = weapon_max_ammo[current_weapon];
            break;
            
        case PickupArmor:
            soldier_armor += pickup_value[pickup_index];
            if(soldier_armor > MaxArmor)
                soldier_armor = MaxArmor;
            break;
    }
    
    // Desactivar el pickup una vez recogido
    pickup_active[pickup_index] = 0;
}