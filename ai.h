#ifndef AI_H
#define AI_H

#include "game.h"
#include <stddef.h>

enum AIDifficulty
{
    AI_EASY = 1,
    AI_NORMAL = 2,
    AI_HARD = 8
};

PlayerInput ai_determine_input(const struct GameState *state, size_t player_index);

void ai_set_difficulty(enum AIDifficulty new_diff);

#endif
