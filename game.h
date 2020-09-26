#ifndef GAME_H
#define GAME_H

#include "constants.h"

struct Ball
{
    short x;
    short y;
};

struct PlayerState
{
    unsigned char score;
    unsigned char y;
};

struct GameState
{
    struct Ball ball;
    struct PlayerState players[PLAYER_COUNT];
};

void g_init(struct GameState *state);

#endif
