#include "Vector.h"

#include <cmath>

// Return the angle of a vector [0,0] to p
static float VectorAngle(Point p);

// Return the magnitude of vector from [0,0] to p
static float VectorMagnitude(Point p);

Vector2D::Vector2D() : m_magnitude(0.0), m_angle(0.0) {}

Vector2D::Vector2D(Vector2D &v)
{
    m_magnitude = v.m_magnitude;
    m_angle = v.m_angle;
}

Vector2D::Vector2D(Point point)
    : m_magnitude(VectorMagnitude(point)),
      m_angle(VectorAngle(point)) {}

Vector2D::Vector2D(float magnitude, float angle)
    : m_magnitude(magnitude),
      m_angle(angle) {}

Point Vector2D::GetPoint()
{
    return {m_magnitude * std::cos(m_angle),
            m_magnitude * std::sin(m_angle)};
}

float Vector2D::GetMagnitude()
{
    return m_magnitude;
}

float Vector2D::GetAngle()
{
    return m_angle;
}

bool Vector2D::NonZero()
{
    return std::abs(m_magnitude) > 0.0001;
}

Vector2D Vector2D::Scale(float scaleFactor)
{
    return Vector2D(m_magnitude * scaleFactor, m_angle);
}

Vector2D Vector2D::Normalize()
{
    return Vector2D(1.0, m_angle);
}

Vector2D Vector2D::Add(Vector2D v)
{
    return Vector2D(GetPoint() + v.GetPoint());
}

Vector2D Vector2D::Subtract(Vector2D v)
{
    return Vector2D(GetPoint() - v.GetPoint());
}

Vector2D Vector2D::Rotate(float angle)
{
    return Vector2D(m_magnitude, m_angle + angle);
}

static float VectorAngle(Point p)
{
    float angle = 0.0;
    if (p.x >= 0.0 && p.y >= 0.0)
    {
        angle = std::atan(p.y / p.x);
    }
    else if (p.x >= 0.0 && p.y < 0.0)
    {
        angle = std::atan(p.y / p.x) + (360.0 * TO_RADS);
    }
    else
    {
        angle = std::atan(p.y / p.x) + (180.0 * TO_RADS);
    }

    if (angle > Pi)
    {
        angle -= (2 * Pi);
    }

    return angle;
}

static float VectorMagnitude(Point p)
{
    return std::sqrt(std::pow(p.x, 2) + std::pow(p.y, 2));
}

Vector2D WASDToMovementVector(bool U, bool D, bool L, bool R)
{
    float magnitude = 1.0;
    float angle = 0.0;

    if ((R && L) || U && D)
    {
        magnitude = 0.0;
    }
    else if (R && U)
    {
        angle = 45.0 * TO_RADS;
    }
    else if (L && U)
    {
        angle = 135.0 * TO_RADS;
    }
    else if (L && D)
    {
        angle = 225.0 * TO_RADS;
    }
    else if (R && D)
    {
        angle = 315.0 * TO_RADS;
    }
    else if (R)
    {
        angle = 0.0 * TO_RADS;
    }
    else if (U)
    {
        angle = 90.0 * TO_RADS;
    }
    else if (L)
    {
        angle = 180.0 * TO_RADS;
    }
    else if (D)
    {
        angle = 270.0 * TO_RADS;
    }
    else
    {
        magnitude = 0.0;
        angle = 0.0;
    }

    return Vector2D(magnitude, angle);
}