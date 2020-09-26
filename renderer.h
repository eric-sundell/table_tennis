#ifndef RENDERER_H
#define RENDERER_H

#include "game.h"
#include <stdbool.h>

bool r_init(void);

bool r_draw_frame(const struct GameState *state);

#endif
