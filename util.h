#ifndef UTIL_H
#define UTIL_H

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
/// \brief Common utility functions.

/// Displays an error message in a dialog box with the given title.
/// \param[in]  message The error message.
/// \param[in]  title   The dialog box's title.
void u_display_error(const char *message, const char *title);

/// Displays the current SDL error in a dialog box.
void u_display_sdl_error(void);

#endif
