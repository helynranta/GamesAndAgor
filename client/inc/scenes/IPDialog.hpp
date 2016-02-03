#ifndef _IPDIALOG_HPP
#define _IPDIALOG_HPP

#include <iostream>
#include <cerrno>

#include "Engine.hpp"
#include "core/Scene.hpp"

class IPDialog : public Scene {
private:

public:
    inline IPDialog () {}

    inline virtual ~IPDialog () {

    }

    inline void awake() override {
        gui->addText("hint", new GUIText(Engine::window->getRenderer(), Engine::R->getFont("res/fonts/OpenSans.ttf")));
        gui->addInput("input", new GUIInput(Engine::window->getRenderer(), Engine::R->getFont("res/fonts/OpenSans.ttf")));
        gui->getText("hint")->setText("Enter server IP address")->setX(Engine::camera->getWidth()/2.0f)->setY(Engine::camera->getHeight()/2.0f-30);
        gui->getText("hint")->setAlign(TEXT_ALIGN::CENTER_XY);
        gui->getInput("input")->setMaxLength(15);
        gui->getInput("input")->setX(Engine::camera->getWidth()/2.0f-200)->setY(Engine::camera->getHeight()/2.0f+30);
    }

    inline void update(float dt) override {
        if(Engine::input->isKeyPressed(SDLK_d)) {
            Engine::connection->disconnect();
            gui->getText("hint")->setText("Enter server IP address");
            gui->getInput("input")->show();
        }
        else if(Engine::input->isKeyPressed(SDLK_RETURN) &&
            Engine::connection->getState() != ConnectionState::CONNECTING
        ) {
            // show right text
            gui->getText("hint")->setText("Trying to connect to server... press d to return");
            gui->getInput("input")->hide();
            Engine::connection->sendUDP(GAME_MESSAGE_TYPE::JOIN, "");
        } else if(Engine::connection->getState() == ConnectionState::DISCONNECTED)
            gui->getInput("input")->show();
        if(Engine::connection->getState() == ConnectionState::CONNECTED)
          Engine::startScene("NickDialog");
        else if(Engine::connection->getState() == ConnectionState::TIMED_OUT) {
            gui->getText("hint")->setText("Connection timed out...");
            gui->getInput("input")->show();
        }
        /*
        switch(Engine::connection->getAck(GAME_MESSAGE_TYPE::NICK)) {
            case -1: break; // not acked yet
            case 0: // negative
                gui->getText("hint")->setText("Username already in use, use other one");
                gui->getInput("nick")->show();
                break;
            case 1: // positive
                Engine::startScene("Game");
            default: break;
        }
        */
    }

    inline void end() override {}

    inline void draw() override {}
};

 #endif
