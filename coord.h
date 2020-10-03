#ifndef COORD_H
#define COORD_H

#define COORD_SCALE 16

inline int coord_from_int(int x)
{
    return x * COORD_SCALE;
}

inline int coord_to_int(int coord)
{
    return coord / COORD_SCALE;
}

inline int coord_mul_frac(int coord, int num, int denom)
{
    int prod = coord * num;
    return prod / denom;
}

#endif
