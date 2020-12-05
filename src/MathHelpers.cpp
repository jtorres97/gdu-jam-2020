#include "MathHelpers.h"

float MapRange(float x, float a, float b, float c, float d)
{
    if (b - a == 0.0)
    {
        return 0.0;
    }

    return x * ((d - c) / (b - a));
}

bool CloseEnough(float a, float b)
{
    return (a > (b - 0.00001) && a < (b + 0.00001));
}