#include "util.h"
#include <SDL.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void u_display_sdl_error(void)
{
    const char *msg = SDL_GetError();
    size_t msg_len = strlen(msg) + 1;
    // Create a copy of the error message, so that we can still use it after a
    // failed call to SDL_ShowSimpleMessageBox
    char *copy = malloc(msg_len);
    if (copy)
    {
        memcpy(copy, msg, msg_len);
        int result = SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "SDL Error",
            copy,
            NULL
        );
        if (!result)
        {
            fprintf(stderr, "SDL error: %s\n", copy);
        }
        free(copy);
    }
    else
    {
        fprintf(stderr, "SDL error: %s\n", msg);
    }
}
