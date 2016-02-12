#ifndef _IPDIALOG_HPP
#define _IPDIALOG_HPP

#include <iostream>
#include <vector>
#include <string>
#include <cerrno>

#include "Engine.hpp"
#include "core/Scene.hpp"

class IPDialog : public Scene {
private:
    bool connecting = false;
    int connectStart = 0;
public:
    inline IPDialog () {}

    inline virtual ~IPDialog () { }

    inline void awake() override {
        gui->addText("hint", new GUIText());
        gui->addInput("input", new GUIInput());
        gui->getText("hint")->setText("Enter server IP address")->setX(Engine::camera->getWidth()/2.0f)->setY(Engine::camera->getHeight()/2.0f-30);
        gui->getText("hint")->setAlign(TEXT_ALIGN::CENTER_XY);
        gui->getInput("input")->setMaxLength(15);
        gui->getInput("input")->setX(Engine::camera->getWidth()/2.0f-200)->setY(Engine::camera->getHeight()/2.0f+30);
    }

    inline void update(float dt) override {
        ConnectionState cstate = Engine::connection->getState();
        int tcpstatus = Engine::connection->getTCPStatus();
        if(Engine::input->isKeyPressed(SDLK_d) && connecting) {
            Engine::input->getchar();
            Engine::input->getInput();
            Engine::connection->disconnect();
            gui->getText("hint")->setText("Enter server IP address");
            gui->getInput("input")->show();
            connecting = false;
        }
        else if(Engine::input->isKeyPressed(SDLK_RETURN) && !connecting) {
            // show right text
            connecting = true;
            gui->getText("hint")->setText("Trying to connect to server...");
            gui->getInput("input")->hide();

            string newIP = gui->getInput("input")->getText();
            if(newIP == "" || newIP == "localhost") newIP = "157.24.108.48";
            Engine::connection->setIP(newIP);

            Engine::connection->connectUDP();
            connectStart = int(SDL_GetTicks());
            //tmp
            uint8_t testBuffer[BUFFER_SIZE];
            MessageHeader dummyGameMessageHeader;
            dummyGameMessageHeader.user_id = 0;
            dummyGameMessageHeader.gameTime = 12;
            Join * joinMessage = new Join(dummyGameMessageHeader);
            int messageLenght = joinMessage->PackSelf(testBuffer);
            Engine::connection->send(testBuffer, messageLenght);
        }
        else if(cstate == ConnectionState::TIMED_OUT) {
            gui->getText("hint")->setText("Connection timed out...");
            gui->getInput("input")->show();
            connecting = false;
        }
        else if(cstate == ConnectionState::CONNECTED && tcpstatus == 1) {
            cout << "both tcp and udp status ok" << endl;
            // revert back
            gui->getText("hint")->setText("Enter server IP address");
            gui->getInput("input")->show();
            Engine::startScene("NickDialog");
        } else if (connecting) {
            gui->getText("hint")->setText("Trying to connect to server: "+to_string(uint(SDL_GetTicks()-connectStart)/1000));
        }
        checkAck();
    }
    inline void checkAck() {
        vector<Message*> msgs = Engine::connection->getMessagesOfType(MESSAGE_TYPE::ACK, GAME_MESSAGE_TYPE::JOIN);
        if(msgs.size() > 0 ){
            JoinAck* ack = static_cast<JoinAck*>(msgs.back());
            if(ack != nullptr) {
                switch(ack->getStatus()) {
                    case 0: // negative
                        gui->getText("hint")->setText("connection refused");
                        gui->getInput("input")->show();
                        connecting = false;
                        break;
                    case 1: // positive
                        gui->getText("hint")->setText("UDP penetrated, test TCP");
                        gui->getInput("input")->hide();
                        Engine::connection->setState(ConnectionState::CONNECTED);
                        Engine::connection->connectTCP();
                        Engine::connection->setID(static_cast<JoinAck*>(ack)->getUserID());
                        connecting = true;
                        break;
                }
            }
        }
    }
    inline void end() override {}

    inline void draw() override {}
};

 #endif
