#include "Player.hpp"

void Player::update(float dT) {
    Circle::update();
    if(m_inputManager->isKeyDown(SDLK_LEFT))
        m_x += -.5*dT;
    if(m_inputManager->isKeyDown(SDLK_RIGHT))
        m_x += .5*dT;
    if(m_inputManager->isKeyDown(SDLK_UP))
        m_y += .5*dT;
    if(m_inputManager->isKeyDown(SDLK_DOWN))
        m_y -= .5*dT;
}
