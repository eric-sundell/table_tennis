#ifndef COORD_H
#define COORD_H

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
/// \brief Functions for working with fixed-point values.

/// Scale factor for fixed-point coordinates.
#define COORD_SCALE 16

/// Converts an integer to a fixed-point value.
/// \param[in]  x   The integer value.
/// \returns    The fixed-point value.
inline int coord_from_int(int x)
{
    return x * COORD_SCALE;
}

/// Converts a fixed-point value to an integer.
/// \param[in]  coord   The fixed-point coordinate.
/// \returns    The integer value.
inline int coord_to_int(int coord)
{
    return coord / COORD_SCALE;
}

/// Multiplies a fixed-point value by a fraction.
/// \param[in]  coord   The fixed-point value.
/// \param[in]  num     The fraction's numerator.
/// \param[in]  denom   The fraction's denominator.
/// \returns    The output fixed-point value.
inline int coord_mul_frac(int coord, int num, int denom)
{
    int prod = coord * num;
    return prod / denom;
}

#endif
