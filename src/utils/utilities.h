#ifndef UTILITIES_H
#define UTILITIES_H

float clamp(float value, float min_value, float max_value);

bool is_over_carrier(float x, float y);
bool is_over_island(float x, float y);
bool is_over_ocean(float x, float y);

void print_1digit_at(int x, int y, int number);
void print_2digits_at(int x, int y, int number);
void print_3digits_at(int x, int y, int number);

void draw_rectangle(int minx, int miny, int width, int height, int color);

#endif