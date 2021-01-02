#ifndef RENDERER_H
#define RENDERER_H

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
/// \brief Functionality exported by the renderer module.

#include "game.h"
#include <stdbool.h>

/// Initializes the renderer.
/// \param[in]  use_vsync   Whether V-sync should be used.
/// \returns True if initialization was successful, false otherwise.
bool r_init(bool use_vsync);

/// Draws the game state to the screen.
/// \param[in]  state   The state to render.
/// \returns True if drawing was successful, false otherwise.
bool r_draw_frame(const struct GameState *state);

/// Releases resources used by the renderer.
void r_quit(void);

#endif
