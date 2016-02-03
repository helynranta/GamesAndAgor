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
        else if(Engine::input->isKeyPressed(SDLK_RETURN) &&
            cstate != ConnectionState::CONNECTING
        ) {
            // show right text
            gui->getText("hint")->setText("Trying to connect to server... press d to return");
            gui->getInput("input")->hide();
            Engine::connection->setIP(gui->getInput("input")->getText());
            //Engine::connection->sendUDP(GAME_MESSAGE_TYPE::JOIN, "");
        } else if(cstate == ConnectionState::DISCONNECTED)
            gui->getInput("input")->show();
        if(cstate == ConnectionState::CONNECTED)
          Engine::startScene("NickDialog");
        else if(cstate == ConnectionState::TIMED_OUT) {
            gui->getText("hint")->setText("Connection timed out...");
            gui->getInput("input")->show();
        }
        /*
        switch(Engine::connection->getAck(GAME_MESSAGE_TYPE::JOIN)) {
            case -1: break; // not acked yet
            case 0: // negative
                gui->getText("hint")->setText("connection refused");
                gui->getInput("nick")->show();
                break;
            case 1: // positive
                gui->getText("hint")->setText("UDP penetrated, test TCP");
                Engine::connection->connectTCP();
                break;
            default: break;
        }
        */
        if(cstate == ConnectionState::CONNECTED && tcpstatus == 1) Engine::startScene("Game");
    }

    inline void end() override {}

    inline void draw() override {}
};

 #endif
