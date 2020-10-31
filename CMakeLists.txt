cmake_minimum_required(VERSION 3.16)

include(FindPkgConfig)

project(table_tennis C)

set(CMAKE_C_STANDARD 99)

find_package(SDL2 REQUIRED)
pkg_search_module(SDL2_MIXER REQUIRED SDL2_mixer)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

add_executable(table_tennis
    main.c
    ai.h ai.c
    constants.h
    coord.h coord.c
    game.h game.c
    renderer.h renderer.c
    sound.h sound.c
    util.h util.c)
set_property(TARGET table_tennis PROPERTY C_EXTENSIONS OFF)
if(MSVC)
    target_compile_options(table_tennis PRIVATE /W4)
else()
    target_compile_options(table_tennis PRIVATE -Wall -Wextra -pedantic)
endif()
target_link_libraries(table_tennis ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
configure_file(sounds/bounce.wav sounds/bounce.wav COPYONLY)
