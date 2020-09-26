#include "renderer.h"
#include "constants.h"
#include "util.h"
#include <SDL.h>
#include <stddef.h>

#define CHECK_RESULT(expr) if (expr) {\
    u_display_sdl_error();\
    return false;\
}

static const SDL_Rect table =
{
    0, TABLE_Y,
    TABLE_WIDTH, TABLE_HEIGHT
};

static bool draw_paddles(const struct PlayerState *players);

static bool draw_table(void);

static SDL_Renderer *renderer;

bool r_init(void)
{
    SDL_Window *window = SDL_CreateWindow(
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
    
    renderer = SDL_CreateRenderer(
        window,
        -1,
        0
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
        SDL_DestroyWindow(window);
        return false;
    }

    if (SDL_RenderSetIntegerScale(renderer, SDL_TRUE))
    {
        u_display_sdl_error();
        SDL_DestroyWindow(window);
        return false;
    }
    
    return true;
}

bool r_draw_frame(const struct GameState *state)
{
    CHECK_RESULT(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255))
    CHECK_RESULT(SDL_RenderClear(renderer))

    if (!draw_table())
        return false;

    CHECK_RESULT(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255))
    SDL_Rect ball =
    {
        state->ball.x,
        state->ball.y + TABLE_Y,
        BALL_SIZE, BALL_SIZE
    };
    CHECK_RESULT(SDL_RenderFillRect(renderer, &ball))

    if (!draw_paddles(state->players))
        return false;

    SDL_RenderPresent(renderer);

    return true;
}

static bool draw_paddles(const struct PlayerState *players)
{
    CHECK_RESULT(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255))
    SDL_Rect paddles[PLAYER_COUNT];
    for (size_t i = 0; i < PLAYER_COUNT; ++i)
    {
        paddles[i].x = player_x_coords[i];
        paddles[i].y = players->y + TABLE_Y;
        paddles[i].w = PADDLE_WIDTH;
        paddles[i].h = PADDLE_HEIGHT;
    }
    CHECK_RESULT(SDL_RenderFillRects(renderer, paddles, PLAYER_COUNT))

    return true;
}

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
