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
/// \brief Implementation of common utility functions.

#include "util.h"
#include <SDL.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void u_display_error(const char *message, const char *title)
{
    int result = SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR,
        title,
        message,
        NULL
    );
    if (!result)
    {
        fprintf(stderr, "%s: %s\n", title, message);
    }
}

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
        u_display_error(copy, "SDL Error");
        free(copy);
    }
    else
    {
        fprintf(stderr, "SDL error: %s\n", msg);
    }
}
