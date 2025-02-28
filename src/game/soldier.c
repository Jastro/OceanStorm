#include "soldier.h"
#include "../utils/definitions.h"
#include "weapon.h"
#include "bullet.h"
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
int soldier_health;
float soldier_stamina;
float soldier_scale;
int is_swimming;

// Variable externa que necesitamos
extern int game_scene;
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
    soldier_angle = -pi / 2;

    // Estado inicial
    soldier_armor = MaxArmor;
    soldier_health = SoldierMaxHealth;
    soldier_bombs = BombCount;
    soldier_immunity_timer = 0;
    soldier_blink_timer = 0;
    soldier_stamina = MaxStamina;
    soldier_scale = 1.0;
    is_swimming = 0;
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
    int direction_x, direction_y, currentSpeed;
    gamepad_direction(&direction_x, &direction_y);

    int over_carrier = is_over_carrier(soldier_x, soldier_y);
    int over_island = is_over_island(soldier_x, soldier_y);
    int on_valid_ground = over_carrier || over_island;
    is_swimming = !on_valid_ground;
    // Calcular nueva posición potencial
    if (is_swimming)
    {
        if (soldier_scale > 0.7)
        {
            soldier_scale = clamp(soldier_scale - 0.005, 0.5, 1);
        }
        currentSpeed = SoldierSwimSpeed;

        // En el agua usamos movimiento libre
        float new_x = soldier_x + direction_x * currentSpeed;
        float new_y = soldier_y + direction_y * currentSpeed;

        // Reducir estamina
        soldier_stamina -= StaminaDrainRate / 60.0;

        if (soldier_stamina <= 0)
        {
            // Ahogándose
            soldier_scale = SoldierDrownScale;
            kill_player();
        }
        else
        {
            // Permitir movimiento en el agua
            soldier_x = new_x;
            soldier_y = new_y;
        }
    }
    else
    {
        // En tierra firme usamos el movimiento por ejes
        if (soldier_scale < 1)
        {
            soldier_scale = 1;
        }
        currentSpeed = SoldierSpeed;
        soldier_stamina = MaxStamina; // Recuperar estamina en tierra

        // Movimiento en X
        float new_x = soldier_x + direction_x * currentSpeed;
        if (is_over_carrier(new_x, soldier_y) || is_over_island(new_x, soldier_y))
        {
            soldier_x = new_x;
        }
        else
        {
            soldier_x = new_x;
        }

        // Movimiento en Y
        float new_y = soldier_y + direction_y * currentSpeed;
        if (is_over_carrier(soldier_x, new_y) || is_over_island(soldier_x, new_y))
        {
            soldier_y = new_y;
        }
        else
        {
            soldier_y = new_y;
        }
    }

    // Actualizar la cámara para seguir al soldado
    if (!is_player_in_vehicle)
    {
        world_map.camera_position.x = soldier_x;
        world_map.camera_position.y = soldier_y;
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
        // Crear la bala un poco por delante del soldado
        // en vez de en su centro para evitar colisiones
        // no deseadas con enemigos
        float firing_x = soldier_x + 6 * cos(soldier_angle);
        float firing_y = soldier_y + 6 * sin(soldier_angle);
        
        fire_weapon(firing_x, firing_y, soldier_angle);
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

    // Color según estado
    if (soldier_state == SoldierStateImmune || soldier_state == SoldierStateBlinking)
    {
        if ((get_frame_counter() / 4) % 2)
        {                                  // Parpadeo rápido
            set_multiply_color(TextColor); // Color normal
        }
        else
        {
            set_multiply_color(0xFFFFFFFF); // Blanco
        }
    }
    else
    {
        set_multiply_color(TextColor);
    }

    // Rotar y escalar el sprite
    set_drawing_angle(soldier_angle);
    set_drawing_scale(soldier_scale, soldier_scale);
    tilemap_draw_region_rotozoomed(&world_map, soldier_x, soldier_y);

    // Restaurar color
    set_multiply_color(color_white);
    
    // Indicador de poder subir al heli
    float dx = soldier_x - heli_x;
    float dy = soldier_y - heli_y;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance < 50.0 && is_enter_vehicle_allowed())
    {
        // Dibujar con parpadeo para que se vea mejor
        if(get_frame_counter() % 40 > 8)
        {
            select_texture(TextureGui);
            select_region(RegionEnterSign);
            tilemap_draw_region(
                &world_map,
                heli_x,
                heli_y - 30);
        }
    }

    // Mostrar texto de recarga si está recargando
    if (weapon_is_reloading[current_weapon])
    {
        select_texture(TextureGui);
        select_region(RegionSoldierReloading);
        
        tilemap_draw_region(
            &world_map,
            soldier_x - SoldierBarWidth / 2,
            soldier_y - ReloadTextOffset);
    }

    // Solo mostrar barra de vida cuando no hay armadura
    if (soldier_armor == 0)
    {
        // convertir a coordenadas de pantalla para dibujar
        float bar_x = soldier_x - SoldierBarWidth / 2;
        float bar_y = soldier_y - SoldierBarOffsetY;
        tilemap_convert_position_to_screen(&world_map, &bar_x, &bar_y);
        
        // dibujar marco vacio
        select_texture(TextureGui);
        select_region(RegionSoldierHealth);
        draw_region_at(bar_x, bar_y);
        
        // Calcular ancho proporcional de la barra
        int bar_height = 6;
        float health_percent = soldier_health / (float)SoldierMaxHealth;
        int health_width = SoldierBarWidth * health_percent;

        // Vida verde cuando está llena, roja cuando está baja
        int health_color = GreenColor;
        
        if (health_percent <= 0.5)
            health_color = RedColor;
        
        // Dibujar barra de vida
        draw_rectangle(bar_x+5, bar_y+3, health_width, bar_height, health_color);
        
        // Restaurar el color
        set_multiply_color(color_white);
    }

    // Dibujar barra de estamina si está nadando
    if (is_swimming)
    {
        // convertir a coordenadas de pantalla para dibujar
        float bar_x = soldier_x - SoldierBarWidth / 2;
        float bar_y = soldier_y - SoldierBarOffsetY - 13; // Encima de la barra de vida
        tilemap_convert_position_to_screen(&world_map, &bar_x, &bar_y);
        
        // dibujar marco vacio
        select_texture(TextureGui);
        select_region(RegionSoldierHealth);
        draw_region_at(bar_x, bar_y);
        
        // Calcular ancho proporcional de la barra
        int bar_height = 6;
        int stamina_width = (soldier_stamina / MaxStamina) * SoldierBarWidth;
        
        // Dibujar barra de estamina
        draw_rectangle(bar_x+5, bar_y+3, stamina_width, bar_height, 0xFFFFFF00);
    }

    // Restaurar color
    set_multiply_color(color_white);
}

void render_soldier_gui()
{
    if (soldier_state == SoldierStateNone)
        return;

    select_texture(TextureGui);

    // Mostrar munición
    select_region(RegionSoldierAmmo);
    draw_region_at(5,5);
    print_2digits_at(46, 9, weapon_current_ammo[current_weapon]);
    print_2digits_at(81, 9, weapon_max_ammo[current_weapon]);

    // Mostrar bombas
    select_region(RegionSoldierBombs);
    draw_region_at(5,47);
    print_1digit_at(46, 51, soldier_bombs);

    // Mostrar armadura
    select_region(RegionSoldierArmor);
    draw_region_at(5,88);
    print_1digit_at(46, 92, soldier_armor);
}

void soldier_take_damage()
{
    if (soldier_state == SoldierStateImmune || soldier_state == SoldierStateBlinking)
        return;

    if (soldier_armor > 0)
    {
        soldier_armor--;
        trigger_screen_shake();
    }
    else if (soldier_health > 0)
    {
        soldier_health -= 25;
        trigger_screen_shake();
    }

    if (soldier_health <= 0)
    {
        kill_player();
    }
    else
    {
        soldier_state = SoldierStateBlinking;
        soldier_blink_timer = 0.5;
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
    
    // Indicar con sonidos si realmente hemos cambiado de arma
    if(current_weapon != original_weapon)
        play_sound(SoundChangeWeapon);
    else
        play_sound(SoundActionCancelled);
}

void switch_weapon_previous()
{
    int original_weapon = current_weapon;
    do
    {
        current_weapon = (current_weapon + 2) % 3;
    } while (!soldier_has_weapon[current_weapon] &&
             current_weapon != original_weapon);
             
    // Indicar con sonidos si realmente hemos cambiado de arma
    if(current_weapon != original_weapon)
        play_sound(SoundChangeWeapon);
    else
        play_sound(SoundActionCancelled);
}

void give_weapon(int weapon_type)
{
    soldier_has_weapon[weapon_type] = 1;
    current_weapon = weapon_type;
}
