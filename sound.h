#ifndef SOUND_H
#define SOUND_H

/// \file
/// \brief Functionality exported by the sound module.

#include <stdbool.h>

/// Initializes the sound system.
/// \returns True if initialization was successful, false otherwise.
bool s_init(void);

/// Plays a bounce sound effect.
void s_play_bounce(void);

/// Plays a "goal scored" sound effect.
void s_play_score(void);

/// Releases resources used by the sound system.
void s_quit(void);

#endif
