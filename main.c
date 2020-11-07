#include "ai.h"
#include "game.h"
#include "renderer.h"
#include "sound.h"
#include "util.h"
#include <SDL.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FRAME_TIME (1000/60)

static const char * const help_text =
"Options:\n"
"--player1=<difficulty>\tSets the AI difficulty for player 1\n"
"--player2=<difficulty>\tSets the AI difficulty for player 2\n"
"\n<difficulty> is one of:\n"
"\tnone\tThe player is not AI-controlled\n"
"\teasy\n"
"\tnormal\n"
"\thard\n";

static SDL_GameController *controllers[PLAYER_COUNT];

static bool starts_with(const char *str, const char *prefix)
{
    size_t prefix_len = strlen(prefix);
    return strncmp(str, prefix, prefix_len) == 0;
}

static enum AIDifficulty extract_difficulty(const char *arg)
{
    const char *equals = strchr(arg, '=');
    const char *diff = equals + 1;
    if (strcmp(diff, "none") == 0)
        return AI_NONE;
    else if (strcmp(diff, "easy") == 0)
        return AI_EASY;
    else if (strcmp(diff, "normal") == 0)
        return AI_NORMAL;
    else if (strcmp(diff, "hard") == 0)
        return AI_HARD;
    else
    {
        fprintf(stderr, "Unrecognized difficulty '%s'\n", diff);
        exit(EXIT_FAILURE);
    }
    
}

static void parse_args(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        if (starts_with(argv[i], "--player1="))
        {
            ai_difficulties[0] = extract_difficulty(argv[i]);
        }
        else if (starts_with(argv[i], "--player2="))
        {
            ai_difficulties[1] = extract_difficulty(argv[i]);
        }
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

static void close_controllers(void)
{
    for (size_t i = 0; i < PLAYER_COUNT; ++i)
    {
        if (controllers[i])
        {
            SDL_GameControllerClose(controllers[i]);
            controllers[i] = NULL;
        }
    }
}

static bool add_controller(int index)
{
    for (size_t i = 0; i < PLAYER_COUNT; ++i)
    {
        if (!controllers[i])
        {
            controllers[i] = SDL_GameControllerOpen(index);
            if (!controllers[i])
            {
                u_display_sdl_error();
                return false;
            }
            SDL_Log(
                "Added controller '%s' for player %u",
                SDL_GameControllerName(controllers[i]),
                (unsigned)i + 1u);
            break;
        }
    }
    return true;
}

static void remove_controller(SDL_JoystickID id)
{
    SDL_GameController *removed = SDL_GameControllerFromInstanceID(id);
    if (removed)
    {
        for (size_t i = 0; i < PLAYER_COUNT; ++i)
        {
            if (controllers[i] == removed)
            {
                SDL_GameControllerClose(removed);
                controllers[i] = NULL;
                SDL_Log(
                    "Removed player %u's controller (ID %d)",
                    (unsigned)i + 1u,
                    (int)id);
                return;
            }
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

    for (size_t i = 0; i < PLAYER_COUNT; ++i)
    {
        if (!controllers[i])
            continue;
        
        int value = SDL_GameControllerGetAxis(
            controllers[i],
            SDL_CONTROLLER_AXIS_LEFTY);
        // Make input symmetric
        if (value < 0)
            value += 1;
        // Deadzone
        value /= 100;
        // Normalize
        const int maxAxisValue = 32767 / 100;
        value = value * PADDLE_MAX_SPEED / maxAxisValue;
        if (value != 0)
            inputs[i] = (PlayerInput)value;
    }
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
                case SDL_CONTROLLERDEVICEADDED:
                    if (!add_controller(e.cdevice.which))
                        return false;
                    break;
                case SDL_CONTROLLERDEVICEREMOVED:
                    remove_controller(e.cdevice.which);
                    break;
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
        if (ai_difficulties[0] != AI_NONE)
            inputs[0] = ai_determine_input(&game_state, 0);
        if (ai_difficulties[1] != AI_NONE)
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
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_GAMECONTROLLER) != 0)
    {
        u_display_sdl_error();
        return EXIT_FAILURE;
    }
    atexit(SDL_Quit);

    if (!s_init())
        return EXIT_FAILURE;
    atexit(s_quit);
    if (!r_init())
        return EXIT_FAILURE;

    atexit(close_controllers);

    bool successful_exit = main_loop();

    return successful_exit ? EXIT_SUCCESS : EXIT_FAILURE;
}
