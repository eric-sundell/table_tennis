#include "sound.h"
#include "util.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static Mix_Chunk *bounce_sample = NULL;

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

    SDL_free(base_path);
    return true;

    error:
    free(path);
    if (base_path)
    {
        SDL_free(base_path);
    }
    if (bounce_sample)
    {
        Mix_FreeChunk(bounce_sample);
        bounce_sample = NULL;
    }
    Mix_CloseAudio();
    return false;
}

void s_play_bounce(void)
{
    int channel = Mix_PlayChannel(-1, bounce_sample, 0);
    if (channel == -1)
    {
        SDL_LogWarn(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to play sound: %s",
            Mix_GetError());
    }
}

void s_quit(void)
{
    if (bounce_sample)
    {
        Mix_FreeChunk(bounce_sample);
        bounce_sample = NULL;
    }
    Mix_CloseAudio();
}
