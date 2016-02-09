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
        ConnectionState cstate = Engine::connection->getState();
        int tcpstatus = Engine::connection->getTCPStatus();
        if(Engine::input->isKeyPressed(SDLK_d)) {
            Engine::connection->disconnect();
            gui->getText("hint")->setText("Enter server IP address");
            gui->getInput("input")->show();
        }
        else if(Engine::input->isKeyPressed(SDLK_RETURN) && cstate != ConnectionState::CONNECTING) {
            // show right text
            gui->getText("hint")->setText("Trying to connect to server... press d to return");
            gui->getInput("input")->hide();
            
            string newIP = gui->getInput("input")->getText();
            if(newIP == "" || newIP == "localhost") newIP = "127.0.0.1";
            Engine::connection->setIP(newIP);

            Engine::connection->connectUDP();
            //tmp
            uint8_t testBuffer[BUFFER_SIZE];
            MessageHeader dummyGameMessageHeader;
            dummyGameMessageHeader.user_id = 0;
            dummyGameMessageHeader.gameTime = 12;
            Join * joinMessage = new Join(dummyGameMessageHeader);
            int messageLenght = joinMessage->PackSelf(testBuffer);
            Engine::connection->send(testBuffer, messageLenght);
        }
        
        if(cstate == ConnectionState::TIMED_OUT) {
            gui->getText("hint")->setText("Connection timed out...");
            gui->getInput("input")->show();
        }

        MessagesAck* ack = Engine::connection->getAck(GAME_MESSAGE_TYPE::JOIN);
        if(ack != nullptr) {
            switch(static_cast<JoinAck*>(ack)->getStatus()) {
                case 0: // negative
                    gui->getText("hint")->setText("connection refused");
                    gui->getInput("input")->show();
                    break;
                case 1: // positive
                    gui->getText("hint")->setText("UDP penetrated, test TCP");
                    gui->getInput("input")->hide();
                    Engine::connection->setState(ConnectionState::CONNECTED);
                    Engine::connection->connectTCP();
                    Engine::connection->setID(static_cast<JoinAck*>(ack)->getUserID());
                    break;
            }
        }
        if(cstate == ConnectionState::CONNECTED && tcpstatus == 1) {
            cout << "both tcp and udp status ok" << endl;
            Engine::connection->sendTCP("FUCK LOL");
            Engine::startScene("NickDialog");
        }
    }

    inline void end() override {}

    inline void draw() override {}
};

 #endif
