#include "game.h"
#include "coord.h"
#include <stddef.h>
#include <stdlib.h>

const short player_x_coords[PLAYER_COUNT] = {8, TABLE_WIDTH - 8 - PADDLE_WIDTH};

static void move_ball(struct GameState *state);

static void reset_ball(struct Ball *ball, int dir_x);

static void inc_score(unsigned char *score)
{
    unsigned new_score = *score + 1;
    if (new_score >= 100)
        new_score = 99;
    *score = new_score;
}

void g_init(struct GameState *state)
{
    reset_ball(&(state->ball), -1);

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
    int new_x = state->ball.x_coord + vel_x;
    int new_y = state->ball.y_coord + vel_y;

    // Left edge collision
    if (new_x < 0)
    {
        inc_score(&(state->players[1].score));
        reset_ball(&(state->ball), 1);
        return;
    }

    // Right edge collision
    if (coord_to_int(new_x) + BALL_SIZE > TABLE_WIDTH)
    {
        inc_score(&(state->players[0].score));
        reset_ball(&(state->ball), -1);
        return;
    }

    // Top edge collision
    if (new_y < 0)
    {
        state->ball.dir_y = -state->ball.dir_y;
        new_y = 0;
    }

    // Bottom edge collision
    if (coord_to_int(new_y) + BALL_SIZE > TABLE_HEIGHT)
    {
        state->ball.dir_y = -state->ball.dir_y;
        new_y = coord_from_int(TABLE_HEIGHT - BALL_SIZE);
    }

    state->ball.x_coord = new_x;
    state->ball.y_coord = new_y;
}

static void reset_ball(struct Ball *ball, int dir_x)
{
    ball->x_coord = coord_from_int(TABLE_WIDTH / 2 - BALL_SIZE / 2);
    ball->y_coord = coord_from_int(10);
    ball->dir_x = dir_x;
    ball->dir_y = 1;
    ball->speed = 1;
}
