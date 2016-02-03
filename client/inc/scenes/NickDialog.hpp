#ifndef _NICKDIALOG_HPP
#define _NICKDIALOG_HPP

#include <iostream>

#include "Engine.hpp"
#include "core/Scene.hpp"

class NickDialog : public Scene {
private:
protected:
public:
    inline NickDialog() {;}
    inline virtual ~NickDialog() {}
    inline void awake() {
        gui->addText("hint", new GUIText(Engine::window->getRenderer(), Engine::R->getFont("res/fonts/OpenSans.ttf")));
        gui->addInput("nick", new GUIInput(Engine::window->getRenderer(), Engine::R->getFont("res/fonts/OpenSans.ttf")));
        gui->getText("hint")->setText("Enter username:")->setX(Engine::camera->getWidth()/2.0f)->setY(Engine::camera->getHeight()/2.0f-30);
        gui->getText("hint")->setAlign(TEXT_ALIGN::CENTER_XY);
        gui->getInput("nick")->setMaxLength(15);
        gui->getInput("nick")->setX(Engine::camera->getWidth()/2.0f-200)->setY(Engine::camera->getHeight()/2.0f+30);
    }
    inline void update(float dt) {
        if(Engine::input->isKeyPressed(SDLK_RETURN)) {
            gui->getText("hint")->setText("Checking username availability");
            gui->getInput("nick")->hide();
            Engine::setTimeout(2000, bind([&](GUIText* text) {
              if(text != nullptr)
                text->setText("Hold your horses, this seems to take a lot of time...");
            }, gui->getText("hint")));
            // put here if connect returns true
            // Engine::connection->sendNick(gui->getText("hint")->getText())
        }
    }
    inline void end() {}
    inline void draw() {}
};

#endif
