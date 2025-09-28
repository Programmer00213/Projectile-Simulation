#pragma once

#include<SDL3/SDL.h>
#include"obj_properties.h"
#include<Custom/SDL_shape.h>

typedef struct Game_State{
    SDL_FPoint mouse_pos;
    uint8_t mouse_button;
    bool button_pressed;
    bool enable_debug;
}Game_State;

typedef struct SDL_State
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width;
    int height;

    int logical_Width;
    int logical_Height;
} SDL_State;

void drawTrajectory(SDL_Renderer *renderer, SDL_FPoint position, float u, float angle, float g);
void debug(SDL_Renderer *renderer, Object *projectile, Game_State *game_state);
SDL_FPoint Move(Object *projectile, Game_State *game_state, SDL_State *state, float delta_time);
bool AABB_Collision(SDL_FRect *r, Object *obj, Game_State *game_state);
bool Circle_Collision(Object *obj1, Object *obj2, Game_State *game_state);