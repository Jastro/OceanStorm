#ifndef UTILITIES_H
#define UTILITIES_H

float clamp(float value, float min_value, float max_value);

bool is_over_carrier(float x, float y);
bool is_over_island(float x, float y);
bool is_over_ocean(float x, float y);

#endif