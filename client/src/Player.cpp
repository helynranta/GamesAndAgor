#include "Player.hpp"

#include "SDL2/SDL.h"

#include "core/Vector2d.hpp"

void Player::update(float dT) {
    Circle::update();
    // extend to some more physics stuff?
    Vector2d l_movementVector(0.0f, 0.0f);
    if(m_inputManager->isKeyDown(SDLK_LEFT))
        l_movementVector.x -= dT * m_speed;
    if(m_inputManager->isKeyDown(SDLK_RIGHT))
        l_movementVector.x += dT * m_speed;
    if(m_inputManager->isKeyDown(SDLK_UP))
        l_movementVector.y += dT * m_speed;
    if(m_inputManager->isKeyDown(SDLK_DOWN))
        l_movementVector.y -= dT * m_speed;
    m_x += l_movementVector.x;
    m_y += l_movementVector.y;
    
    l_movementVector.normalize();
}
