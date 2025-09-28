#include <Custom/SDL_shape.h>
#include <Custom/SDL_math.h>

static SDL_FPoint pre_circle_vertex_coor[CIRCLE_VERTEX] = { // hashing circle vertex coordinate of unit circle, for fast circle drawing
{0,0},{0,-1},{0.214970, -0.976621},
{0.419889, -0.907575},{0.605174, -0.796093},{0.762162, -0.647386},
{0.883512, -0.468408},{0.963550, -0.267528},{0.998533, -0.054139},
{0.986826, 0.161782},{0.928977, 0.370138},{0.827689, 0.561187},
{0.687699, 0.725996},{0.515554, 0.856857},{0.319302, 0.947653},
{0.108119, 0.994138},{-0.108119, 0.994138},{-0.319302, 0.947653},
{-0.515554, 0.856857},{-0.687699, 0.725996},{-0.827689, 0.561187},
{-0.928977, 0.370138},{-0.986827, 0.161781},{-0.998533, -0.054139},
{-0.963550, -0.267528},{-0.883512, -0.468409},{-0.762162, -0.647386},
{-0.605174, -0.796093},{-0.419889, -0.907575},{-0.214970, -0.976621}
};

static int pre_circle_index[INDICES] = { // hashing circle vertex index, for fast circle drawing
0, 1, 2, 0, 2, 3,
0, 3, 4, 0, 4, 5,
0, 5, 6, 0, 6, 7,
0, 7, 8, 0, 8, 9,
0, 9, 10, 0, 10, 11,
0, 11, 12, 0, 12, 13,
0, 13, 14, 0, 14, 15,
0, 15, 16, 0, 16, 17,
0, 17, 18, 0, 18, 19,
0, 19, 20, 0, 20, 21,
0, 21, 22, 0, 22, 23,
0, 23, 24, 0, 24, 25,
0, 25, 26, 0, 26, 27,
0, 27, 28, 0, 28, 29,
0, 29, 1,
};

void SDL_RenderCircle(SDL_Renderer *renderer, SDL_FCircle *circle){
    SDL_Vertex vertex[CIRCLE_VERTEX];
    int circle_indices[INDICES];

    for (int i = 0; i < CIRCLE_VERTEX; i++){
        vertex[i].position.x = pre_circle_vertex_coor[i].x * circle->r + circle->center.x; 
        vertex[i].position.y = pre_circle_vertex_coor[i].y * circle->r + circle->center.y ;

        vertex[i].color.r = circle->color.r/255.0f;
        vertex[i].color.g = circle->color.g/255.0f;
        vertex[i].color.b = circle->color.b/255.0f;
        vertex[i].color.a = circle->color.a/255.0f;
    }

    SDL_RenderGeometry(renderer, NULL, vertex, CIRCLE_VERTEX, pre_circle_index, INDICES);
}

void SDL_RenderTriangle(SDL_Renderer *renderer, SDL_FTriangle *triangle){

    SDL_Vertex vertex[3];
    int triangle_indices[3] = {0, 1, 2};

    vertex[0].position = triangle->a; 

    vertex[1].position = triangle->b;

    vertex[2].position = triangle->c;

    for (int i = 0; i < 3; i++){
        vertex[i].color.r = triangle->color.r/255.0f;
        vertex[i].color.g = triangle->color.g/255.0f;
        vertex[i].color.b = triangle->color.b/255.0f;
        vertex[i].color.a = triangle->color.a/255.0f;
    }

    SDL_RenderGeometry(renderer, NULL, vertex, 3, triangle_indices, 3);
}