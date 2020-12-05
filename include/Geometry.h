#pragma once

#include "MathHelpers.h"

#include <SDL2/SDL.h>

struct Point
{
    double x, y;

    Point operator+(const Point &p)
    {
        return {x + p.x, y + p.y};
    }

    Point operator-(const Point &p)
    {
        return {x - p.x, y - p.y};
    }

    void operator+=(const Point &p)
    {
        x += p.x;
        y += p.y;
    }

    void operator=(const Point &p)
    {
        x = p.x;
        y = p.y;
    }
};

struct Size
{
    float w = 0.0;
    float h = 0.0;

    void operator=(const Size &s)
    {
        w = s.w;
        h = s.h;
    }
};

struct Rectangle
{
    Point pos;
    Size size;

    Point Center()
    {
        return {pos.x + size.w / 2, pos.y + size.h / 2};
    }

    void operator=(const Rectangle &R)
    {
        pos = R.pos;
        size = R.size;
    }
};

Point MapToUnitCircle(Point p);
bool PointInRect(Point p, Rectangle r);
bool RectangleCollision(Rectangle a, Rectangle b);
