#include<Custom/SDL_math.h>

SDL_FPoint Rotation(SDL_FPoint center, SDL_FPoint coordinate, float angle){
    angle = MATH_PI * angle / 180.0f;

    coordinate.x = coordinate.x - center.x;
    coordinate.y = coordinate.y - center.y;

    float x = coordinate.x * cosf(angle) - coordinate.y * sinf(angle);
    float y = coordinate.y * cosf(angle) + coordinate.x * sinf(angle);

    coordinate.x = x + center.x;
    coordinate.y = y + center.y;

    return coordinate;
}

float get_Distance(SDL_FPoint p1, SDL_FPoint p2){
    return sqrt(powf((p1.x - p2.x),2) + powf((p1.y-p2.y),2));
}

float get_Angle(SDL_FPoint p1, SDL_FPoint p2){
    return atan2f(-(p2.y - p1.y), p2.x - p1.x);
}