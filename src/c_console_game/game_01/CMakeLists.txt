cmake_minimum_required(VERSION 3.14)
project(game_01 C)

set(CMAKE_C_STANDARD 99)
add_executable(game_01
        game_01.c
        mac_util.c)
target_link_libraries(${PROJECT_NAME} ncurses)
