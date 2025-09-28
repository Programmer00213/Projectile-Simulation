#pragma once

#include <SDL3/SDL.h>
#define CIRCLE_VERTEX 30
#define SIDES (CIRCLE_VERTEX - 1)
#define STEPS 360.0f/SIDES
#define INDICES SIDES * 3

typedef struct SDL_FCircle{
    SDL_FPoint center;
    float r;
    SDL_Color color;
}SDL_FCircle;

typedef struct SDL_FTriangle{
    SDL_FPoint a, b, c;
    SDL_Color color;
}SDL_FTriangle;

void SDL_RenderTriangle(SDL_Renderer *renderer, SDL_FTriangle *triangle);
void SDL_RenderCircle(SDL_Renderer *renderer, SDL_FCircle *circle);