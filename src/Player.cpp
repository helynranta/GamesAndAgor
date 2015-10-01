#include "Player.hpp"

void Player::update() {
    Circle::update();
    if(m_inputManager->isKeyDown(SDLK_LEFT))
        m_x += -.01;
    if(m_inputManager->isKeyDown(SDLK_RIGHT))
        m_x += .01;
    if(m_inputManager->isKeyDown(SDLK_UP))
        m_y += .01;
    if(m_inputManager->isKeyDown(SDLK_DOWN))
        m_y -= .01;
}
