#pragma once

#include<Custom/game.h>

void handleClick(SDL_Renderer *renderer, Object *projectile,  Game_State *game_state);
void handleKeyPress(SDL_Renderer *renderer, SDL_Scancode key, Game_State *game_state, bool key_down);