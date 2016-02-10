#include "scenes/Game.hpp"

#include "Engine.hpp"

#include <stdlib.h> // rand()
#include <time.h> // time

void Game::awake() {
    chat = new Chat(gui);
    gui->addText("PAUSED", new GUIText());
    /*
        //gui->getText("PAUSED")->hide();
        gui->getText("PAUSED");
        gui->getText("PAUSED")->setX(Engine::camera->getWidth()/2)->setY(Engine::camera->getHeight()/2);
        gui->getText("PAUSED")->setAlign(TEXT_ALIGN::CENTER_XY);
        gui->getText("PAUSED")->setColor({200,10,10,255});
        // INIT DEBUG PRINTS
        gui->addText("d-topleft", new GUIText());

        gui->addText("d-topright", new GUIText());
        gui->getText("d-topright")->setX(Engine::camera->getWidth());
        gui->getText("d-topright")->setAlign(TEXT_ALIGN::RIGHT);

        gui->addText("d-bottomleft", new GUIText());
        gui->getText("d-bottomleft")->setY(Engine::camera->getHeight());
        gui->getText("d-bottomleft")->setAlign(TEXT_ALIGN::OVER);

        gui->addText("d-bottomright", new GUIText());
        gui->getText("d-bottomright")->setX(Engine::camera->getWidth())->setY(Engine::camera->getHeight());
        gui->getText("d-bottomright")->setAlign(TEXT_ALIGN::OVER_RIGHT);

        gui->addText("cam-pos", new GUIText());
        gui->getText("cam-pos")->setX(Engine::camera->getWidth()/2)->setAlign(TEXT_ALIGN::CENTER);

        gui->addText("cam-scale", new GUIText());
        gui->getText("cam-scale")->setX(Engine::camera->getWidth()/2)->setAlign(TEXT_ALIGN::CENTER)->setY(30);

        gui->addText("player-pos", new GUIText());
        gui->getText("player-pos")->setAlign(TEXT_ALIGN::CENTER_XY);
        gui->getText("player-pos")->setColor({50,50,50,255});
    */

    srand(time(NULL));
    m_enemies.emplace_back(Player("enemy", rand()&255,rand()%255,rand()%255));
    m_player.init();
    m_enemies[0].init();
    gui->addText(m_enemies[0].getNick(), new GUIText(Engine::window->getRenderer(), Engine::R->getFont("res/fonts/OpenSans.ttf")));
    gui->addText(m_player.getNick(), new GUIText(Engine::window->getRenderer(), Engine::R->getFont("res/fonts/OpenSans.ttf")));
/*    */
}
void Game::update(float dt) {
    // this is how camera behaves in real gameplay
    Engine::camera->setPos(m_player.getX(), m_player.getY());
    Engine::camera->setScale(float(m_player.getR())/100);
    m_player.update(dt);

    if(Engine::input->isKeyPressed(SDLK_t) && chat->isActive() == false) {
        chat->Activate();  
        Engine::input->getchar();
    } 
    else if(Engine::input->isKeyPressed(SDLK_RETURN) && chat->isActive() == true && (gui->getInput("chat")->getText()).size()) {
        Engine::input->getchar();
        Engine::connection->sendTCP(gui->getInput("chat")->getText());
        gui->getInput("chat")->setText("");
    } else if(Engine::input->isKeyPressed(SDLK_RETURN) && chat->isActive() == true && !(gui->getInput("chat")->getText()).size()) {
        chat->deActivate();  
        Engine::input->getchar();
    }
    m_player.setTakeInput(!chat->isActive());
}
void Game::draw() {
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

    if(Engine::gameState == GameState::PAUSE)
        gui->getText("PAUSED")->show();
    else
        gui->getText("PAUSED")->hide();
    /*
        // ALL DEBUG PRINTS!
        if(Engine::isDebugging()) {
            gui->getText("d-topleft")->setText('('+std::to_string(Engine::camera->getViewport().w)+','+std::to_string(Engine::camera->getViewport().y)+')')->show();
            gui->getText("d-bottomleft")->setText('('+std::to_string(Engine::camera->getViewport().w)+','+std::to_string(Engine::camera->getViewport().h)+')')->show();
            gui->getText("d-topright")->setText('('+std::to_string(Engine::camera->getViewport().x)+','+std::to_string(Engine::camera->getViewport().y)+')')->show();
            gui->getText("d-bottomright")->setText('('+std::to_string(Engine::camera->getViewport().x)+','+std::to_string(Engine::camera->getViewport().h)+')')->show();
            gui->getText("cam-pos")->setText("cam-pos ("+std::to_string(Engine::camera->getX())+","+std::to_string(Engine::camera->getY())+")")->show();
            // round scale
            std::stringstream l_scale;
            l_scale << std::fixed << std::setprecision(2) << (Engine::camera->getScale());
            gui->getText("cam-scale")->setText("cam-scale: "+l_scale.str())->show();
            gui->getText("player-pos")->setText("("+std::to_string(m_player.getX())+","+std::to_string(m_player.getY())+")");
            gui->getText("player-pos")->setPos(l_ppos.x+l_ppos.w/2, l_ppos.y+l_ppos.h/2)->show();
            gui->getText("player-pos")->setScale(0.6f);
        } else {
            gui->getText("player-pos")->hide();
            gui->getText("d-topleft")->hide();
            gui->getText("d-bottomleft")->hide();
            gui->getText("d-topright")->hide();
            gui->getText("d-bottomright")->hide();
            gui->getText("cam-pos")->hide();
            gui->getText("cam-scale")->hide();
        }
    */
}
void Game::end() {
    delete chat;
}
