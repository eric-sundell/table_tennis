#ifndef AI_H
#define AI_H

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
