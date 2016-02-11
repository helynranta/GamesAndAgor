#include "Player.hpp"

#include "SDL2/SDL.h"
#include "Engine.hpp"
#include "core/Vector2d.hpp"

void Player::update(float dT) {
    Circle::update();
    Vector2d l_dir(0.0f, 0.0f);
    if(takeInput) {
        // extend to some more physics stuff?
        if(Engine::input->isKeyDown(SDLK_a))
            l_dir.x -= dT * m_speed;
        if(Engine::input->isKeyDown(SDLK_d))
            l_dir.x += dT * m_speed;
        if(Engine::input->isKeyDown(SDLK_w))
            l_dir.y += dT * m_speed;
        if(Engine::input->isKeyDown(SDLK_s))
            l_dir.y -= dT * m_speed;
        m_x += l_dir.x;
        m_y += l_dir.y;

        /* FOR DEBUG PURPOSES ONLY
        if(Engine::input->isKeyDown(SDLK_q))
            scale(-1.0f);
        if(Engine::input->isKeyDown(SDLK_e))
            scale(1.0f);
        */
        l_dir.normalize();
    }
    // do this after PackSelf is implemented
    //static uint8_t updateBuffer[BUFFER_SIZE];
    //GamePlayer* playermsg = new GamePlayer(Engine::connection->getID(), m_x, m_y, l_dir.x, l_dir.y, m_r*2);
    //int messageLength = playermsg->PackSelf(updateBuffer);
    //Engine::connection->send(playermsg, messageLength);
    //delete player;
}
