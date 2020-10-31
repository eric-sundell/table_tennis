#include "ai.h"
#include "constants.h"
#include "coord.h"
#include <stdlib.h>

enum AIDifficulty ai_difficulties[PLAYER_COUNT];

PlayerInput ai_determine_input(const struct GameState *state, size_t player_index)
{
    enum AIDifficulty difficulty = ai_difficulties[player_index];
    int x_dist = abs(
        coord_to_int(state->ball.x_coord) + BALL_SIZE / 2
        - (player_x_coords[player_index] + PADDLE_WIDTH / 2));
    int y_diff = coord_to_int(state->ball.y_coord) + BALL_SIZE / 2
        - (state->players[player_index].y + PADDLE_HEIGHT / 2);
    int dir = y_diff;
    x_dist /= difficulty;
    if (x_dist > 0)
        dir /= x_dist;
    if (dir > PADDLE_MAX_SPEED)
        dir = PADDLE_MAX_SPEED;
    else if (dir < -PADDLE_MAX_SPEED)
        dir = -PADDLE_MAX_SPEED;
    return dir;
}
