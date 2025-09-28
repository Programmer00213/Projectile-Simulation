#pragma once

#include<SDL3/SDL.h>

typedef struct Object{
    float u; //initial velocity
    float g; //accelaration due to gravity
    float angle; //initial angle
    float e; //coefficient of restitution
    float slowmotion_factor; // to give slow motion effect
    SDL_FPoint direction;
    SDL_FPoint object_pos; 
    SDL_FRect dimensions; // dimesions of object, for circles, w = h = r 
    float m; //mass
    bool movable; // can move

    float t; // timer

    bool eject; // initiate projectile throw
}Object;