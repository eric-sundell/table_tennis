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
/// \brief Implementation of the renderer module.

#include "renderer.h"
#include "constants.h"
#include "coord.h"
#include "util.h"
#include <SDL.h>
#include <stddef.h>
#include <stdio.h>

/// The thickness of the score numbers, in pixels.
#define SCORE_THICKNESS 3

/// Displays an SDL error and returns false if \a expr is not zero.
/// \param  expr    The expression to check.
#define CHECK_RESULT(expr) if (expr) {\
    u_display_sdl_error();\
    return false;\
}

/// The table rectangle.
static const SDL_Rect table =
{
    0, TABLE_Y,
    TABLE_WIDTH, TABLE_HEIGHT
};

static bool draw_paddles(const struct PlayerState *players);

static bool draw_table(void);

static bool draw_score(unsigned score, int x);

static bool draw_0(int x);
static bool draw_1(int x);
static bool draw_2(int x);
static bool draw_3(int x);
static bool draw_4(int x);
static bool draw_5(int x);
static bool draw_6(int x);
static bool draw_7(int x);
static bool draw_8(int x);
static bool draw_9(int x);

/// Function pointer to digit drawing function.
typedef bool (*DrawFunc)(int x);

/// The drawing functions for digits.
static const DrawFunc draw_funcs[] =
{
    draw_0,
    draw_1,
    draw_2,
    draw_3,
    draw_4,
    draw_5,
    draw_6,
    draw_7,
    draw_8,
    draw_9
};

static SDL_Window *window;

/// The SDL renderer.
static SDL_Renderer *renderer;

bool r_init(bool use_vsync)
{
    window = SDL_CreateWindow(
        "Table Tennis",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED
    );
    if (!window)
    {
        u_display_sdl_error();
        return false;
    }
    
    Uint32 flags = 0;
    flags |= use_vsync ? SDL_RENDERER_PRESENTVSYNC : 0;
    renderer = SDL_CreateRenderer(
        window,
        -1,
        flags
    );
    if (!renderer)
    {
        u_display_sdl_error();
        SDL_DestroyWindow(window);
        return false;
    }

    if (SDL_RenderSetLogicalSize(renderer, DISPLAY_WIDTH, DISPLAY_HEIGHT))
    {
        u_display_sdl_error();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return false;
    }

    if (SDL_RenderSetIntegerScale(renderer, SDL_TRUE))
    {
        u_display_sdl_error();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return false;
    }
    
    return true;
}

bool r_draw_frame(const struct GameState *state)
{
    CHECK_RESULT(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255))
    CHECK_RESULT(SDL_RenderClear(renderer))

    CHECK_RESULT(SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255))
    if (!draw_table())
        return false;

    CHECK_RESULT(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255))
    if (!draw_score(state->players[0].score, SCORE_THICKNESS))
        return false;
    if (!draw_score(state->players[1].score, DISPLAY_WIDTH - SCORE_THICKNESS * 10))
        return false;

    SDL_Rect ball =
    {
        coord_to_int(state->ball.x_coord),
        coord_to_int(state->ball.y_coord) + TABLE_Y,
        BALL_SIZE, BALL_SIZE
    };
    CHECK_RESULT(SDL_RenderFillRect(renderer, &ball))

    if (!draw_paddles(state->players))
        return false;

    SDL_RenderPresent(renderer);

    return true;
}

void r_quit(void)
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }
}

/// Draws the players' paddles.
/// \param[in]  players The players to draw.
/// \returns    True if successful, false otherwise.
static bool draw_paddles(const struct PlayerState *players)
{
    SDL_Rect paddles[PLAYER_COUNT];
    for (size_t i = 0; i < PLAYER_COUNT; ++i)
    {
        paddles[i].x = player_x_coords[i];
        paddles[i].y = players[i].y + TABLE_Y;
        paddles[i].w = PADDLE_WIDTH;
        paddles[i].h = PADDLE_HEIGHT;
    }
    CHECK_RESULT(SDL_RenderFillRects(renderer, paddles, PLAYER_COUNT))

    return true;
}

/// Draws the table.
/// \returns    True if successful, false otherwise.
static bool draw_table(void)
{
    CHECK_RESULT(SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255))
    CHECK_RESULT(SDL_RenderDrawRect(renderer, &table))
    
    for (int y = 0; y < TABLE_HEIGHT - 2; y += 16)
    {
        SDL_Rect line = {TABLE_WIDTH / 2 - 1, TABLE_Y + 3 + y, 2, 12};
        CHECK_RESULT(SDL_RenderFillRect(renderer, &line))
    }

    return true;
}

/// Draws the score at the given X coordinate.
/// \param[in]  score   The score to draw.
/// \param[in]  x       The X coordinate to draw at.
/// \returns    True if successful, false otherwise.
static bool draw_score(unsigned score, int x)
{
    char buffer[3];
    snprintf(buffer, 3, "%2u", score);

    for (int i = 0; i < 2; ++i)
    {
        char ch = buffer[i];
        if (ch >= '0' && ch <= '9')
        {
            DrawFunc func = draw_funcs[ch - '0'];
            if (!func(x))
                return false;
        }
        x += SCORE_THICKNESS * 5;
    }

    return true;
}

/// Draws the digit 0.
/// \param[in]  x   The X coordinate to draw at.
/// \returns    True if successful, false otherwise.
static bool draw_0(int x)
{
    SDL_Rect rects[] =
    {
        {x, 0, SCORE_THICKNESS * 4, SCORE_THICKNESS},
        {x + SCORE_THICKNESS * 3, SCORE_THICKNESS, SCORE_THICKNESS, SCORE_THICKNESS * 5},
        {x, SCORE_THICKNESS, SCORE_THICKNESS, SCORE_THICKNESS * 5},
        {x, SCORE_THICKNESS * 6, SCORE_THICKNESS * 4, SCORE_THICKNESS}
    };
    int count = sizeof(rects) / sizeof(rects[0]);
    CHECK_RESULT(SDL_RenderFillRects(renderer, rects, count))
    return true;
}

/// Draws the digit 1.
/// \param[in]  x   The X coordinate to draw at.
/// \returns    True if successful, false otherwise.
static bool draw_1(int x)
{
    SDL_Rect rects[] =
    {
        {x + SCORE_THICKNESS * 3, 0, SCORE_THICKNESS, SCORE_THICKNESS * 7}
    };
    int count = sizeof(rects) / sizeof(rects[0]);
    CHECK_RESULT(SDL_RenderFillRects(renderer, rects, count))
    return true;
}

/// Draws the digit 2.
/// \param[in]  x   The X coordinate to draw at.
/// \returns    True if successful, false otherwise.
static bool draw_2(int x)
{
    SDL_Rect rects[] =
    {
        {x, 0, SCORE_THICKNESS * 4, SCORE_THICKNESS},
        {x + SCORE_THICKNESS * 3, SCORE_THICKNESS, SCORE_THICKNESS, SCORE_THICKNESS * 2},
        {x, SCORE_THICKNESS * 3, SCORE_THICKNESS * 4, SCORE_THICKNESS},
        {x, SCORE_THICKNESS * 4, SCORE_THICKNESS, SCORE_THICKNESS * 2},
        {x, SCORE_THICKNESS * 6, SCORE_THICKNESS * 4, SCORE_THICKNESS}
    };
    int count = sizeof(rects) / sizeof(rects[0]);
    CHECK_RESULT(SDL_RenderFillRects(renderer, rects, count))
    return true;
}

/// Draws the digit 3.
/// \param[in]  x   The X coordinate to draw at.
/// \returns    True if successful, false otherwise.
static bool draw_3(int x)
{
    SDL_Rect rects[] =
    {
        {x, 0, SCORE_THICKNESS * 4, SCORE_THICKNESS},
        {x + SCORE_THICKNESS * 3, SCORE_THICKNESS, SCORE_THICKNESS, SCORE_THICKNESS * 5},
        {x + SCORE_THICKNESS, SCORE_THICKNESS * 3, SCORE_THICKNESS * 2, SCORE_THICKNESS},
        {x, SCORE_THICKNESS * 6, SCORE_THICKNESS * 4, SCORE_THICKNESS}
    };
    int count = sizeof(rects) / sizeof(rects[0]);
    CHECK_RESULT(SDL_RenderFillRects(renderer, rects, count))
    return true;
}

/// Draws the digit 4.
/// \param[in]  x   The X coordinate to draw at.
/// \returns    True if successful, false otherwise.
static bool draw_4(int x)
{
    SDL_Rect rects[] =
    {
        {x, 0, SCORE_THICKNESS, SCORE_THICKNESS * 4},
        {x + SCORE_THICKNESS, SCORE_THICKNESS * 3, SCORE_THICKNESS * 2, SCORE_THICKNESS},
        {x + SCORE_THICKNESS * 3, 0, SCORE_THICKNESS, SCORE_THICKNESS * 7}
    };
    int count = sizeof(rects) / sizeof(rects[0]);
    CHECK_RESULT(SDL_RenderFillRects(renderer, rects, count))
    return true;
}

/// Draws the digit 5.
/// \param[in]  x   The X coordinate to draw at.
/// \returns    True if successful, false otherwise.
static bool draw_5(int x)
{
    SDL_Rect rects[] =
    {
        {x, 0, SCORE_THICKNESS * 4, SCORE_THICKNESS},
        {x, SCORE_THICKNESS, SCORE_THICKNESS, SCORE_THICKNESS * 2},
        {x, SCORE_THICKNESS * 3, SCORE_THICKNESS * 4, SCORE_THICKNESS},
        {x + SCORE_THICKNESS * 3, SCORE_THICKNESS * 4, SCORE_THICKNESS, SCORE_THICKNESS * 2},
        {x, SCORE_THICKNESS * 6, SCORE_THICKNESS * 4, SCORE_THICKNESS}
    };
    int count = sizeof(rects) / sizeof(rects[0]);
    CHECK_RESULT(SDL_RenderFillRects(renderer, rects, count))
    return true;
}

/// Draws the digit 6.
/// \param[in]  x   The X coordinate to draw at.
/// \returns    True if successful, false otherwise.
static bool draw_6(int x)
{
    SDL_Rect rects[] =
    {
        {x, 0, SCORE_THICKNESS, SCORE_THICKNESS * 7},
        {x + SCORE_THICKNESS, 0, SCORE_THICKNESS * 3, SCORE_THICKNESS},
        {x + SCORE_THICKNESS, SCORE_THICKNESS * 3, SCORE_THICKNESS * 3, SCORE_THICKNESS},
        {x + SCORE_THICKNESS, SCORE_THICKNESS * 6, SCORE_THICKNESS * 3, SCORE_THICKNESS},
        {x + SCORE_THICKNESS * 3, SCORE_THICKNESS * 4, SCORE_THICKNESS, SCORE_THICKNESS * 2}
    };
    int count = sizeof(rects) / sizeof(rects[0]);
    CHECK_RESULT(SDL_RenderFillRects(renderer, rects, count))
    return true;
}

/// Draws the digit 7.
/// \param[in]  x   The X coordinate to draw at.
/// \returns    True if successful, false otherwise.
static bool draw_7(int x)
{
    SDL_Rect rects[] =
    {
        {x, 0, SCORE_THICKNESS * 4, SCORE_THICKNESS},
        {x + SCORE_THICKNESS * 3, SCORE_THICKNESS, SCORE_THICKNESS, SCORE_THICKNESS * 6}
    };
    int count = sizeof(rects) / sizeof(rects[0]);
    CHECK_RESULT(SDL_RenderFillRects(renderer, rects, count))
    return true;
}

/// Draws the digit 8.
/// \param[in]  x   The X coordinate to draw at.
/// \returns    True if successful, false otherwise.
static bool draw_8(int x)
{
    SDL_Rect rects[] =
    {
        {x, 0, SCORE_THICKNESS, SCORE_THICKNESS * 7},
        {x + SCORE_THICKNESS, 0, SCORE_THICKNESS * 2, SCORE_THICKNESS},
        {x + SCORE_THICKNESS, SCORE_THICKNESS * 3, SCORE_THICKNESS * 2, SCORE_THICKNESS},
        {x + SCORE_THICKNESS, SCORE_THICKNESS * 6, SCORE_THICKNESS * 2, SCORE_THICKNESS},
        {x + SCORE_THICKNESS * 3, 0, SCORE_THICKNESS, SCORE_THICKNESS * 7}
    };
    int count = sizeof(rects) / sizeof(rects[0]);
    CHECK_RESULT(SDL_RenderFillRects(renderer, rects, count))
    return true;
}

/// Draws the digit 9.
/// \param[in]  x   The X coordinate to draw at.
/// \returns    True if successful, false otherwise.
static bool draw_9(int x)
{
    SDL_Rect rects[] =
    {
        {x, 0, SCORE_THICKNESS, SCORE_THICKNESS * 4},
        {x + SCORE_THICKNESS, 0, SCORE_THICKNESS * 2, SCORE_THICKNESS},
        {x + SCORE_THICKNESS, SCORE_THICKNESS * 3, SCORE_THICKNESS * 2, SCORE_THICKNESS},
        {x + SCORE_THICKNESS * 3, 0, SCORE_THICKNESS, SCORE_THICKNESS * 7}
    };
    int count = sizeof(rects) / sizeof(rects[0]);
    CHECK_RESULT(SDL_RenderFillRects(renderer, rects, count))
    return true;
}
