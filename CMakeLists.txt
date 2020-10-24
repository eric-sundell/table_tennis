cmake_minimum_required(VERSION 3.16)

project(table_tennis C)

set(CMAKE_C_STANDARD 99)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(table_tennis main.c ai.h ai.c constants.h coord.h coord.c game.h game.c renderer.h renderer.c util.h util.c)
set_property(TARGET table_tennis PROPERTY C_EXTENSIONS OFF)
target_link_libraries(table_tennis ${SDL2_LIBRARIES})
