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
