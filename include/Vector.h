#pragma once

#include "Geometry.h"

class Vector2D
{
public:
    Vector2D();
    Vector2D(Vector2D &v);
    Vector2D(Point point);
    Vector2D(float magnitude, float angle);
    Point GetPoint();
    float GetMagnitude();
    float GetAngle();

    Vector2D Scale(float scaleFactor);
    Vector2D Normalize();
    Vector2D Add(Vector2D v);
    Vector2D Rotate(float angle);

private:
    float m_magnitude = 0.0;
    float m_angle = 0.0;
};

// Converts a combination of directions into a unit vector for movement at that angle
Vector2D WASDToMovementVector(bool U, bool D, bool L, bool R);
