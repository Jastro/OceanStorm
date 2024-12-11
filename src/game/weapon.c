#include "weapon.h"
#include "../utils/definitions.h"
#include "bullet.h"

// Arrays para características de armas
int[3] weapon_type;
int[3] weapon_fire_mode;
int[3] weapon_max_ammo;
int[3] weapon_current_ammo;
float[3] weapon_fire_rate;
float[3] weapon_reload_time;
float[3] weapon_damage;
float[3] weapon_range;
float[3] weapon_spread;
int[3] weapon_bullet_count;
float[3] weapon_last_shot;
float[3] weapon_reload_start;
int[3] weapon_is_reloading;
float[3] weapon_speed;

int current_weapon;

void initialize_weapons()
{
    // Pistola
    weapon_type[0] = WeaponTypePistol;
    weapon_speed[0] = 5.0;
    weapon_fire_mode[0] = FireModeSingle;
    weapon_max_ammo[0] = 12;
    weapon_current_ammo[0] = 12;
    weapon_fire_rate[0] = 0.5;
    weapon_reload_time[0] = 2.0;
    weapon_damage[0] = PistolDamage;
    weapon_range[0] = 200;
    weapon_spread[0] = 0;
    weapon_bullet_count[0] = 1;
    weapon_is_reloading[0] = 0;
    weapon_last_shot[0] = 0;

    // Escopeta
    weapon_type[1] = WeaponTypeShotgun;
    weapon_speed[1] = 4.0;
    weapon_fire_mode[1] = FireModeSpread;
    weapon_max_ammo[1] = 6;
    weapon_current_ammo[1] = 6;
    weapon_fire_rate[1] = 1.0;
    weapon_reload_time[1] = 3.0;
    weapon_damage[1] = ShotgunDamage;
    weapon_range[1] = 100;
    weapon_spread[1] = 30;
    weapon_bullet_count[1] = 8;
    weapon_is_reloading[1] = 0;
    weapon_last_shot[1] = 0;

    // Metralleta
    weapon_type[2] = WeaponTypeSubmachine;
    weapon_speed[2] = 6.0;
    weapon_fire_mode[2] = FireModeAuto;
    weapon_max_ammo[2] = 30;
    weapon_current_ammo[2] = 30;
    weapon_fire_rate[2] = 0.1;
    weapon_reload_time[2] = 2.5;
    weapon_damage[2] = SubmachineGunDamage;
    weapon_range[2] = 150;
    weapon_spread[2] = 10;
    weapon_bullet_count[2] = 1;
    weapon_is_reloading[2] = 0;
    weapon_last_shot[2] = 0;

    current_weapon = WeaponTypePistol;
}

void fire_weapon(float x, float y, float angle)
{
    float current_time = get_frame_counter() / 60.0;

    // Validar si podemos disparar
    if (weapon_current_ammo[current_weapon] <= 0 ||
        weapon_is_reloading[current_weapon] ||
        current_time - weapon_last_shot[current_weapon] < weapon_fire_rate[current_weapon])
    {
        return;
    }

    // Disparar según el modo del arma
    switch (weapon_fire_mode[current_weapon])
    {
    case FireModeSingle:
    case FireModeAuto:
        create_bullet(x, y, angle, weapon_spread[current_weapon], BulletTypePlayer);
        break;

    case FireModeSpread:
        for (int i = 0; i < weapon_bullet_count[current_weapon]; i++)
        {
            create_bullet(x, y, angle, weapon_spread[current_weapon], BulletTypePlayer);
        }
        break;
    }

    // Actualizar estado del arma
    weapon_current_ammo[current_weapon]--;
    weapon_last_shot[current_weapon] = current_time;

    if (weapon_current_ammo[current_weapon] <= 0)
    {
        reload_weapon();
    }
}

void reload_weapon()
{
    if (!weapon_is_reloading[current_weapon] &&
        weapon_current_ammo[current_weapon] < weapon_max_ammo[current_weapon])
    {
        weapon_is_reloading[current_weapon] = 1;
        weapon_reload_start[current_weapon] = get_frame_counter() / 60.0;
    }
}

void update_weapon(float delta_time)
{
    float current_time = get_frame_counter() / 60.0;

    if (weapon_is_reloading[current_weapon])
    {
        if (current_time - weapon_reload_start[current_weapon] >= weapon_reload_time[current_weapon])
        {
            weapon_current_ammo[current_weapon] = weapon_max_ammo[current_weapon];
            weapon_is_reloading[current_weapon] = 0;
        }
    }
}