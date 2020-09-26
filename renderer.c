#include "renderer.h"
#include "constants.h"
#include "util.h"
#include <SDL.h>

#define CHECK_RESULT(expr) if (expr) {\
    u_display_sdl_error();\
    return false;\
}

static SDL_Renderer *renderer;

bool r_init(void)
{
    SDL_Window *window = SDL_CreateWindow(
        "Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
    if (!window)
    {
        u_display_sdl_error();
        return false;
    }
    
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_TARGETTEXTURE
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

bool r_draw_frame(void)
{
    CHECK_RESULT(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255))
    CHECK_RESULT(SDL_RenderClear(renderer))

    CHECK_RESULT(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255))
    CHECK_RESULT(SDL_RenderDrawRect(renderer, NULL))
    SDL_Rect paddle;
    paddle.x = 10;
    paddle.y = 10;
    paddle.w = PADDLE_WIDTH;
    paddle.h = PADDLE_HEIGHT;
    CHECK_RESULT(SDL_RenderFillRect(renderer, &paddle))

    SDL_Rect ball =
    {
        64, 64, BALL_SIZE, BALL_SIZE
    };
    CHECK_RESULT(SDL_RenderFillRect(renderer, &ball))

    SDL_RenderPresent(renderer);

    return true;
}
