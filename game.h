#ifndef GAME_H
#define GAME_H

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
/// \brief Functionality exported by the gameplay module.

#include "constants.h"

/// Contains the current state of the ball.
struct Ball
{
    /// The ball's horizontal position.
    short x_coord;

    /// The ball's vertical position.
    short y_coord;

    /// The ball's horizontal direction.
    short dir_x;

    /// The ball's vertical direction.
    short dir_y;

    /// The ball's speed.
    short speed;
};

/// Represents how far a player wants to move their paddle.
typedef signed char PlayerInput;

/// Contains information about a player.
struct PlayerState
{
    /// The player's score.
    unsigned char score;

    /// The vertical position of the player's paddle.
    unsigned char y;
};

/// Contains the state of the game table.
struct GameState
{
    /// The game's ball.
    struct Ball ball;

    /// The players' information.
    struct PlayerState players[PLAYER_COUNT];
};

/// Initializes the game state's members to their initial values.
/// \param[out] state   The state to initialize.
void g_init(struct GameState *state);

/// Updates the game's state by one frame, taking into account the players'
/// inputs.
/// \param[in]  state   The state to update.
/// \param[in]  inputs  The players' inputs.
void g_update(struct GameState *state, const PlayerInput *inputs);

#endif
