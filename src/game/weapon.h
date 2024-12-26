#ifndef WEAPON_H
#define WEAPON_H

// Tipos de armas
#define WeaponTypePistol 0
#define WeaponTypeShotgun 1
#define WeaponTypeSubmachine 2

// Tipos de disparo
#define FireModeSingle 0    // Un disparo por click (pistola)
#define FireModeAuto 1      // Disparo continuo mientras se mantiene (metralleta)
#define FireModeSpread 2    // Disparo en área (escopeta)

// Arrays con las características de cada arma
extern int[3] weapon_type;          // Tipo de arma
extern int[3] weapon_fire_mode;     // Modo de disparo
extern int[3] weapon_max_ammo;      // Munición máxima
extern int[3] weapon_current_ammo;  // Munición actual
extern float[3] weapon_fire_rate;   // Tiempo entre disparos
extern float[3] weapon_reload_time; // Tiempo de recarga
extern float[3] weapon_damage;      // Daño por disparo
extern float[3] weapon_range;       // Alcance
extern float[3] weapon_spread;      // Dispersión
extern int[3] weapon_bullet_count;  // Número de balas por disparo
extern float[3] weapon_last_shot;   // Último disparo
extern float[3] weapon_reload_start;// Cuando empezó la recarga
extern int[3] weapon_is_reloading;  // Estado de recarga
extern int current_weapon;     // Arma actual
extern float[3] weapon_speed;

void initialize_weapons();
void reset_weapons();
void update_weapon();
void fire_weapon(float x, float y, float angle);
void reload_weapon();

#endif