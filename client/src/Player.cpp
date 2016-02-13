#include "Player.hpp"

#include "SDL2/SDL.h"
#include "core/Vector2d.hpp"

void Player::update(float dT) {
    Circle::update();
    Vector2d l_dir(0.0f, 0.0f);
    if(takeInput) {
        if(Engine::input->isKeyDown(SDLK_a))
            l_dir.x -= dT * m_speed;
        if(Engine::input->isKeyDown(SDLK_d))
            l_dir.x += dT * m_speed;
        if(Engine::input->isKeyDown(SDLK_w))
            l_dir.y += dT * m_speed;
        if(Engine::input->isKeyDown(SDLK_s))
            l_dir.y -= dT * m_speed;

        m_sx += l_dir.x;
        m_sy += l_dir.y;

        /* FOR DEBUG PURPOSES ONLY
        if(Engine::input->isKeyDown(SDLK_q))
            scale(-1.0f);
        if(Engine::input->isKeyDown(SDLK_e))
            scale(1.0f);
        */
        l_dir.normalize();
    }
    // do this after PackSelf is implemented
    static uint lastSend = 0;
    if(lastSend + 20 < SDL_GetTicks() && initialized) {
        uint8_t updateBuffer[BUFFER_SIZE];
        Move* m = new Move(Engine::connection->createDummyHeader(
            Engine::connection->getID(), SDL_GetTicks(), MESSAGE_TYPE::PLAYER_MOVEMENT, 10
        ), 0, uint16_t(m_sx), uint16_t(m_sy), uint16_t(l_dir.x), uint16_t(l_dir.y));
        int messageLength = m->PackSelf(updateBuffer);
        Engine::connection->send(updateBuffer, messageLength);
        lastSend = SDL_GetTicks();
        delete m;
    }
}
