#ifndef _MENU_HPP
#define _MENU_HPP

#include <iostream>

#include "Engine.hpp"
#include "core/Scene.hpp"

class Menu : public Scene {
private:

public:
    inline Menu () {}

    inline virtual ~Menu () {

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
        if(Engine::input->isKeyPressed(SDLK_RETURN))
            Engine::startScene("Game");
    }

    inline void end() override {}

    inline void draw() override {}
};

 #endif
