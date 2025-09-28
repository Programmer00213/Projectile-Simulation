#pragma once

#include<SDL3/SDL.h>
#include<math.h>

#define MATH_PI 3.1415927f

SDL_FPoint Rotation(SDL_FPoint center, SDL_FPoint coordinate, float angle);
float get_Distance(SDL_FPoint p1, SDL_FPoint p2);
float get_Angle(SDL_FPoint p1, SDL_FPoint p2);