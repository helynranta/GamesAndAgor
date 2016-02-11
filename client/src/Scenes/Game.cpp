#include "scenes/Game.hpp"

#include "Engine.hpp"

#include <stdlib.h> // rand()
#include <time.h> // time

void Game::awake(void) {
    chat = new Chat(gui);

    srand(time(NULL));
    //m_enemies.emplace_back(Player("enemy", rand()&255,rand()%255,rand()%255));
    m_player.init();
    //m_enemies[0].init();
    //gui->addText(m_enemies[0].getNick(), new GUIText(Engine::window->getRenderer(), Engine::R->getFont("res/fonts/OpenSans.ttf")));
    gui->addText(m_player.getNick(), new GUIText());
}
void Game::update(float dt) {
    // this is how camera behaves in real gameplay
    Engine::camera->setPos(m_player.getX(), m_player.getY());
    Engine::camera->setScale(float(m_player.getR())/100);
    m_player.update(dt);
    updateChat();
    handleMessages();
}
void Game::updateChat(void) {
    // stop input from player if chat is active
    m_player.setTakeInput(!chat->isActive());
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
        return;
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
        return;
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
        return;
    }
}
void Game::handleMessages(void) {
    static vector<Message*> update;
    update.clear();
    update = Engine::connection->getMessagesOfType(MESSAGE_TYPE::GAME_MESSAGE, GAME_MESSAGE_TYPE::GAME_UPDATE);
    if(update.size()>0) {
        GameUpdate* u = static_cast<GameUpdate*>(update.back());
        m_player.setPos(u->getPosX(), u->getPosY(), SDL_GetTicks());
        delete u;       
    }
}
void Game::draw(void) {
    SDL_Rect l_ppos;
    // draw all "enemies"
    for(auto& circle : m_enemies) {
        Color c = circle.getColor();
        l_ppos = Engine::camera->transformToWorldCordinates(circle.getDestRect());
        SDL_SetTextureColorMod(Engine::R->getTexture("res/circle.png"), c.r, c.g, c.b);
        SDL_RenderCopy(Engine::window->getRenderer(), Engine::R->getTexture("res/circle.png"), NULL, &l_ppos);
        gui->getText(circle.getNick())->setPos(l_ppos.x+l_ppos.w/3.0f, l_ppos.y+l_ppos.h/2.4f)->show();
        gui->getText(circle.getNick())->setScale(0.5f/Engine::camera->getScale());
    }
    // draw player
    l_ppos = Engine::camera->transformToWorldCordinates(m_player.getDestRect());
    SDL_SetTextureColorMod(Engine::R->getTexture("res/circle.png"), 255, 255, 255);
    SDL_RenderCopy(Engine::window->getRenderer(), Engine::R->getTexture("res/circle.png"), NULL, &l_ppos );
    gui->getText(m_player.getNick())->setPos(l_ppos.x+l_ppos.w/3, l_ppos.y+l_ppos.h/2.4)->show();

}
void Game::end(void) {
    delete chat;
}
