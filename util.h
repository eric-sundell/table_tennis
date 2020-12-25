#ifndef UTIL_H
#define UTIL_H

/// \file
/// \brief Common utility functions.

/// Displays an error message in a dialog box with the given title.
/// \param[in]  message The error message.
/// \param[in]  title   The dialog box's title.
void u_display_error(const char *message, const char *title);

/// Displays the current SDL error in a dialog box.
void u_display_sdl_error(void);

#endif
