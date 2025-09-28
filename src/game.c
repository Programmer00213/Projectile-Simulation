#include <Custom/game.h>
#include <Custom/SDL_math.h>

// enable debug mode
void debug(SDL_Renderer *renderer, Object *projectile, Game_State *game_state)
{
    int scale = 50;

    float x_comp = projectile->u * cosf(projectile->angle);
    float y_comp = projectile->u * -sinf(projectile->angle) + projectile->g * projectile->t;

    float v = sqrt(powf(x_comp, 2) + powf(y_comp, 2));

    float new_x_comp = projectile->object_pos.x + scale * x_comp / v;
    float new_y_comp = projectile->object_pos.y + scale * y_comp / v;

    // float vector = sqrt(powf(x_comp, 2) + powf(y_comp, 2));
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_RenderLine(renderer,
                   projectile->object_pos.x,
                   projectile->object_pos.y,
                   new_x_comp,
                   projectile->object_pos.y);

    SDL_RenderLine(renderer,
                   projectile->object_pos.x,
                   projectile->object_pos.y,
                   projectile->object_pos.x,
                   new_y_comp);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderLine(renderer, projectile->object_pos.x, projectile->object_pos.y, new_x_comp, new_y_comp);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderLine(renderer, projectile->object_pos.x, projectile->object_pos.y, game_state->mouse_pos.x, game_state->mouse_pos.y);
}

// The Position of Projectile is Updated Here.
SDL_FPoint Move(Object *projectile, Game_State *game_state, SDL_State *state, float delta_time)
{
    if (projectile->eject)
        projectile->t += delta_time * projectile->slowmotion_factor;

    projectile->object_pos.x = projectile->dimensions.x + projectile->u * cosf(projectile->angle) * projectile->t;
    projectile->object_pos.y = projectile->dimensions.y + (-projectile->u * sinf(projectile->angle) * projectile->t + 0.5 * projectile->g * powf(projectile->t, 2));

    // get the direction/ sign of the projectile
    projectile->direction.x = projectile->object_pos.x - projectile->direction.x;
    projectile->direction.y = projectile->object_pos.y - projectile->direction.y;

    // This piece of code, repositions the projectile to the opposite side, when the projectile crosses the screen boundry, so that projectile stays in the viewport
    if (projectile->object_pos.x + projectile->dimensions.w <= 0 && projectile->direction.x <= 0)
    {
        projectile->dimensions.x = state->logical_Width;
        projectile->t = 0;
    }

    else if (projectile->object_pos.x - projectile->dimensions.w >= state->logical_Width && projectile->direction.x >= 0){
        projectile->dimensions.x = 0;
        projectile->t = 0;
    }

    if (projectile->object_pos.y + projectile->dimensions.w < 0 && projectile->direction.y < 0){
        projectile->dimensions.y = state->logical_Height;
        projectile->t = 0;
    }
    else if (projectile->object_pos.y - projectile->dimensions.w >= state->logical_Height && projectile->direction.y >= 0){
        projectile->dimensions.y = 2 * projectile->dimensions.w + 1;
        projectile->t = 0;
    }

    projectile->direction.x = projectile->object_pos.x;
    projectile->direction.y = projectile->object_pos.y;

    return projectile->object_pos;
}

void drawTrajectory(SDL_Renderer *renderer, SDL_FPoint position, float u, float angle, float g)
{
    SDL_FCircle dots = {{200, 700}, 6, {230, 230, 230, 158}};
    float step = 0;
    for (int i = 0; i < 9; i++)
    {
        dots.center.x = position.x + u * cosf(angle) * step;
        dots.center.y = position.y + (-u * sinf(angle) * step + 0.5 * g * powf(step, 2));
        dots.r -= 0.55;
        SDL_RenderCircle(renderer, &dots);
        step += 0.03;
    }
}

bool Collision_Response(Object *projectile, SDL_FRect *rect, Game_State *game_state)
{
    SDL_FPoint n = {0, -1};
    // Simple Normal Calculation
    if (projectile->object_pos.x < rect->x)
    {
        // Right Collision
        n.x = -1;
        n.y = 0;
    }
    else if (projectile->object_pos.x > rect->x + rect->w)
    {
        // Left Collision
        n.x = 1;
        n.y = 0;
    }
    if (projectile->object_pos.y < rect->y)
    {
        // Up Collision
        n.x = 0;
        n.y = -1;
    }
    else if (projectile->object_pos.y > rect->y + rect->h)
    {
        // Down Collision
        n.x = 0;
        n.y = 1;
    }

    // Position Correcting Code. After Collision
    if (n.x != 0)
    {
        if (n.x > 0)
            projectile->object_pos.x = rect->x + rect->w + projectile->dimensions.w;
        else
            projectile->object_pos.x = rect->x - projectile->dimensions.w;
    }
    if (n.y != 0)
    {
        if (n.y > 0)
            projectile->object_pos.y = rect->y + rect->h + projectile->dimensions.w;
        else
            projectile->object_pos.y = rect->y - projectile->dimensions.w;
    }

    // Reseting Projectile Position and Timer, For proper Re-Bounce of projectile.
    projectile->dimensions.y = projectile->object_pos.y;
    projectile->dimensions.x = projectile->object_pos.x;
    projectile->t = 0;

    // Calculating Component Of Final Velocity
    float x_comp = projectile->u * cosf(projectile->angle);
    float y_comp = projectile->u * -sinf(projectile->angle) + projectile->g * projectile->t;

    float v = sqrt(powf(x_comp, 2) + powf(y_comp, 2));
    float dot = x_comp * n.x + y_comp * n.y;

    // float rx = x_comp - (2*dot)*n.x;
    // float ry = y_comp - (2*dot)*n.y;

    float rx = x_comp - (1 + projectile->e) * dot * n.x;
    float ry = y_comp - (1 + projectile->e) * dot * n.y;

    // Passing New Velocity And Angle
    projectile->u = sqrt(powf(rx, 2) + powf(ry, 2));
    projectile->angle = atan2f(-ry, rx);
}

bool AABB_Collision(SDL_FRect *r, Object *obj, Game_State *game_state)
{
    // Find the closest point on the rectangle to the circle's center.
    // This may lie on a side, a corner, or inside the rectangle.
    float closest_X = fmax(r->x, fmin(obj->object_pos.x, r->x + r->w));
    float closest_Y = fmax(r->y, fmin(obj->object_pos.y, r->y + r->h));

    float dx = obj->object_pos.x - closest_X;
    float dy = obj->object_pos.y - closest_Y;

    // if distance between circle and rectangle is less than or equal to radius of the projectile/circle, then there is collision
    if (dx * dx + dy * dy <= obj->dimensions.w * obj->dimensions.w)
    {
        Collision_Response(obj, r, game_state);
    }

    return true;
}

//collision between circle
bool Circle_Collision(Object *obj1, Object *obj2, Game_State *game_state)
{
    float distance = sqrt(powf(obj1->object_pos.x - game_state->mouse_pos.x, 2) + powf(obj1->object_pos.y - game_state->mouse_pos.y, 2));
    if (distance <= obj1->dimensions.w + 5 && game_state->button_pressed && game_state->mouse_button == 3)
    {
        // SDL_Log("Collision with Mouse");
        obj1->dimensions.x = game_state->mouse_pos.x;
        obj1->dimensions.y = game_state->mouse_pos.y;
        obj1->u = 0;
        obj1->eject = false;

        obj1->t = 0;
    }

    return true;
}