#include "heli.h"
#include "island.h"
#include "bullet.h"
#include "render_system.h"
#include "../utils/definitions.h"
#include "../utils/utilities.h"
#include "video.h"
#include "input.h"
#include "math.h"
#include "string.h"
#include "soldier.h"
#include "events.h"
#include "weapon.h"

// Variables globales para el estado del avión
float heli_x;
float heli_y;
float heli_angle;
float heli_scale;
float heli_velocity;
float fuel;
int heli_frame;
int anim_timer;
int is_player_in_vehicle = 1;
int heli_current_ammo;
float heli_last_shot_time;
int heli_health;
float health_flash_timer = 0;
bool heli_heal_enabled;
int active_cannon = 0;

// Variable externa para el estado del juego
extern int game_scene;

void reload_heli()
{
    heli_current_ammo = clamp(
        heli_current_ammo + HeliReloadRate,
        0,
        HeliMaxAmmo);
}

void shoot_from_heli()
{
    float current_time = get_frame_counter() / 60.0;

    if (heli_current_ammo > 0 &&
        current_time - heli_last_shot_time >= HeliFireRate)
    {
        float shoot_angle;
        float offset_angle;

        switch (active_cannon)
        {
        case 0: // Frontal
            shoot_angle = heli_angle - pi / 2;
            offset_angle = shoot_angle;
            break;
        case 1: // Lateral derecho
            shoot_angle = heli_angle;
            offset_angle = shoot_angle;
            break;
        case 2: // Trasero
            shoot_angle = heli_angle + pi / 2;
            offset_angle = shoot_angle;
            break;
        case 3: // Lateral izquierdo
            shoot_angle = heli_angle - pi;
            offset_angle = shoot_angle;
            break;
        }

        // Calcular punto de origen de la bala usando el offset correcto
        float bullet_x = heli_x + cos(offset_angle) * HeliFrameWidth * heli_scale * 0.5;
        float bullet_y = heli_y + sin(offset_angle) * HeliFrameWidth * heli_scale * 0.5;

        create_bullet(bullet_x, bullet_y, shoot_angle, 0, BulletTypePlayer);

        heli_current_ammo--;
        heli_last_shot_time = current_time;
        play_sound(SoundShoot);
    }
}

void initialize_heli()
{
    select_texture(TextureHeli);

    // Los frames están uno al lado del otro, no uno encima del otro
    // Frame 1
    select_region(0);
    define_region(
        0,               // x inicial del primer frame
        0,               // y inicial (mismo para ambos)
        HeliFrameWidth,  // anchura del frame
        HeliFrameHeight, // altura completa
        47,              // punto central x (mitad del frame individual)
        39               // punto central y
    );

    // Frame 2
    select_region(1);
    define_region(
        HeliFrameWidth,      // x inicial del segundo frame (mitad del sprite)
        0,                   // y inicial
        HeliFrameWidth * 2,  // ancho total para el segundo frame
        HeliFrameHeight,     // altura completa
        47 + HeliFrameWidth, // punto central x (mitad del frame individual)
        39                   // punto central y
    );

    heli_frame = 0;
    anim_timer = 0;
    reset_heli();
    
    // Configurar loop para el sonido
    select_sound(SoundHeli);
    set_sound_loop(true);
    set_sound_loop_start(30126);
    set_sound_loop_end(102642);
}

void reset_heli()
{
    // El avión empieza en el centro del carrier, pero 40 pixels a la izquierda
    heli_x = StartingX + (CarrierWidth / 2) - 40;
    heli_y = StartingY + 20;
    heli_angle = 0.0;
    heli_scale = InitialScale;
    heli_velocity = 0.0;
    fuel = MaxFuel;
    heli_health = HeliMaxHealth;
    heli_heal_enabled = true;
    heli_current_ammo = HeliMaxAmmo;
    heli_last_shot_time = 0;
    active_cannon = 0;

    // Centrar la cámara en el avión
    world_map.camera_position.x = heli_x;
    world_map.camera_position.y = heli_y;
}

void exit_vehicle()
{
    // Si ya estamos fuera, no hacer nada
    if (!is_player_in_vehicle)
        return;

    // Intentar salir del heli por un lado
    soldier_x = heli_x + cos(heli_angle) * 30;
    soldier_y = heli_y + sin(heli_angle) * 30;

    if (is_over_ocean(soldier_x, soldier_y))
    {
        // Intentar salir por el lado contrario
        soldier_x = heli_x - cos(heli_angle) * 30;
        soldier_y = heli_y - sin(heli_angle) * 30;

        // Si no podemos, cancelar la salida
        // (reproducir sonido indicandolo)
        if (is_over_ocean(soldier_x, soldier_y))
        {
            play_sound(SoundActionCancelled);
            return;
        }
    }

    // Actualizar estado solo si hemos salido
    is_player_in_vehicle = 0;
    soldier_state = SoldierStateActive;
    target_zoom = CameraZoomGround;
    play_sound(SoundEnterHeli);
    
    // parar sonido del heli quitando
    // el loop y dejando que termine
    select_channel(ChannelHeli);
    set_channel_loop(false);
}

// No permitimos entrar al heli y dejar una isla
// tras poner la bomba, hasta vencer a los soldados
bool is_enter_vehicle_allowed()
{
    if(is_over_carrier(soldier_x, soldier_y))
        return true;
    
    if(is_over_island(soldier_x, soldier_y))
    {
        for(int i = 0; i < MaxActiveBombs; i++)
            if(bomb_active[i])
                return false;
        
        for (int i = 0; i < MaxEnemies; i++)
            if (enemy_active[i] && enemy_type[i] == EnemyTypeSoldier)
                return false;
        
        return true;
    }
    
    return false;
}

void enter_vehicle()
{
    // Si ya estamos dentro, no hacer nada
    if (is_player_in_vehicle)
        return;
    
    // Comprobar si permitimos subir al heli
    if(!is_enter_vehicle_allowed())
    {
        // la primera vez salta el diálogo
        if (!has_event_happened(FleeIsland))
        {
            end_frame();
            queue_dialog(&DW_FleeIsland);
            queue_dialog(&DW_FleeIslandReply);
            start_dialog_sequence();

            mark_event_as_happened(FleeIsland);
        }
        
        // el resto lo avisamos solo con un sonido
        else play_sound(SoundActionCancelled);
        
        return;
    }
    
    is_player_in_vehicle = 1;
    soldier_state = SoldierStateNone;
    target_zoom = CameraZoomAir;
    play_sound(SoundEnterHeli);
    
    // iniciar sonido del heli
    play_sound_in_channel(SoundHeli, ChannelHeli);
}

void update_heli()
{
    if (!is_player_in_vehicle)
        return;
    
    // Obtener entrada del control
    int direction_x, direction_y;
    gamepad_direction(&direction_x, &direction_y);

    if (gamepad_button_l() == 1)
    {
        active_cannon = (active_cannon + (4-1)) % 4;
        play_sound(SoundChangeWeapon);
    }
    
    if (gamepad_button_r() == 1)
    {
        active_cannon = (active_cannon + 1) % 4;
        play_sound(SoundChangeWeapon);
    }
    
    // Rotar el avión
    if (gamepad_left() > 0)
        heli_angle -= RotationSpeed;
    if (gamepad_right() > 0)
        heli_angle += RotationSpeed;

    if (gamepad_button_a() > 0)
    {
        shoot_from_heli();
    }

    // Movimiento hacia adelante
    if (gamepad_up() > 0 && fuel > 0)
    {
        heli_x += MovementSpeed * sin(heli_angle);
        heli_y -= MovementSpeed * cos(heli_angle);
        fuel -= FuelConsumption;
        heli_scale = clamp(heli_scale + AscendSpeed, MinScale, MaxScale);
        
        // Avisar de combustible bajo con sonido
        if(fuel < LowFuel && fuel >= (LowFuel - FuelConsumption))
            play_sound(SoundLowFuel);
        
        // Actualizar animación
        anim_timer++;
        if (anim_timer >= HeliAnimSpeed)
        {
            anim_timer = 0;
            heli_frame = 1 - heli_frame; // Alternar entre 0 y 1
        }
    }
    else
    {
        heli_scale = clamp(heli_scale - DescentSpeed, MinScale, MaxScale);
        heli_frame = 0; // Frame base cuando no se mueve
        anim_timer = 0;
    }

    // Comprobar aterrizaje
    if (heli_scale <= LandingScale)
    {
        // Si estamos sobre el carrier o una isla, permitir aterrizaje
        if (is_over_carrier(heli_x, heli_y) || is_over_island(heli_x, heli_y))
        {
            // Aterrizaje exitoso
            heli_scale = LandingScale; // Mantenemos esta escala para que se vea bien

            // Solo repostar si estamos en el carrier
            if (is_over_carrier(heli_x, heli_y))
            {
                // Recargar combustible
                fuel = clamp(fuel + RefuelRate, 0, MaxFuel);
                reload_heli();

                // Curar helicóptero
                if(heli_heal_enabled)
                {
                    heli_health = clamp(heli_health + RefuelRate, 0, HeliMaxHealth);
                    
                    // No seguir curando tras alzanzar el maximo
                    if(heli_health >= HeliMaxHealth)
                        heli_heal_enabled = false;
                }

                // Restaurar soldado completamente
                soldier_health = SoldierMaxHealth;
                soldier_armor = MaxArmor;
                soldier_bombs = BombCount;

                // Recargar todas las armas del soldado
                for (int i = 0; i < 3; i++)
                {
                    if (soldier_has_weapon[i])
                    {
                        weapon_current_ammo[i] = weapon_max_ammo[i];
                    }
                }
            }
        }
        else if (heli_scale <= MinScale)
        {
            kill_player();
        }
    }

    // Game over si nos quedamos sin combustible y sin altura
    if (fuel <= 0 && heli_scale <= MinScale && !is_over_carrier(heli_x, heli_y) && !is_over_island(heli_x, heli_y))
    {
        kill_player();
    }

    // Mantener el avión dentro del mundo
    heli_x = clamp(heli_x, 0, WorldWidth);
    heli_y = clamp(heli_y, 0, WorldHeight);

    if (is_player_in_vehicle)
    {
        world_map.camera_position.x = heli_x;
        world_map.camera_position.y = heli_y;
    }

    if (!is_over_ocean(heli_x, heli_y) && gamepad_button_b() == 1)
    {
        exit_vehicle();
        return;
    }
    
    // Reactivar la curacion si volamos fuera del carrier
    if(is_over_ocean(heli_x, heli_y))
    {
        if(heli_scale > LandingScale)
            heli_heal_enabled = true;
    }
}

void render_heli_fuel()
{
    // dibujar marco vacio
    select_texture(TextureGui);
    select_region(RegionHeliFuel);
    draw_region_at(5, 5);
    
    // Calcular ancho proporcional de la barra
    int max_bar_width = 75;
    int bar_height = 11;
    int fuel_width = (int)((fuel / MaxFuel) * max_bar_width);

    // Dibujar barra de combustible
    draw_rectangle(48, 10, fuel_width, bar_height, color_white);
    
    // Avisar de combustible bajo
    if (fuel <= (MaxFuel / 2))
    {
        if (!has_event_happened(FuelWarning))
        {
            queue_dialog(&DW_FuelHalf);
            queue_dialog(&DW_FuelHalfReply);
            start_dialog_sequence();

            mark_event_as_happened(FuelWarning);
        }
    }
    
    // Restaurar el color
    set_multiply_color(color_white);
}

void render_heli_health()
{
    // dibujar marco vacio
    select_texture(TextureGui);
    select_region(RegionHeliHealth);
    draw_region_at(5, 28);
    
    // Calcular ancho proporcional de la barra
    int max_bar_width = 75;
    int bar_height = 11;
    int health_width = (int)((heli_health / (float)HeliMaxHealth) * max_bar_width);
    int health_color = GreenColor;
    
    // Vida roja cuando recibe daño
    if (health_flash_timer > 0)
        health_color = RedColor;
    
    // Dibujar barra de vida
    draw_rectangle(48, 33, health_width, bar_height, health_color);
    
    // Restaurar el color
    set_multiply_color(color_white);
}

void render_heli_gui()
{
    // Dibujar las barras
    render_heli_fuel();
    render_heli_health();
    
    // Mostrar munición
    select_texture(TextureGui);
    select_region(RegionHeliAmmo);
    draw_region_at(5,51);
    print_3digits_at(46, 55, heli_current_ammo);
    print_3digits_at(91, 55, HeliMaxAmmo);
    
    // Informar de estado del cañón
    if (active_cannon == 0)
        print_at(5, 99, "CANNON: FRONT");
    else if (active_cannon == 1)
        print_at(5, 99, "CANNON: RIGHT");
    else if (active_cannon == 2)
        print_at(5, 99, "CANNON: REAR");
    else
        print_at(5, 99, "CANNON: LEFT");

    print_at(5,119,"[L/R to rotate]");
}

void render_heli()
{
    float height_factor = (heli_scale - LandingScale) / (MaxScale - LandingScale);
    float max_shadow_distance = HeliShadowOffset;

    // Evitar que la sombra vaya al lado contrario
    // cuando estamos por debajo de las islas
    float shadow_offset = 2 + max(0, height_factor * max_shadow_distance);

    // Si la sombra esta sobre el mar reducirla para dar efecto
    // de mas distancia
    float shadow_x = heli_x + shadow_offset;
    float shadow_y = heli_y + shadow_offset;
    float shadow_scale = heli_scale;

    if (is_over_ocean(shadow_x, shadow_y))
        shadow_scale *= 0.7;

    // 1. Dibujar la sombra primero
    select_texture(TextureHeli);
    select_region(RegionHeli + heli_frame);
    set_multiply_color(0x80000000); // Negro semi-transparente
    set_drawing_scale(shadow_scale, shadow_scale);
    set_drawing_angle(heli_angle);
    tilemap_draw_region_rotozoomed(&world_map, shadow_x, shadow_y);

    // 2. Dibujar el avión
    set_multiply_color(0xFFFFFFFF);
    select_region(heli_frame);
    set_drawing_scale(heli_scale, heli_scale);
    set_drawing_angle(heli_angle);
    tilemap_draw_region_rotozoomed(&world_map, heli_x, heli_y);

    // Restaurar el color
    set_multiply_color(color_white);

    // 3. Actualizar la interfaz
    if (!is_player_in_vehicle)
    {
        if (heli_scale > LandingScale)
        {
            heli_scale = clamp(heli_scale - DescentSpeed, MinScale, MaxScale);
        }
    }

    if (health_flash_timer > 0)
    {
        health_flash_timer -= 1.0 / 60.0;
    }

    // 4. Cuando estemos sobre islas o el carrier:
    if (is_player_in_vehicle && !is_over_ocean(heli_x, heli_y))
    {
        // si estamos en tierra avisamos de que podemos salir
        if(heli_scale == LandingScale)
        {
            // Dibujar con parpadeo para que se vea mejor
            if(get_frame_counter() % 40 > 8)
            {
                select_texture(TextureGui);
                select_region(RegionExitSign);
                tilemap_draw_region(
                    &world_map,
                    heli_x,
                    heli_y - 30);
            }
        }
    }
    
    // 5. Cuando estemos volando indicar la direccion del cañon
    if (is_player_in_vehicle && heli_scale > LandingScale)
    {
        // Dibujar con parpadeo para que se vea mejor
        if(get_frame_counter() % 40 > 8)
        {
            select_texture(TextureGui);
            select_region(RegionCannonArrow);
            
            float cannon_angle = heli_angle + pi/2 * (active_cannon - 1);
            set_drawing_angle(cannon_angle);
            tilemap_draw_region_rotated(&world_map, heli_x, heli_y);
        }
    }
}

void update_camera_zoom()
{
    if (camera_zoom != target_zoom)
    {
        float diff = target_zoom - camera_zoom;
        camera_zoom += diff * CameraZoomSpeed;
    }
}