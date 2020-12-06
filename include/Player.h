#pragma once

#include "Entity.h"
#include "Timer.h"
#include "Animation.h"

class Player : public Entity
{
public:
    Player();

    void Update(GameState &state) override;
    void Render(SDLRenderer &renderer) override;

    void SetRightAnimation(std::shared_ptr<Animation> animation);
    void SetLeftAnimation(std::shared_ptr<Animation> animation);
    void SetUpAnimation(std::shared_ptr<Animation> animation);
    void SetDownAnimation(std::shared_ptr<Animation> animation);

    float GetRotation();

protected:
    float m_rotation = 0.0f;
    std::shared_ptr<Animation> m_rightAnimation;
    std::shared_ptr<Animation> m_leftAnimation;
    std::shared_ptr<Animation> m_upAnimation;
    std::shared_ptr<Animation> m_downAnimation;
};