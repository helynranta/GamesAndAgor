#ifndef _NICKDIALOG_HPP
#define _NICKDIALOG_HPP

#include <iostream>

#include "Engine.hpp"
#include "core/Scene.hpp"
#include "Inet/Messages.hpp"

class NickDialog : public Scene {
private:
    uint8_t nickbuffer[BUFFER_SIZE];
    int messageLength;
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
        if(Engine::input->isKeyPressed(SDLK_RETURN) && (gui->getInput("nick")->getText()).size()) {
            gui->getText("hint")->setText("Checking username availability");
            gui->getInput("nick")->hide();
            // try to send nick to server via udp
            Nick* nick = new Nick(Engine::connection->createDummyHeader(Engine::connection->getID(), SDL_GetTicks(), MESSAGE_TYPE::GAME_MESSAGE, 0), gui->getInput("nick")->getText());
            messageLength = nick->PackSelf(nickbuffer);
            Engine::connection->send(nickbuffer, messageLength);
        }
        // check if nick has been acked
        vector<Message*> msgs = Engine::connection->getMessagesOfType(MESSAGE_TYPE::ACK, GAME_MESSAGE_TYPE::NICK);
        if(msgs.size()>0) {
            NickAck* ack = static_cast<NickAck*>(msgs.back());
            if(ack != nullptr) {
                switch(ack->getStatus()) {
                    case 0: // negative
                        gui->getText("hint")->setText("This nick is already in use, choose another one");
                        gui->getInput("nick")->show();
                        break;
                    case 1: // positive
                        Engine::setNick(gui->getInput("nick")->getText());
                        // revert inputs back
                        gui->getText("hint")->setText("Enter username:");
                        gui->getInput("nick")->setText("");
                        // ack the ack
                        messageLength = ack->PackSelf(nickbuffer);
                        Engine::connection->send(nickbuffer, messageLength);
                        // start game
                        Engine::startScene("Game");
                        break;
                    default:
                        gui->getText("hint")->setText("Server had unhandeld error");
                        gui->getInput("nick")->hide();
                        break;
                }
            }
        }
    }
    inline void end() {}
    inline void draw() {}
};

#endif
