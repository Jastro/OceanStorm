#ifndef PICKUP_H
#define PICKUP_H


// Estado del pickup
extern float[MaxPickups] pickup_x;
extern float[MaxPickups] pickup_y;
extern int[MaxPickups] pickup_type;   // Tipo de pickup (arma, munición, etc)
extern int[MaxPickups] pickup_active; // 1 si está activo, 0 si no
extern int[MaxPickups] pickup_value;  // ID del arma, cantidad de munición, etc

void initialize_pickups();
void spawn_pickup(float x, float y, int type, int value);
void update_pickups();
void render_pickups();
void collect_pickup(int pickup_index);

#endif