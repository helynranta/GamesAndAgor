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
        if(Engine::input->isKeyPressed(SDLK_RETURN)) {
            // show right text
            gui->getText("hint")->setText("Trying to connect to server...");
            gui->getInput("input")->hide();
            Engine::connection->connect("127.0.0.1", "5200");
        } else if(Engine::connection->getState() == ConnectionState::DISCONNECTED)
            gui->getInput("input")->show();
        if(Engine::connection->getState() == ConnectionState::CONNECTED)
          Engine::startScene("NickDialog");
        else if(Engine::connection->getState() == ConnectionState::REFUSED) {
            gui->getText("hint")->setText("Connection refused...");
            gui->getInput("input")->show();
        }
    }

    inline void end() override {}

    inline void draw() override {}
};

 #endif
