#ifndef GAME_H
#define GAME_H

#include "constants.h"

struct Ball
{
    short x_coord;
    short y_coord;
    short dir_x;
    short dir_y;
    short speed;
};

typedef signed char PlayerInput;

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

void g_update(struct GameState *state, const PlayerInput *inputs);

#endif
