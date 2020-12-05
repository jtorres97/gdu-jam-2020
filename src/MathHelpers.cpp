#include "MathHelpers.h"

float MapRange(float x, float a, float b, float c, float d)
{
    if (b - a == 0.0)
    {
        return 0.0;
    }

    return x * ((d - c) / (b - a));
}
