#include "soldier.h"
#include "../utils/definitions.h"
#include "weapon.h"
#include "video.h"
#include "input.h"
#include "math.h"
#include "string.h"
#include "misc.h"
#include "./bullet.h"
#include "bomb.h"

extern int is_player_in_vehicle;

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

void initialize_soldier()
{
    select_texture(TextureSoldier);
    select_region(RegionSoldier);
    define_region(0, 0, SoldierWidth, SoldierHeight, SoldierWidth / 2, SoldierHeight / 2);
    
    // Inicialmente solo tiene la pistola
    soldier_has_weapon[0] = 1;
    soldier_has_weapon[1] = 0;
    soldier_has_weapon[2] = 0;
    
    reset_soldier();
}

void reset_soldier()
{
    // Empezamos fuera del heli
    is_player_in_vehicle = false;
    soldier_state = SoldierStateActive;
    soldier_x = StartingX;
    soldier_y = StartingY - 50;
    soldier_angle = -pi/2;
    
    // Estado inicial
    soldier_armor = MaxArmor;
    soldier_bombs = BombCount;
    soldier_immunity_timer = 0;
    soldier_blink_timer = 0;
}

void update_soldier()
{
    if (soldier_state == SoldierStateNone)
        return;

    // Actualizar inmunidad
    if (soldier_state == SoldierStateImmune)
    {
        soldier_immunity_timer -= 1.0 / 60.0;
        if (soldier_immunity_timer <= 0)
            soldier_state = SoldierStateActive;
    }

    // Actualizar parpadeo
    if (soldier_state == SoldierStateBlinking)
    {
        soldier_blink_timer -= 1.0 / 60.0;
        if (soldier_blink_timer <= 0)
            soldier_state = SoldierStateImmune;
    }

    // Verificar si estamos cerca del avión para poder subir
    float dx = soldier_x - heli_x;
    float dy = soldier_y - heli_y;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance < 50.0 && gamepad_button_b() == 1)
    {
        enter_vehicle();
        return;
    }

    // Movimiento del soldado
    int direction_x, direction_y;
    gamepad_direction(&direction_x, &direction_y);

    // Calcular nueva posición potencial
    float new_x = soldier_x + direction_x * SoldierSpeed;
    float new_y = soldier_y + direction_y * SoldierSpeed;

    // Solo mover si la nueva posición está en terreno válido
    if (soldier_is_over_carrier(new_x, new_y) || is_over_island(new_x, new_y))
    {
        soldier_x = new_x;
        soldier_y = new_y;
    }

    // Actualizar la cámara para seguir al soldado
    if (!is_player_in_vehicle)
    {
        camera_x = soldier_x - ScreenCenterX;
        camera_y = soldier_y - ScreenCenterY;
    }

    // Apuntar con cruceta
    int aim_x = 0, aim_y = 0;
    if (gamepad_up() > 0)
        aim_y = -1;
    if (gamepad_down() > 0)
        aim_y = 1;
    if (gamepad_left() > 0)
        aim_x = -1;
    if (gamepad_right() > 0)
        aim_x = 1;

    if (aim_x != 0 || aim_y != 0)
    {
        soldier_angle = atan2(aim_y, aim_x);
    }

    // Cambiar arma con L/R
    if (gamepad_button_l() == 1)
        switch_weapon_previous();
    if (gamepad_button_r() == 1)
        switch_weapon_next();

    // Comprobar si necesita recargar
    if (!weapon_is_reloading[current_weapon] && weapon_current_ammo[current_weapon] <= 0)
    {
        // Iniciar recarga automática
        weapon_is_reloading[current_weapon] = 1;
        weapon_reload_start[current_weapon] = get_frame_counter() / 60.0;
    }

    // Comprobar si ha terminado la recarga
    if (weapon_is_reloading[current_weapon])
    {
        float current_time = get_frame_counter() / 60.0;
        if (current_time - weapon_reload_start[current_weapon] >= ReloadTime)
        {
            weapon_current_ammo[current_weapon] = weapon_max_ammo[current_weapon];
            weapon_is_reloading[current_weapon] = 0;
        }
    }

    // A para disparar (solo si no está recargando)
    if (gamepad_button_a() > 0 && !weapon_is_reloading[current_weapon])
    {
        fire_weapon(soldier_x, soldier_y, soldier_angle);
    }

    // B para bombas
    if (gamepad_button_b() == 1 && soldier_bombs > 0)
    {
        place_bomb();
    }

    // Actualizar balas existentes
    update_bullets();
}

void render_soldier()
{
    if (soldier_state == SoldierStateNone)
        return;

    // Dibujar soldado usando el sprite
    select_texture(TextureSoldier);
    select_region(RegionSoldier);
    set_multiply_color(TextColor);

    // Rotar el sprite según el ángulo de apuntado
    set_drawing_angle(soldier_angle);
    draw_region_rotated_at(soldier_x - camera_x, soldier_y - camera_y);

    // Indicador de poder subir al avión
    float dx = soldier_x - heli_x;
    float dy = soldier_y - heli_y;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance < 50.0)
    {
        set_multiply_color(0xFF00FF00); // Verde
        print_at((int)(soldier_x - camera_x),
                 (int)(soldier_y - camera_y - 30),
                 "Press B to enter");
    }

    // Mostrar texto de recarga si está recargando
    if (weapon_is_reloading[current_weapon])
    {
        select_texture(-1);
        set_multiply_color(RedColor);
        print_at(
            (int)(soldier_x - camera_x - 30),
            (int)(soldier_y - camera_y - ReloadTextOffset),
            "RECARGANDO");
    }
}

void render_soldier_ui()
{
    if (soldier_state == SoldierStateNone)
        return;

    select_texture(-1);
    set_multiply_color(TextColor);

    // Buffer temporal para convertir números
    int[12] number_buffer; // Suficientemente grande para cualquier número

    // Mostrar munición
    print_at(10, 10, "AMMO: ");
    itoa(weapon_current_ammo[current_weapon], number_buffer, 10);
    print_at(70, 10, number_buffer);
    print_at(100, 10, "/");
    itoa(weapon_max_ammo[current_weapon], number_buffer, 10);
    print_at(120, 10, number_buffer);

    // Mostrar bombas
    print_at(10, 30, "BOMBS: ");
    itoa(soldier_bombs, number_buffer, 10);
    print_at(70, 30, number_buffer);

    // Mostrar armadura
    print_at(10, 50, "ARMOR: ");
    itoa(soldier_armor, number_buffer, 10);
    print_at(70, 50, number_buffer);
}

void soldier_take_damage()
{
    if (soldier_state == SoldierStateImmune ||
        soldier_state == SoldierStateBlinking)
        return;

    soldier_armor--;

    if (soldier_armor <= 0)
    {
        game_state = StateGameOver;
    }
    else
    {
        soldier_state = SoldierStateBlinking;
        soldier_blink_timer = 0.5; // Medio segundo de parpadeo
        soldier_immunity_timer = ImmunityTime;
    }
}

void place_bomb()
{
    if (soldier_bombs > 0 && can_plant_bomb(soldier_x, soldier_y))
    {
        plant_bomb(soldier_x, soldier_y);
        soldier_bombs--;
    }
}

void collect_supplies()
{
    soldier_bombs = BombCount;
}

void switch_weapon_next()
{
    int original_weapon = current_weapon;
    do
    {
        current_weapon = (current_weapon + 1) % 3;
    } while (!soldier_has_weapon[current_weapon] &&
             current_weapon != original_weapon);
}

void switch_weapon_previous()
{
    int original_weapon = current_weapon;
    do
    {
        current_weapon = (current_weapon + 2) % 3;
    } while (!soldier_has_weapon[current_weapon] &&
             current_weapon != original_weapon);
}

void give_weapon(int weapon_type)
{
    soldier_has_weapon[weapon_type] = 1;
    current_weapon = weapon_type;
}
