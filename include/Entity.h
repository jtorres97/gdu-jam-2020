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

    void Damage(int damage);
    bool IsAlive();

protected:
    Entity();

    int m_maxHP = 0;
    int m_HP = 0;

    bool m_isActive = true;
    Point m_position;
    Size m_bound;
    Vector2D m_velocity;
};