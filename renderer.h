#ifndef RENDERER_H
#define RENDERER_H

/// \file
/// \brief Functionality exported by the renderer module.

#include "game.h"
#include <stdbool.h>

/// Initializes the renderer.
/// \param[in]  use_vsync   Whether V-sync should be used.
/// \returns True if initialization was successful, false otherwise.
bool r_init(bool use_vsync);

/// Draws the game state to the screen.
/// \param[in]  state   The state to render.
/// \returns True if drawing was successful, false otherwise.
bool r_draw_frame(const struct GameState *state);

/// Releases resources used by the renderer.
void r_quit(void);

#endif
