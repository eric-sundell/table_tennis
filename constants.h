#ifndef CONSTANTS_H
#define CONSTANTS_H

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
/// \brief Constant values used throughout the program.

/// The width of the screen, in pixels.
#define DISPLAY_WIDTH 320

/// The height of the screen, in pixels.
#define DISPLAY_HEIGHT 240

/// The width of the table, in pixels.
#define TABLE_WIDTH 320

/// The height of the table, in pixels.
#define TABLE_HEIGHT 210

/// The Y coordinate where that table starts.
#define TABLE_Y (DISPLAY_HEIGHT - TABLE_HEIGHT)

/// The width and height of the ball, in pixels.
#define BALL_SIZE 4

/// The width of the paddle, in pixels.
#define PADDLE_WIDTH 4

/// The height of the paddle, in pixels.
#define PADDLE_HEIGHT 16

/// The maximum speed of the paddle, in pixels.
#define PADDLE_MAX_SPEED 4

/// The number of players in the game.
#define PLAYER_COUNT 2

/// The horizontal coordinates of the players' paddles.
extern const short player_x_coords[PLAYER_COUNT];

#endif
