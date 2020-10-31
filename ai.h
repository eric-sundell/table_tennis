#ifndef AI_H
#define AI_H

#include "constants.h"
#include "game.h"
#include <stddef.h>

enum AIDifficulty
{
    AI_NONE = 0,
    AI_EASY = 1,
    AI_NORMAL = 2,
    AI_HARD = 8
};

extern enum AIDifficulty ai_difficulties[PLAYER_COUNT];

PlayerInput ai_determine_input(const struct GameState *state, size_t player_index);

#endif
