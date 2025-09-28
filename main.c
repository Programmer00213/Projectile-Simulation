#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <Custom/SDL_shape.h>
#include <Custom/SDL_math.h>
#include <Custom/SDL_input.h>
#include <Custom/obj_properties.h>
#include <Custom/game.h>

Game_State game_state = {.enable_debug = false};

bool initialize(SDL_State *state);
void cleanup(SDL_State *state);

// Coordinates for our wall
const int wall_width = 25;
SDL_FRect walls[4] = {
    {.x = 400, .y = 50, .w = 600, .h = wall_width},
    {.x = 1500, .y = 300, .w = wall_width, .h = 200},
    {.x = 200, .y = 600, .w = 400, .h = wall_width},
    {.x = 700, .y = 600, .w = 400, .h = wall_width},
};

int main(int argc, char *argv[])
{

    SDL_State state;

    // Initializing SDL Window and SDL Renderer
    if (!initialize(&state))
    {
        return -1;
    }

    bool running = true;

    // object defination
        // defining the projectile properties
    Object projectile = {
        .u = 0.0f,
        .g = 9.8*50.0f,
        .e = 0.8,
        .slowmotion_factor = 1,
        .direction = {100, 380},
        .angle = 30*MATH_PI/180,
        .object_pos = {100, 380},
        .dimensions = {100, 380, 10, 10},
        .m = 20,
        .t = 0,
        .eject = false
    };
        // Filling circle struct to render a circle, which is our projectile
    SDL_FCircle ball = {
        .center = projectile.object_pos,
        .r = projectile.dimensions.w,
        .color = {207, 80, 10, 255}
    };

    // needed for delta time calculation
    Uint32 prev_time = SDL_GetTicks();

    while (running)
    {
        // Delta Time Calculation
        Uint32 now_time = SDL_GetTicks();
        float delta_time = (now_time - prev_time) / 1000.0f;
        prev_time = now_time;

        // Polling events
        SDL_Event event = {0};
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                {
                    running = false;
                    break;
                }

                case SDL_EVENT_MOUSE_MOTION:
                {
                    game_state.mouse_pos.x = event.motion.x;
                    game_state.mouse_pos.y = event.motion.y;
                    break;
                }

                case SDL_EVENT_MOUSE_BUTTON_UP:
                {
                    game_state.button_pressed = false;
                    game_state.mouse_button = event.button.button;
                    break;
                }

                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    game_state.button_pressed = true;
                    game_state.mouse_button = event.button.button;
                    break;
                }

                case SDL_EVENT_KEY_UP:{
                    handleKeyPress(state.renderer, event.key.scancode, &game_state, false);
                    break;
                }

                case SDL_EVENT_KEY_DOWN:{
                    handleKeyPress(state.renderer, event.key.scancode, &game_state, true);
                    break;
                }
            }
        }
        // Adding window color and clearing screen
        SDL_SetRenderDrawColor(state.renderer, 20, 20, 20, 255);
        SDL_RenderClear(state.renderer);

        // handle mouse input
        handleClick(state.renderer, &projectile, &game_state);

        // Draw objects here
            // Drawing walls
        SDL_SetRenderDrawColor(state.renderer, 50, 50, 50, 255);
        SDL_RenderFillRects(state.renderer, walls, 4);

            // Drawing the projectile
        ball.center = Move(&projectile, &game_state, &state, delta_time);
        SDL_RenderCircle(state.renderer, &ball);

        // enable Debug mode
        if(game_state.enable_debug == true)
            debug(state.renderer, &projectile, &game_state);

        // Swapping buffers
        SDL_RenderPresent(state.renderer);

        // static wall collision
        for (int i = 0; i < 4; i++)
            AABB_Collision(&walls[i], &projectile, &game_state);

        // mouse pointer collision
        Circle_Collision(&projectile, NULL, &game_state);
    }

    // Cleanup function to destroy SDL Window and SDL_Renderer
    cleanup(&state);
    return 0;
}

void cleanup(SDL_State *state)
{
    SDL_DestroyWindow(state->window);
    SDL_DestroyRenderer(state->renderer);

    SDL_Quit();
}

bool initialize(SDL_State *state)
{

    // window setup specifications
    state->width = 1600;
    state->height = 900;

    state->logical_Width = 1600;
    state->logical_Height = 900;

    // Initialize SDL Video
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error Initializing SDL3", NULL);
        return false;
    }

    // Create SDL Window
    state->window = SDL_CreateWindow("Balls", state->width, state->height, 0);
    if (!state->window)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to create SDL3 window", NULL);
        return false;
    }

    // Create SDL Renderer
    state->renderer = SDL_CreateRenderer(state->window, NULL);
    if (!state->renderer)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to create SDL3 renderer", NULL);
        return false;
    }

    // enabling VSync
    SDL_SetRenderVSync(state->renderer, 1);
    SDL_SetRenderDrawBlendMode(state->renderer, SDL_BLENDMODE_BLEND);

    // Creating Logical Presentation
    SDL_SetRenderLogicalPresentation(state->renderer, state->logical_Width, state->logical_Height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return true;
}