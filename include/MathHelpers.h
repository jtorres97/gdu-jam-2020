#pragma once

static const float Pi = 3.141592;
static const float TO_RADS = Pi / 180.0;
static const float TO_DEGS = 180.0 / Pi;

// Maps the value x from the range [a,b] to a new value in range [c,d]
float MapRange(float x, float a, float b, float c, float d);

bool CloseEnough(float a, float b);
