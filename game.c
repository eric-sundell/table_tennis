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
/// \brief Implementation of the gameplay module.

#include "game.h"
#include "coord.h"
#include "sound.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

const short player_x_coords[PLAYER_COUNT] = {8, TABLE_WIDTH - 8 - PADDLE_WIDTH};

static void move_ball(struct GameState *state);

static void reset_ball(struct Ball *ball, int dir_x);

/// Computes the GCD of two numbers.
/// \param[in]  a   The first number.
/// \param[in]  b   The second number.
/// \returns    The GCD of the two numbers.
static int gcd(int a, int b)
{
    if (a == 0 || b == 0)
        return 1;
    a = abs(a);
    b = abs(b);

    if (a == b)
        return a;
    else if (a > b)
        return gcd(a - b, b);
    else
        return gcd(a, b - a);
}

/// Increases a score, clamping if required.
/// \param[in]  score   The original score.
/// \returns    The new score.
static unsigned char inc_score(unsigned char score)
{
    unsigned new_score = score + 1;
    if (new_score >= 100)
        new_score = 99;
    return new_score;
}

/// Determines if the ball is colliding with the paddle.
/// \param[in]  pad_x   The paddle's X coordinate.
/// \param[in]  pad_y   The paddle's Y coordinate.
/// \param[in]  ball_x  The ball's X coordinate.
/// \param[in]  ball_y  The ball's Y coordinate.
/// \returns    Whether the ball and paddle are colliding.
static bool paddle_collide(int pad_x, int pad_y, int ball_x, int ball_y)
{
    return pad_x < ball_x + BALL_SIZE
        && pad_x + PADDLE_WIDTH > ball_x
        && pad_y < ball_y + BALL_SIZE
        && pad_y + PADDLE_HEIGHT > ball_y;
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

/// Updates the ball's position and resolves collisions.
/// \param[in]  state   The game state.
static void move_ball(struct GameState *state)
{
    int speed = state->ball.speed;
    int denom = abs(state->ball.dir_x) + abs(state->ball.dir_y);
    int vel_x = coord_mul_frac(speed, state->ball.dir_x, denom);
    int vel_y = coord_mul_frac(speed, state->ball.dir_y, denom);
    int new_x = state->ball.x_coord + vel_x;
    int new_y = state->ball.y_coord + vel_y;

    // Left edge collision
    if (new_x < 0)
    {
        state->players[1].score = inc_score(state->players[1].score);
        reset_ball(&(state->ball), 1);
        s_play_score();
        return;
    }

    // Right edge collision
    if (coord_to_int(new_x) + BALL_SIZE > TABLE_WIDTH)
    {
        state->players[0].score = inc_score(state->players[0].score);
        reset_ball(&(state->ball), -1);
        s_play_score();
        return;
    }

    // Top edge collision
    if (new_y < 0)
    {
        state->ball.dir_y = -state->ball.dir_y;
        new_y = 0;
        s_play_bounce();
    }

    // Bottom edge collision
    if (coord_to_int(new_y) + BALL_SIZE > TABLE_HEIGHT)
    {
        state->ball.dir_y = -state->ball.dir_y;
        new_y = coord_from_int(TABLE_HEIGHT - BALL_SIZE);
        s_play_bounce();
    }

    // Paddle collisions
    for (int i = 0; i < PLAYER_COUNT; ++i)
    {
        if (paddle_collide(player_x_coords[i],
            state->players[i].y,
            coord_to_int(new_x),
            coord_to_int(new_y)))
        {
            int dir_x = coord_to_int(new_x) + BALL_SIZE / 2
                - (player_x_coords[i] + PADDLE_WIDTH / 2);
            int dir_y = coord_to_int(new_y) + BALL_SIZE / 2
                - (state->players[i].y + PADDLE_HEIGHT / 2);
            if (dir_x == 0)
                dir_x = i == 0 ? 1 : -1;
            int divisor = gcd(dir_x, dir_y);
            dir_x /= divisor;
            dir_y /= divisor;
            state->ball.dir_x = dir_x;
            state->ball.dir_y = dir_y;
            state->ball.speed += COORD_SCALE / 2;
            // Cap speed to prevent collision glitches
            const int max_speed = coord_from_int(6);
            if (state->ball.speed > max_speed)
                state->ball.speed = max_speed;
            s_play_bounce();
            break;
        }
    }

    state->ball.x_coord = new_x;
    state->ball.y_coord = new_y;
}

/// Returns the ball to its starting position.
/// \param[out] ball    The ball.
/// \param[in]  dir_x   The X direction the ball should travel.
static void reset_ball(struct Ball *ball, int dir_x)
{
    const int min_x = 50;
    const int max_x = 160;
    int rand_x = rand() / (RAND_MAX / (max_x - min_x) + 1) + min_x;
    ball->x_coord = coord_from_int(TABLE_WIDTH / 2 - BALL_SIZE / 2);
    ball->y_coord = coord_from_int(10);
    ball->dir_x = dir_x * rand_x;
    ball->dir_y = 64;
    ball->speed = coord_from_int(1);
}
