#ifndef AI_H
#define AI_H

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
/// \brief Functionality exported by the AI module.

#include "constants.h"
#include "game.h"
#include <stddef.h>

/// Represents the "intelligence" of an AI opponent.
enum AIDifficulty
{
    AI_NONE = 0,
    AI_EASY = 1,
    AI_NORMAL = 2,
    AI_HARD = 8
};

/// The difficulties of the game's players.
extern enum AIDifficulty ai_difficulties[PLAYER_COUNT];

/// Calculates a player's input in response to the current game state.
/// \param[in]  state           The current game state.
/// \param[in]  player_index    The index of the AI player.
/// \returns    The AI player's input.
PlayerInput ai_determine_input(const struct GameState *state, size_t player_index);

#endif
