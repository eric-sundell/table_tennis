#include "game.h"
#include "coord.h"
#include <stddef.h>
#include <stdlib.h>

const short player_x_coords[PLAYER_COUNT] = {8, TABLE_WIDTH - 8 - PADDLE_WIDTH};

static void move_ball(struct GameState *state);

void g_init(struct GameState *state)
{
    state->ball.x_coord = coord_from_int(TABLE_WIDTH / 2 - BALL_SIZE / 2);
    state->ball.y_coord = coord_from_int(10);
    state->ball.dir_x = 1;
    state->ball.dir_y = 1;
    state->ball.speed = 1;

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

    move_ball(state);
}

static void move_ball(struct GameState *state)
{
    int speed = coord_from_int(state->ball.speed);
    int denom = abs(state->ball.dir_x) + abs(state->ball.dir_y);
    int vel_x = coord_mul_frac(speed, state->ball.dir_x, denom);
    int vel_y = coord_mul_frac(speed, state->ball.dir_y, denom);
    state->ball.x_coord += vel_x;
    state->ball.y_coord += vel_y;
}
