#include <Custom/SDL_input.h>
#include <Custom/SDL_math.h>
#include <Custom/game.h>

static SDL_FPoint corrected;


bool release = false; // to throw projectile
float velocity, angle;

void handleClick(SDL_Renderer *renderer, Object *projectile, Game_State *game_state)
{
    if(!game_state->button_pressed && release){
        projectile->eject = true;
        release = false;

        /* Reseting the projectile position to current position and timer to zero, 
        so that new trajectory is calculated from current position and is not affected by past value of timer t, and position. 
        
        This part is extremely important for proper projectile motion*/
        projectile->t = 0;
        projectile->dimensions.x = projectile->object_pos.x;
        projectile->dimensions.y = projectile->object_pos.y;
        projectile->slowmotion_factor = 1;

        // on ejection of projectile, pass the calculated initial velocity and angle to the projectile
        projectile->u = velocity;
        projectile->angle = angle;
    }
    if (game_state->button_pressed && game_state->mouse_button == 1)
    {
        // This slows down projectiles motion
        projectile->slowmotion_factor = 0.1;

        // This line calculates initial velocity by, calculating distance from projectile_center to mouse_pointer
        velocity = get_Distance(projectile->object_pos, game_state->mouse_pos) * 2;

        // getting points exactly opposite to the current mouse position for initial angle calculation, to understand enable debug mode by pressing F12.
        corrected = game_state->mouse_pos;
        corrected.x = -corrected.x + 2 * projectile->object_pos.x;
        corrected.y = -corrected.y + 2 * projectile->object_pos.y;

        angle = get_Angle(projectile->object_pos, corrected);

        // Draw the trajectory of the projectile
        drawTrajectory(renderer, projectile->object_pos, velocity, angle, projectile->g);

        release = true; // signal to initiate projectile throw
    }
}

void handleKeyPress(SDL_Renderer *renderer, SDL_Scancode key, Game_State *game_state, bool key_down){
    if(key == SDL_SCANCODE_F12 && key_down){
        game_state->enable_debug = !game_state->enable_debug;
    }
}