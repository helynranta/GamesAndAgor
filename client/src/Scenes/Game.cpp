#include "scenes/Game.hpp"

#include "Engine.hpp"

#include <stdlib.h> // rand()
#include <time.h> // time

void Game::awake(void) {
    // populate static object list
    m_player = new Player(Engine::getNick(), 50, 50, 50);
    //cout << Engine::getNick() << endl;
    gui->addText("ping", new GUIText());
    chat = new Chat(gui);

    srand(time(NULL));
    //m_enemies.emplace_back(Player("enemy", rand()&255,rand()%255,rand()%255));
    m_player->init();
    //m_enemies[0].init();
    //gui->addText(m_enemies[0].getNick(), new GUIText(Engine::window->getRenderer(), Engine::R->getFont("res/fonts/OpenSans.ttf")));
    gui->addText(m_player->getNick(), new GUIText());

    gui->addText("player-pos", new GUIText());
    gui->getText("player-pos")->setAlign(TEXT_ALIGN::CENTER_XY)->setPos(400, 20);


    gui->addText("amount-of-statics", new GUIText());
    gui->getText("amount-of-statics")->setPos(0, 20);
    // this is test
    Engine::camera->setPos(1000, 1000);
    gui->addText("main-game-hint", new GUIText());
    gui->getText("main-game-hint")->setAlign(TEXT_ALIGN::CENTER_XY)->setPos(400, 100);
    gui->getText("main-game-hint")->setText("");
}
void Game::update(float dt) {
    // if server suddenly is unresponsive, halt the whole game
    if(Engine::connection->getState() == ConnectionState::TIMING_OUT) {
        uint t = uint(12000 - (SDL_GetTicks() - Engine::connection->getLastServerUpdate()))/1000;
        gui->getText("main-game-hint")->setText("Game timing out in "+to_string(t));
        if(t > 12000) {
            Engine::connection->disconnect();
            Engine::startScene("IPDialog");
        }
        return;
    }
    // otherwise start by getting input from player
    m_player->update(dt);
    // then update everyone elses positions
    for(auto& it : drawables) {
        it->update();
    }
    // update all messages from server
    updateChat();
    handleMessages();
    // print ping and player position for player
    gui->getText("ping")->setText("Ping: "+to_string(Engine::connection->getPing()));
    gui->getText("player-pos")->setText("("+std::to_string(m_player->getX())+","+std::to_string(m_player->getY())+")");
    // this is how camera behaves in real gameplay (now in use)
    Engine::camera->setPos(m_player->getX(), m_player->getY());
    Engine::camera->setScale(float(m_player->getR())/100);

}
void Game::updateChat(void) {
    // stop input from player if chat is active
    m_player->setTakeInput(!chat->isActive());
    // fetch all chat messages from server
    vector<string> messages = Engine::connection->getChatMessages();
    if(messages.size() > 0) {
        for(auto& msg : messages) {
            chat->addLog(msg);
        }
        messages.clear();
    }
    // activate chat
    if(Engine::input->isKeyPressed(SDLK_t) && chat->isActive() == false) {
        // throw away all input before this
        Engine::input->getchar();
        Engine::input->getInput();
        // clear old text and activate
        gui->getInput("chat")->setText("");
        chat->Activate();
    }
    // send new chat message
    else if(Engine::input->isKeyPressed(SDLK_RETURN) && chat->isActive() == true && (gui->getInput("chat")->getText()).size()) {
        // throw away all input before this
        Engine::input->getInput();
        Engine::input->getchar();
        // send the text via tcp
        Engine::connection->sendTCP(gui->getInput("chat")->getText());
        // clear input
        gui->getInput("chat")->setText("");
    }
    // if input is empty and return is pressed, close chat window
    else if(Engine::input->isKeyPressed(SDLK_RETURN) && chat->isActive() == true && !(gui->getInput("chat")->getText()).size()) {
        // throw away all input before this
        Engine::input->getchar();
        Engine::input->getInput();
        // hide chat
        chat->deActivate();
        // clear input
        gui->getInput("chat")->setText("");
    }
}
void Game::handleMessages(void) {
    doGameUpdate();
    vector<Message*> msgs;
    // handle dying
    msgs = Engine::connection->getMessagesOfType(MESSAGE_TYPE::GAME_MESSAGE, GAME_MESSAGE_TYPE::PLAYER_DEAD);
    if(msgs.size()) {
        PlayerDead* death = dynamic_cast<PlayerDead*>(msgs.back());
        if(death != nullptr) {
            m_player->unInitialize();
            // send ack
            int id = Engine::connection->getID();
            uint8_t buffer[BUFFER_SIZE];
            PlayerDeadAck* ack = new PlayerDeadAck(
                Engine::connection->createDummyHeader(
                    id, SDL_GetTicks(), MESSAGE_TYPE::GAME_MESSAGE, 10
                ), id
            );
            int length = ack->PackSelf(buffer);
            Engine::connection->send(buffer, length);
            gui->getText("main-game-hint")->setText("You died!");
            Engine::setTimeout(2000, [this](){
                gui->getText("main-game-hint")->setText(" ");
            });
        } else cerr << "unable to cast död message" << endl;
    }
    msgs.clear();
    // handle game end
    msgs = Engine::connection->getMessagesOfType(MESSAGE_TYPE::GAME_MESSAGE, GAME_MESSAGE_TYPE::GAME_END);
    if(msgs.size()) {
        cout << "game is over" << endl;
        GameEnd* end = dynamic_cast<GameEnd*>(msgs.back());
        if(end != nullptr) {
            Points* p = end->getPoints();
            uint winner = 0;
            for(uint it = 0; it < (p->player_ids).size(); it++) {
                if(p->player_points[winner] < p->player_points[it]) winner = it;
            }
            gui->getText("main-game-hint")->setText("Game ended,"+p->player_nicks[winner]+" won");
            // just wait a sec and then jump right back to menu
            Engine::setTimeout(2000, [this]() {
                Engine::connection->disconnect();
                Engine::startScene("IPDialog");
            });
        } else cerr << "unable to cast end message" << endl;
    }
    msgs.clear();
    // handle new points messages
    msgs = Engine::connection->getMessagesOfType(MESSAGE_TYPE::GAME_MESSAGE, GAME_MESSAGE_TYPE::POINTS);
    if(msgs.size()) {
        cout << "got some points" << endl;
        // implement when ready
        Points* p = dynamic_cast<Points*>(msgs.back());
        if(p != nullptr) {
            for(uint it = 0; it < (p->player_ids).size(); it++) {
                auto mit = m_enemies.find(p->player_ids[it]);
                if(mit != m_enemies.end()) {
                    mit->second->setNick(p->player_nicks[it]);
                }
            }
        } else cerr << "unable to cast points message" << endl;

    }
    msgs.clear();
    // someone dropped out
    msgs = Engine::connection->getMessagesOfType(MESSAGE_TYPE::GAME_MESSAGE, GAME_MESSAGE_TYPE::PLAYER_OUT);
    if(msgs.size()) {
        for(auto& mis : msgs) {
            PlayerOut* out = dynamic_cast<PlayerOut*>(mis);
            if(out != nullptr) {
                auto mapit = m_enemies.find(out->getPlayerID());
                if(mapit != m_enemies.end())
                    chat->addLog(mapit->second->getNick()+" left the game");
                else
                    chat->addLog("Player id "+to_string(out->getPlayerID())+" left the game");
            } else cerr << "unable to cast out message" << endl;
        }
    }
}
void Game::doGameUpdate(void) {
    // handle game update messages
    GameUpdate* u = nullptr;
    static vector<Message*> update;
    update.clear();
    update = Engine::connection->getMessagesOfType(MESSAGE_TYPE::GAME_MESSAGE, GAME_MESSAGE_TYPE::GAME_UPDATE);
    // do latest
    if(update.size()) {
        u = reinterpret_cast<GameUpdate*>(update.back());
        if(u == nullptr) cerr << "update cast failed" << endl;
        m_player->setSPos(u->getPosX(), u->getPosY(), SDL_GetTicks());
        m_player->setDir(u->getDirX(), u->getDirX());
        m_player->setSR(u->getSize());
        vector<GameObject*> objs = u->getGameObjects();
        vector<GamePlayer*> players = u->getGamePlayers();
        drawables.clear();
        gui->getText("amount-of-statics")->setText("Amount of objs: "+to_string(objs.size()));

        if(objs.size()) {
            //cout << "Game.cpp - Number of static objects: " << objs.size() << endl;
            for(auto& oit : objs) {
                uint16_t id = oit->getObjectID();
                Circle* so = nullptr; // "static object", many innovative name much wow
                // try to find in map, if found then use that
                auto o = m_statics.find(id);
                if(o != m_statics.end()) so = o->second;
                // if didnt find then just create (this should never happen with static objects)
                if(so == nullptr) {
                    cerr << "Had to create new static object: " << id << " (id)" << endl;
                    cout << "pos" << oit->getLocX() << ","<<oit->getLocY() << endl;
                    so = new Circle(to_string(id));
                    so->setColor(80,80,80);
                    so->setSR(40);
                    so->setR(40);
                    //so->setR(40);
                    m_statics.insert({id, so});
                }
                // set pos and dir and place to drawable objects
                so->setPos(oit->getLocX(), oit->getLocY());
                drawables.push_back(so);
            }
        }
        if(players.size()) {
            // for each player in list
            for(auto& pit : players) {
                uint16_t id = pit->playerID;
                Circle* player = nullptr;
                // try to find in map, if found then use that
                auto e = m_enemies.find(id);
                if(e != m_enemies.end()) player = e->second;
                // if didnt find then just create
                if(player == nullptr) {
                    cout << "new player" << id << endl;
                    player = new Circle(to_string(id), false);
                    m_enemies.insert({id, player});
                }
                // set pos and dir and place to drawable objects
                player->setSPos(pit->pos_x, pit->pos_y, SDL_GetTicks());
                player->setDir(pit->dir_x, pit->dir_y);
                player->setSR(pit->size);
                //cout << int(pit->size) <<endl;
                drawables.push_back(player);
            }
        }
    }
    if(u != nullptr) delete u;
}
void Game::draw(void) {
    SDL_Rect l_ppos;
    // draw player
    for(auto& it : drawables) {
        if(it->isInitialized()) {
            Color l_c = it->getColor();
            l_ppos = Engine::camera->transformToWorldCordinates(it->getDestRect());
            SDL_SetTextureColorMod(Engine::R->getTexture("res/circle.png"), l_c.r, l_c.g, l_c.b);
            SDL_RenderCopy(Engine::window->getRenderer(), Engine::R->getTexture("res/circle.png"), NULL, &l_ppos);
            // if we got name of this player already
            if(it->getNick().size()) {
                // if gui object has not been created
                if(gui->getText("nick:"+it->getNick()) == nullptr) {
                    gui->addText("nick:"+it->getNick(), new GUIText());
                    gui->getText(it->getNick())->setText(it->getNick());
                    gui->getText(it->getNick())->setAlign(TEXT_ALIGN::CENTER_XY);
                }
                // put text to its rightfull place
                gui->getText("nick:"+it->getNick())->setPos(l_ppos.x+l_ppos.w/3, l_ppos.y+l_ppos.h/2.4)->show();
                gui->getText("nick:"+it->getNick())->setScale(0.5f/Engine::camera->getScale());
            }
        }
    }

    l_ppos = Engine::camera->transformToWorldCordinates(m_player->getDestRect());
    SDL_SetTextureColorMod(Engine::R->getTexture("res/circle.png"), 150, 150, 50);
    SDL_RenderCopy(Engine::window->getRenderer(), Engine::R->getTexture("res/circle.png"), NULL, &l_ppos );
    gui->getText(m_player->getNick())->setPos(l_ppos.x+l_ppos.w/3, l_ppos.y+l_ppos.h/2.4)->show();
    gui->getText(m_player->getNick())->setScale(0.5f/Engine::camera->getScale());
}
void Game::end(void) {
    // free memory
    delete chat;
    delete m_player;
    for(auto& it : m_statics) delete it.second;
    for(auto& it : m_enemies) delete it.second;
    m_statics.clear();
    m_enemies.clear();
}
