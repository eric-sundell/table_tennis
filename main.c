#include "game.h"
#include "renderer.h"
#include "util.h"
#include <SDL.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool main_loop(void)
{
    struct GameState game_state;
    g_init(&game_state);
    while (1)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_QUIT:
                    return true;
            }
        }

        r_draw_frame(&game_state);
    }
}

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        u_display_sdl_error();
        return EXIT_FAILURE;
    }

    if (!r_init())
    {
        SDL_Quit();
        return EXIT_FAILURE;
    }

    bool successful_exit = main_loop();

    SDL_Quit();
    return successful_exit ? EXIT_SUCCESS : EXIT_FAILURE;
}
