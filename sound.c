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
/// \brief Implementation of the sound module.

#include "sound.h"
#include "util.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/// Sample for the bounce sound effect.
static Mix_Chunk *bounce_sample = NULL;

/// Sample for the score sound effect.
static Mix_Chunk *score_sample = NULL;

/// Returns the concatenation of two strings.
/// \param[in]  a   The first string.
/// \param[in]  b   The second string.
/// \returns    The concatenated string.
static char *concat(const char *a, const char *b)
{
    size_t a_len = strlen(a);
    size_t b_len = strlen(b) + 1;
    char *buffer = malloc(a_len + b_len);
    if (buffer)
    {
        memcpy(buffer, a, a_len);
        memcpy(buffer + a_len, b, b_len);
    }
    return buffer;
}

/// Plays a sound sample.
/// \param[in]  sample  The sample to play.
static void play_sample(Mix_Chunk *sample)
{
    int channel = Mix_PlayChannel(-1, sample, 0);
    if (channel == -1)
    {
        SDL_LogWarn(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to play sound: %s",
            Mix_GetError());
    }
}

bool s_init(void)
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024))
    {
        u_display_error(Mix_GetError(), "SDL Mixer Error");
        return false;
    }

    char *base_path = SDL_GetBasePath();
    char *path = NULL;
    if (!base_path)
    {
        u_display_sdl_error();
        goto error;
    }

    path = concat(base_path, "sounds/bounce.wav");
    if (!path)
    {
        u_display_error(strerror(errno), "Error");
        goto error;
    }
    bounce_sample = Mix_LoadWAV(path);
    if (!bounce_sample)
    {
        u_display_error(Mix_GetError(), "SDL Mixer Error");
        goto error;
    }
    free(path);

    path = concat(base_path, "sounds/score.wav");
    if (!path)
    {
        u_display_error(strerror(errno), "Error");
        goto error;
    }
    score_sample = Mix_LoadWAV(path);
    if (!bounce_sample)
    {
        u_display_error(Mix_GetError(), "SDL Mixer Error");
        goto error;
    }
    free(path);

    SDL_free(base_path);
    return true;

    error:
    free(path);
    if (base_path)
    {
        SDL_free(base_path);
    }
    s_quit();
    return false;
}

void s_play_bounce(void)
{
    play_sample(bounce_sample);
}

void s_play_score(void)
{
    play_sample(score_sample);
}

void s_quit(void)
{
    if (bounce_sample)
    {
        Mix_FreeChunk(bounce_sample);
        bounce_sample = NULL;
    }
    if (score_sample)
    {
        Mix_FreeChunk(score_sample);
        score_sample = NULL;
    }
    Mix_CloseAudio();
}
