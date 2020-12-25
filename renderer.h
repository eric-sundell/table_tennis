#ifndef RENDERER_H
#define RENDERER_H

/// \file
/// \brief Functionality exported by the renderer module.

#include "game.h"
#include <stdbool.h>

/// Initializes the renderer.
/// \returns True if initialization was successful, false otherwise.
bool r_init(void);

/// Draws the game state to the screen.
/// \param[in]  state   The state to render.
/// \returns True if drawing was successful, false otherwise.
bool r_draw_frame(const struct GameState *state);

#endif
