#ifndef CONSTANTS_H
#define CONSTANTS_H

/// \file
/// \brief Constant values used throughout the program.

/// The width of the screen, in pixels.
#define DISPLAY_WIDTH 320

/// The height of the screen, in pixels.
#define DISPLAY_HEIGHT 240

/// The width of the table, in pixels.
#define TABLE_WIDTH 320

/// The height of the table, in pixels.
#define TABLE_HEIGHT 210

/// The Y coordinate where that table starts.
#define TABLE_Y (DISPLAY_HEIGHT - TABLE_HEIGHT)

/// The width and height of the ball, in pixels.
#define BALL_SIZE 4

/// The width of the paddle, in pixels.
#define PADDLE_WIDTH 4

/// The height of the paddle, in pixels.
#define PADDLE_HEIGHT 16

/// The maximum speed of the paddle, in pixels.
#define PADDLE_MAX_SPEED 4

/// The number of players in the game.
#define PLAYER_COUNT 2

/// The horizontal coordinates of the players' paddles.
extern const short player_x_coords[PLAYER_COUNT];

#endif
