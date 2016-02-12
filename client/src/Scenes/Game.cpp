#include "scenes/Game.hpp"

#include "Engine.hpp"

#include <stdlib.h> // rand()
#include <time.h> // time

void Game::awake(void) {
    // populate static object list
    for(int i = 0; i < 100; i++) m_statics.insert({i, new Circle("")});

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
    gui->addText("player-pos-server", new GUIText());
    gui->getText("player-pos-server")->setAlign(TEXT_ALIGN::CENTER_XY)->setPos(400, 50);
    // this is test
    Engine::camera->setPos(1000, 1000);
}
void Game::update(float dt) {
    // this is how camera behaves in real gameplay
    //Engine::camera->setPos(m_player->getX(), m_player->getY());
    //Engine::camera->setScale(float(m_player->getR())/100);
    m_player->update(dt);
    updateChat();
    handleMessages();
    gui->getText("ping")->setText("Ping: "+to_string(Engine::connection->getPing()));
    gui->getText("player-pos")->setText("("+std::to_string(m_player->getX())+","+std::to_string(m_player->getY())+")");
    gui->getText("player-pos-server")->setText("("+std::to_string(m_player->getSX())+","+std::to_string(m_player->getSY())+")");

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
    // handle game update messages
    static vector<Message*> update;
    update.clear();
    update = Engine::connection->getMessagesOfType(MESSAGE_TYPE::GAME_MESSAGE, GAME_MESSAGE_TYPE::GAME_UPDATE);
    if(update.size()>0) {
        GameUpdate* u = reinterpret_cast<GameUpdate*>(update.front());
        if(u == nullptr) cerr << "update cast failed" << endl;
        m_player->setSPos(u->getPosX(), u->getPosY(), SDL_GetTicks());
        m_player->setDir(u->getDirX(), u->getDirX());
        //cout <<"recieved "<< u->getPosX() << " " << u->getPosY() << endl;
        delete u;
        vector<GameObject*> objs = u->getGameObjects();
        //cout << objs.size() << endl;
        vector<GamePlayer*> players = u->getGamePlayers();
        cout << players.size() << endl;
    }
}
void Game::draw(void) {
    SDL_Rect l_ppos;
    // draw all "enemies"
    for(auto& circle : m_enemies) {
        Color c = circle->getColor();
        l_ppos = Engine::camera->transformToWorldCordinates(circle->getDestRect());
        SDL_SetTextureColorMod(Engine::R->getTexture("res/circle.png"), c.r, c.g, c.b);
        SDL_RenderCopy(Engine::window->getRenderer(), Engine::R->getTexture("res/circle.png"), NULL, &l_ppos);
        gui->getText(circle->getNick())->setPos(l_ppos.x+l_ppos.w/3.0f, l_ppos.y+l_ppos.h/2.4f)->show();
        gui->getText(circle->getNick())->setScale(0.5f/Engine::camera->getScale());
    }
    // draw player
    l_ppos = Engine::camera->transformToWorldCordinates(m_player->getDestRect());
    SDL_SetTextureColorMod(Engine::R->getTexture("res/circle.png"), 150, 150, 50);
    SDL_RenderCopy(Engine::window->getRenderer(), Engine::R->getTexture("res/circle.png"), NULL, &l_ppos );
    gui->getText(m_player->getNick())->setPos(l_ppos.x+l_ppos.w/3, l_ppos.y+l_ppos.h/2.4)->show();

}
void Game::end(void) {
    // free memory
    delete chat;
    delete m_player;
    for(auto& it : m_statics) delete it.second;
    for(auto& it : m_enemies) delete it;
    m_statics.clear();
    m_enemies.clear();
}
