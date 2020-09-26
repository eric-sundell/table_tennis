#include "game.h"
#include "renderer.h"
#include "util.h"
#include <SDL.h>
#include <stdbool.h>
#include <stddef.h>

#define FRAME_TIME (1000/60)

static void read_inputs(PlayerInput *inputs)
{
    const int paddle_speed = 5;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    int p1_mult = keys[SDL_SCANCODE_LSHIFT] ? 2 : 1;
    int p2_mult = keys[SDL_SCANCODE_RSHIFT] ? 2 : 1;
    if (keys[SDL_SCANCODE_W])
        inputs[0] -= paddle_speed * p1_mult;
    if (keys[SDL_SCANCODE_S])
        inputs[0] += paddle_speed * p1_mult;
    if (keys[SDL_SCANCODE_UP])
        inputs[1] -= paddle_speed * p2_mult;
    if (keys[SDL_SCANCODE_DOWN])
        inputs[1] += paddle_speed * p2_mult;
}

static bool main_loop(void)
{
    struct GameState game_state;
    
    g_init(&game_state);
    Uint32 last_frame = SDL_GetTicks();
    int remaining_time = 0;
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

        Uint32 current_frame = SDL_GetTicks();
        int elapsed = current_frame - last_frame;
        remaining_time += elapsed;
        last_frame = current_frame;
        PlayerInput inputs[PLAYER_COUNT] = {0};
        read_inputs(inputs);
        while (remaining_time >= FRAME_TIME)
        {
            g_update(&game_state, inputs);
            remaining_time -= FRAME_TIME;
        }

        if (!r_draw_frame(&game_state))
            return false;
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
