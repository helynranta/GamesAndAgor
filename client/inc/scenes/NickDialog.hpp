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
        gui->addText("hint", new GUIText());
        gui->addInput("nick", new GUIInput());
        gui->getText("hint")->setText("Enter username:")->setX(Engine::camera->getWidth()/2.0f)->setY(Engine::camera->getHeight()/2.0f-30);
        gui->getText("hint")->setAlign(TEXT_ALIGN::CENTER_XY);
        gui->getInput("nick")->setMaxLength(15);
        gui->getInput("nick")->setX(Engine::camera->getWidth()/2.0f-200)->setY(Engine::camera->getHeight()/2.0f+30);
    }
    inline void update(float dt) {
        if(Engine::connection->getState() == ConnectionState::DISCONNECTED) {
            gui->getText("hint")->setText("FUCKED UP, RETURNING TO IP DIALOG");
            gui->getInput("nick")->hide();
            Engine::setTimeout(3000, [&]() {
                Engine::startScene("IPDialog");
            });
        }
        if(Engine::input->isKeyPressed(SDLK_RETURN) && (gui->getInput("nick")->getText()).size()) {
            gui->getText("hint")->setText("Checking username availability");
            gui->getInput("nick")->hide();
            // put here if connect returns true
            uint8_t nickbuffer[BUFFER_SIZE];
            Nick* nick = new Nick(Engine::connection->createHeader(), "Oskari");
            int messageLength = nick->PackSelf(nickbuffer);
            Engine::connection->send(nickbuffer, messageLength);
        }
        
        MessagesAck* ack = Engine::connection->getAck(GAME_MESSAGE_TYPE::NICK);
        if(ack != nullptr) {
            cout << "lol" << endl;
            switch(static_cast<NickAck*>(ack)->getStatus()) {
                case 0: // negative
                    gui->getText("hint")->setText("This nick is already in use, choose another one");
                    gui->getInput("nick")->show();
                    break;
                case 1: // positive
                    Engine::startScene("Game");
                    break;
                default: break;
            }
        }
    }
    inline void end() {}
    inline void draw() {}
};

#endif
