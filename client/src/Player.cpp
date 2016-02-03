#include "Player.hpp"

#include "SDL2/SDL.h"
#include "Engine.hpp"
#include "core/Vector2d.hpp"

void Player::update(float dT) {
    Circle::update();
    // extend to some more physics stuff?
    Vector2d l_movementVector(0.0f, 0.0f);
    if(Engine::input->isKeyDown(SDLK_a))
        l_movementVector.x -= dT * m_speed;
    if(Engine::input->isKeyDown(SDLK_d))
        l_movementVector.x += dT * m_speed;
    if(Engine::input->isKeyDown(SDLK_w))
        l_movementVector.y += dT * m_speed;
    if(Engine::input->isKeyDown(SDLK_s))
        l_movementVector.y -= dT * m_speed;
    m_x += l_movementVector.x;
    m_y += l_movementVector.y;

    if(Engine::input->isKeyDown(SDLK_q))
        scale(-1.0f);
    if(Engine::input->isKeyDown(SDLK_e))
        scale(1.0f);

    l_movementVector.normalize();

    //InetConnection->send(player.x);
}
