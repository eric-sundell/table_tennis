#include "game.h"
#include <stddef.h>

const short player_x_coords[PLAYER_COUNT] = {8, TABLE_WIDTH - 8 - PADDLE_WIDTH};

void g_init(struct GameState *state)
{
    state->ball.x = TABLE_WIDTH / 2 - BALL_SIZE / 2;
    state->ball.y = TABLE_HEIGHT / 2 - BALL_SIZE / 2;

    for (size_t i = 0; i < PLAYER_COUNT; ++i)
    {
        state->players[i].score = 0;
        state->players[i].y = TABLE_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    }
}

void g_update(struct GameState *state, const PlayerInput *inputs)
{
    for (size_t i = 0; i < PLAYER_COUNT; ++i)
    {
        int new_y = state->players[i].y + inputs[i];
        if (new_y < 0)
            new_y = 0;
        else if (new_y > TABLE_HEIGHT - PADDLE_HEIGHT)
            new_y = TABLE_HEIGHT - PADDLE_HEIGHT;
        state->players[i].y = new_y;
    }
}
