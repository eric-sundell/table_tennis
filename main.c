/*
table_tennis - A simple two player game
Copyright (C) 2021  Eric Sundell

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/// \file
/// \brief Program entry point.

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

/// The length of one frame (in milliseconds).
#define FRAME_TIME (1000/60)

/// The help text displayed when the `--help` option is provided.
static const char * const help_text =
"Options:\n"
"--vsync\t\tEnables vertical synchronization\n"
"--player1=<difficulty>\tSets the AI difficulty for player 1\n"
"--player2=<difficulty>\tSets the AI difficulty for player 2\n"
"\n<difficulty> is one of:\n"
"\tnone\tThe player is not AI-controlled\n"
"\teasy\n"
"\tnormal\n"
"\thard\n";

/// Contains user-supplied options.
struct GameOptions
{
    /// Whether V-sync should be used.
    bool use_vsync;
};

/// The controllers (if any) used by the players.
static SDL_GameController *controllers[PLAYER_COUNT];

/// Determines if a string begins with a prefix.
/// \param[in]  str     The string to search.
/// \param[in]  prefix  The string to search for.
/// \returns    Whether the string begins with the prefix.
static bool starts_with(const char *str, const char *prefix)
{
    size_t prefix_len = strlen(prefix);
    return strncmp(str, prefix, prefix_len) == 0;
}

/// Gets the difficulty after the equals sign in the given string.
/// \param[in]  arg The argument text.
/// \returns    The parsed difficulty.
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

/// Parses the program's command line arguments.
/// \param[in]  argc    The number of arguments.
/// \param[in]  argv    The argument values.
/// \returns Parsed options.
static struct GameOptions parse_args(int argc, char **argv)
{
    struct GameOptions options = { false };
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
        else if (strcmp(argv[i], "--vsync") == 0)
        {
            options.use_vsync = true;
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
    return options;
}

/// Closes all open controllers.
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

/// Assigns the controller with the given index to a player.
/// \param[in]  index   The controller's index.
/// \returns True if the controller was added successfully, false otherwise.
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

/// Closes a controller that has been removed.
/// \param[in]  id  The controller's joystick ID.
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

/// Reads the players' inputs from the keyboard and attached controllers.
/// \param[out] inputs  The players' inputs.
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

/// The game loop.
/// \returns True if the loop finished without errors, false otherwise.
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

/// Program entry point.
/// \param[in]  argc    The number of arguments.
/// \param[in]  argv    The argument values.
/// \returns    The exit status.
int main(int argc, char **argv)
{
    struct GameOptions options = parse_args(argc, argv);
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
    if (!r_init(options.use_vsync))
        return EXIT_FAILURE;
    atexit(r_quit);

    atexit(close_controllers);

    bool successful_exit = main_loop();

    return successful_exit ? EXIT_SUCCESS : EXIT_FAILURE;
}
