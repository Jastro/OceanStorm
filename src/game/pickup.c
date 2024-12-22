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
    select_texture(TexturePickups);
    define_region_matrix(
        0,                  // ID primera región
        0, 0,              // Punto inicial
        36, 45,            // Punto final
        18, 16,            // Punto de referencia (centro)
        4, 1,              // Matriz de 4x1 frames
        0                  // Sin separación
    );

    // Inicializar arrays
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

void spawn_random_pickup(float x, float y) {
    int[4] available_types;
    available_types[0] = PickupHealth;
    available_types[1] = PickupArmor;
    available_types[2] = -1;
    available_types[3] = -1;
    int num_types = 2;  // Empezamos con salud y armadura siempre disponibles
    
    // Añadir escopeta si no la tiene
    if(!soldier_has_weapon[WeaponTypeShotgun]) {
        available_types[num_types++] = PickupShotgun;
    }
    
    // Añadir metralleta si no la tiene
    if(!soldier_has_weapon[WeaponTypeSubmachine]) {
        available_types[num_types++] = PickupSubmachine;
    }
    
    // 40% de probabilidad de generar pickup
    if(rand() % 100 < 40) {
        int type = available_types[rand() % num_types];
        spawn_pickup(x, y, type, 1);  // El valor ya no es necesario realmente
    }
}

void update_pickups() {
    for(int i = 0; i < MaxPickups; i++) {
        if(!pickup_active[i]) continue;
        
        // Comprobar distancia con el jugador
        float dx = pickup_x[i] - soldier_x;
        float dy = pickup_y[i] - soldier_y;
        float distance = sqrt(dx*dx + dy*dy);
        
        // Si está suficientemente cerca, recoger automáticamente
        if(distance < 15.0) {  // Radio de recolección menor que el radio de mostrar texto
            collect_pickup(i);
        }
    }
}

void render_pickups() {
    select_texture(TexturePickups);
    
    for(int i = 0; i < MaxPickups; i++) {
        if(!pickup_active[i]) continue;
        
        // Seleccionar la región según el tipo de pickup
        select_region(pickup_type[i]);  // Ahora el tipo coincide con el frame
        
        // Dibujar el pickup
        tilemap_draw_region(&world_map, pickup_x[i], pickup_y[i]);
    }
}

void collect_pickup(int pickup_index) {
    if(!pickup_active[pickup_index]) return;
    
    play_sound(SoundPickup);

    switch(pickup_type[pickup_index]) {
        case PickupShotgun:
            give_weapon(WeaponTypeShotgun);
            break;
            
        case PickupSubmachine:
            give_weapon(WeaponTypeSubmachine);
            break;
            
        case PickupArmor:
            soldier_armor = clamp(soldier_armor + 1, 0, MaxArmor);
            break;
            
        case PickupHealth:
            soldier_health = clamp(soldier_health + 25, 0, SoldierMaxHealth);
            break;
    }
    
    pickup_active[pickup_index] = 0;
}