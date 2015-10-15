#pragma once

#include <iostream>

#include "Engine.hpp"

class Menu : public Scene {
private:

public:
    inline Menu () {}

    inline virtual ~Menu () {

    }

    inline void awake() override {
        gui->addText("hint", new GUIText(Window::getRenderer(), R::getFont("res/fonts/OpenSans.ttf")));
        gui->addInput("input", new GUIInput(Window::getRenderer(), R::getFont("res/fonts/OpenSans.ttf")));
        gui->getText("hint")->setText("Enter server IP address")->setX(Camera::getWidth()/2.0f)->setY(Camera::getHeight()/2.0f-30);
        gui->getText("hint")->setAlign(TEXT_ALIGN::CENTER_XY);
        gui->getInput("input")->setMaxLength(15);
        gui->getInput("input")->setX(Camera::getWidth()/2.0f-200)->setY(Camera::getHeight()/2.0f+30);
    }

    inline void update(float dt) override {
        if(Input::isKeyPressed(SDLK_RETURN))
            Engine::startScene("Game");
    }

    inline void end() override {}

    inline void draw() override {}
};

/*
void Game::menu() {
    GUIInput* l_input = new GUIInput(m_Input, m_renderer, R->getFont("res/fonts/OpenSans.ttf")); // input field
    bool isConnected = false;   // for connection check
    while(m_gameState!= GameState::EXIT && (m_nickname.length() == 0 || !isConnected))
    {
        // clear screen
        SDL_RenderClear(m_renderer);
        // set clear color
        SDL_SetRenderDrawColor(m_renderer, 50, 50, 50, 255);
        // process events
        processInput();
        // test connection screen if nickname is given
        if (!isConnected) {
            l_input->setMaxLength(29);
            // create info text, centered to screen
            m_guiText->renderText(m_camera->getWidth()/2,m_camera->getHeight()/2-80, "Give host IP address:", TEXT_ALIGN::CENTER_XY);
            // wait for return to be pressed
            if(l_input->update(m_camera->getWidth()/2-200, m_camera->getHeight()/2))
            {
                // finished condition is that we have connected to server
                // TMP TMP
                SDL_RenderClear(m_renderer);
                m_guiText->renderText(m_camera->getWidth()/2,m_camera->getHeight()/2, "Connecting to server...", TEXT_ALIGN::CENTER_XY);
                SDL_RenderPresent(m_renderer);
                // TMTP TMP
                isConnected = true;
                l_input->empty();
            }
            l_input->draw();
        }
        // as long as we dont have proper nickname
        else if(m_nickname.length() == 0)
        {
            l_input->setMaxLength(8);
            // create info text, centered to screen
            m_guiText->renderText(m_camera->getWidth()/2,m_camera->getHeight()/2-80, "Give nickname: (3-8 characters)", TEXT_ALIGN::CENTER_XY);
            // wait for return to be pressed
            if(l_input->update(m_camera->getWidth()/2-200, m_camera->getHeight()/2))
            {
                if((l_input->getText()).length() > 2) {
                    m_nickname = l_input->getText();
                }
            }
            l_input->draw();
        }
        // render screen
        SDL_RenderPresent(m_renderer);
        // have a little break
    }
}
*/
