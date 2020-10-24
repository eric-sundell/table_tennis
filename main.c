#include "ai.h"
#include "game.h"
#include "renderer.h"
#include "util.h"
#include <SDL.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FRAME_TIME (1000/60)

static bool p1_ai = false;
static bool p2_ai = false;

static const char * const help_text =
"Options:\n"
"--ai1\t\tLeft player is controlled by AI\n"
"--ai2\t\tRight player is controlled by AI\n"
"--easy\t\tSets the AI difficulty to easy\n"
"--normal\tSets the AI difficulty to normal\n"
"--hard\t\tSets the AI difficulty to hard";

static void parse_args(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "--ai1") == 0)
            p1_ai = true;
        else if (strcmp(argv[i], "--ai2") == 0)
            p2_ai = true;
        else if (strcmp(argv[i], "--easy") == 0)
            ai_set_difficulty(AI_EASY);
        else if (strcmp(argv[i], "--normal") == 0)
            ai_set_difficulty(AI_NORMAL);
        else if (strcmp(argv[i], "--hard") == 0)
            ai_set_difficulty(AI_HARD);
        else if (strcmp(argv[i], "--help") == 0)
        {
            puts(help_text);
            exit(EXIT_SUCCESS);
        }
        else
        {
            fprintf(stderr, "%s: Unrecognized option '%s'\n", argv[0], argv[i]);
            exit(EXIT_FAILURE);
        }
    }
}

static void read_inputs(PlayerInput *inputs)
{
    const int paddle_speed = PADDLE_MAX_SPEED / 2;
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
        if (p1_ai)
            inputs[0] = ai_determine_input(&game_state, 0);
        if (p2_ai)
            inputs[1] = ai_determine_input(&game_state, 1);
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
    parse_args(argc, argv);

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
