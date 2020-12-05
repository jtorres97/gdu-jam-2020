#pragma once

#include "Geometry.h"
#include "Renderer.h"
#include "GameState.h"
#include "Vector.h"

class Entity
{
public:
    virtual void Update(GameState &state);
    virtual void Render(SDLRenderer &renderer);

    Rectangle GetHitBox();
    bool IsActive();

    void Deactivate();

    Point GetPosition();

    Vector2D GetVelocity();
    void SetVelocity(Vector2D velocity);

protected:
    Entity();

    bool m_isActive = true;
    Point m_position;
    Size m_bound;
    Vector2D m_velocity;
};