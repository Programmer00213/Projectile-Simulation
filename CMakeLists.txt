cmake_minimum_required(VERSION 3.10)
project(projectile)

set(CMAKE_C_STANDARD 17)

include_Directories(include)
link_directories(lib)

set(
    PROJECT_FILES
    main.c 
    src/utils/SDL_shape.c
    src/utils/SDL_math.c
    
    src/input/SDL_input.c
    src/game.c
)

add_executable(main ${PROJECT_FILES})
target_link_libraries(main SDL3 SDL3_image)