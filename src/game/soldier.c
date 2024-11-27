#include "soldier.h"
#include "../utils/definitions.h"
#include "weapon.h"
#include "video.h"
#include "input.h"
#include "math.h"
#include "string.h"
#include "misc.h"

// Definir las variables globales que declaramos como extern en soldier.h
float soldier_x;
float soldier_y;
float soldier_angle;
int soldier_state;
int soldier_armor;
int soldier_bombs;
float soldier_immunity_timer;
float soldier_blink_timer;
int[3] soldier_has_weapon;

// Variable externa que necesitamos
extern float camera_x;
extern float camera_y;
extern int game_state;
extern float[MaxBullets] bullet_x;
extern float[MaxBullets] bullet_y; 
extern float[MaxBullets] bullet_angle;
extern int[MaxBullets] bullet_active;

void initialize_soldier() {
    soldier_armor = MaxArmor;
    soldier_bombs = BombCount;
    soldier_state = SoldierStateNone;
    soldier_immunity_timer = 0;
    soldier_blink_timer = 0;
    
    // Inicialmente solo tiene la pistola
    soldier_has_weapon[0] = 1;
    soldier_has_weapon[1] = 0;
    soldier_has_weapon[2] = 0;
}

void update_soldier() {
   if(soldier_state == SoldierStateNone) return;
   
   // Actualizar inmunidad
   if(soldier_state == SoldierStateImmune) {
       soldier_immunity_timer -= 1.0/60.0;
       if(soldier_immunity_timer <= 0)
           soldier_state = SoldierStateActive;
   }

   // Actualizar parpadeo
   if(soldier_state == SoldierStateBlinking) {
       soldier_blink_timer -= 1.0/60.0;
       if(soldier_blink_timer <= 0)
           soldier_state = SoldierStateImmune;
   }
   
   // Movimiento del soldado 
   int direction_x, direction_y;
   gamepad_direction(&direction_x, &direction_y);
   soldier_x += direction_x * SoldierSpeed;
   soldier_y += direction_y * SoldierSpeed;
   
   // Apuntar con cruceta
   int aim_x = 0, aim_y = 0;
   if(gamepad_up() > 0) aim_y = -1;
   if(gamepad_down() > 0) aim_y = 1;
   if(gamepad_left() > 0) aim_x = -1;
   if(gamepad_right() > 0) aim_x = 1;

   if(aim_x != 0 || aim_y != 0) {
       soldier_angle = atan2(aim_y, aim_x);
   }
   
   // Cambiar arma con L/R
   if(gamepad_button_l() == 1)
       switch_weapon_previous();
   if(gamepad_button_r() == 1)
       switch_weapon_next();
   
   // A para disparar
    if(gamepad_button_a() > 0 && !weapon_is_reloading[current_weapon]) {
       fire_weapon(soldier_x, soldier_y, soldier_angle);
    }
   
   // B para bombas 
   if(gamepad_button_b() == 1 && soldier_bombs > 0) {
       place_bomb();
   }

   // Actualizar balas existentes
   update_bullets();
}

void render_soldier() {
   if(soldier_state == SoldierStateNone) return;

   // Dibujar soldado
   set_multiply_color(TextColor);
   select_texture(-1);
   draw_region_at(soldier_x - camera_x, soldier_y - camera_y);
   
   // Flecha de dirección
   float arrow_length = 20.0;
   set_multiply_color(RedColor);
   for(float t = 0; t < arrow_length; t += 2.0) {
       float point_x = soldier_x + cos(soldier_angle) * t;
       float point_y = soldier_y + sin(soldier_angle) * t;
       draw_region_at(point_x - camera_x, point_y - camera_y);
   }

   // Dibujar balas
   select_texture(TextureBullet);
   set_multiply_color(TextColor);
   for(int i = 0; i < MaxBullets; i++) {
       if(bullet_active[i]) {
           draw_region_at(bullet_x[i] - camera_x, bullet_y[i] - camera_y);
       }
   }
}

void render_soldier_ui() {
    if(soldier_state == SoldierStateNone) return;

    select_texture(-1);
    set_multiply_color(TextColor);

    // Buffer temporal para convertir números
    int[12] number_buffer;  // Suficientemente grande para cualquier número

    // Mostrar munición
    print_at(10, 40, "AMMO: ");
    itoa(weapon_current_ammo[current_weapon], number_buffer, 10);
    print_at(70, 40, number_buffer);
    print_at(100, 40, "/");
    itoa(weapon_max_ammo[current_weapon], number_buffer, 10);
    print_at(120, 40, number_buffer);

    // Mostrar bombas
    print_at(10, 60, "BOMBS: ");
    itoa(soldier_bombs, number_buffer, 10);
    print_at(70, 60, number_buffer);

    // Mostrar armadura
    print_at(10, 80, "ARMOR: ");
    itoa(soldier_armor, number_buffer, 10);
    print_at(70, 80, number_buffer);
}

void soldier_take_damage() {
    if(soldier_state == SoldierStateImmune || 
       soldier_state == SoldierStateBlinking) return;
    
    soldier_armor--;
    
    if(soldier_armor <= 0)
    {
        game_state = StateGameOver;
    }
    else
    {
        soldier_state = SoldierStateBlinking;
        soldier_blink_timer = 0.5;  // Medio segundo de parpadeo
        soldier_immunity_timer = ImmunityTime;
    }
}

void place_bomb() {
    if(soldier_bombs > 0)
    {
        // TODO: Implementar lógica de bombas
        soldier_bombs--;
    }
}

void collect_supplies() {
    soldier_bombs = BombCount;
}

void switch_weapon_next() {
    int original_weapon = current_weapon;
    do
    {
        current_weapon = (current_weapon + 1) % 3;
    }
    while(!soldier_has_weapon[current_weapon] && 
          current_weapon != original_weapon);
}

void switch_weapon_previous() {
    int original_weapon = current_weapon;
    do
    {
        current_weapon = (current_weapon + 2) % 3;
    }
    while(!soldier_has_weapon[current_weapon] && 
          current_weapon != original_weapon);
}

void give_weapon(int weapon_type) {
    soldier_has_weapon[weapon_type] = 1;
    current_weapon = weapon_type;
}

void update_camera_zoom() {
    // Interpolar suavemente entre el zoom actual y el objetivo
    if(camera_zoom != target_zoom) {
        float diff = target_zoom - camera_zoom;
        camera_zoom += diff * CameraZoomSpeed;
    }
}
