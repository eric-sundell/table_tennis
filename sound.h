#ifndef SOUND_H
#define SOUND_H

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
