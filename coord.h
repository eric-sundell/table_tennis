#ifndef COORD_H
#define COORD_H

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
