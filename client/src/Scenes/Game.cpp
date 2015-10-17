#include "scenes/Game.hpp"

void Game::awake() {

    m_player.init();
    gui->addText("PAUSED", new GUIText(Window::getRenderer(), R::getFont("res/fonts/OpenSans.ttf")));
    //gui->getText("PAUSED")->hide();
    gui->getText("PAUSED");
    gui->getText("PAUSED")->setX(Camera::getWidth()/2)->setY(Camera::getHeight()/2);
    gui->getText("PAUSED")->setAlign(TEXT_ALIGN::CENTER_XY);
    gui->getText("PAUSED")->setColor({200,10,10,255});
    // INIT DEBUG PRINTS
    gui->addText("d-topleft", new GUIText(Window::getRenderer(), R::getFont("res/fonts/OpenSans.ttf")));

    gui->addText("d-topright", new GUIText(Window::getRenderer(), R::getFont("res/fonts/OpenSans.ttf")));
    gui->getText("d-topright")->setX(Camera::getWidth());
    gui->getText("d-topright")->setAlign(TEXT_ALIGN::RIGHT);

    gui->addText("d-bottomleft", new GUIText(Window::getRenderer(), R::getFont("res/fonts/OpenSans.ttf")));
    gui->getText("d-bottomleft")->setY(Camera::getHeight());
    gui->getText("d-bottomleft")->setAlign(TEXT_ALIGN::OVER);

    gui->addText("d-bottomright", new GUIText(Window::getRenderer(), R::getFont("res/fonts/OpenSans.ttf")));
    gui->getText("d-bottomright")->setX(Camera::getWidth())->setY(Camera::getHeight());
    gui->getText("d-bottomright")->setAlign(TEXT_ALIGN::OVER_RIGHT);

    gui->addText("cam-pos", new GUIText(Window::getRenderer(), R::getFont("res/fonts/OpenSans.ttf")));
    gui->getText("cam-pos")->setX(Camera::getWidth()/2)->setAlign(TEXT_ALIGN::CENTER);

    gui->addText("cam-scale", new GUIText(Window::getRenderer(), R::getFont("res/fonts/OpenSans.ttf")));
    gui->getText("cam-scale")->setX(Camera::getWidth()/2)->setAlign(TEXT_ALIGN::CENTER)->setY(30);

    gui->addText("player-pos", new GUIText(Window::getRenderer(), R::getFont("res/fonts/OpenSans.ttf")));
    gui->getText("player-pos")->setAlign(TEXT_ALIGN::CENTER_XY);
    gui->getText("player-pos")->setColor({50,50,50,255});
/*    */
}
void Game::update(float dt) {
    m_player.update(dt);
    // this is how camera behaves in real gameplay
    //Camera::setPos(m_player.getX(), m_player.getY());
    //Camera::setScale(float(m_player.getR())/100);

    if(Input::isKeyDown(SDLK_q))
        Camera::scale(-0.5f);
    if(Input::isKeyDown(SDLK_e))
        Camera::scale(0.5f);
    if(Input::isKeyDown(SDLK_a))
        Camera::moveX(-1);
    if(Input::isKeyDown(SDLK_d))
        Camera::moveX(1);
    if(Input::isKeyDown(SDLK_w))
        Camera::moveY(1);
    if(Input::isKeyDown(SDLK_s))
        Camera::moveY(-1);

}
void Game::draw() {
    SDL_Rect l_ppos = Camera::transformToWorldCordinates(m_player.getDestRect());
    SDL_RenderCopy(Window::getRenderer(), R::getTexture("res/circle.png"), NULL, &l_ppos );

    if(Engine::gameState == GameState::PAUSE)
        gui->getText("PAUSED")->show();
    else
        gui->getText("PAUSED")->hide();

    // ALL DEBUG PRINTS!
    if(Engine::isDebugging()) {
        gui->getText("d-topleft")->setText('('+std::to_string(Camera::getViewport().w)+','+std::to_string(Camera::getViewport().y)+')')->show();
        gui->getText("d-bottomleft")->setText('('+std::to_string(Camera::getViewport().w)+','+std::to_string(Camera::getViewport().h)+')')->show();
        gui->getText("d-topright")->setText('('+std::to_string(Camera::getViewport().x)+','+std::to_string(Camera::getViewport().y)+')')->show();
        gui->getText("d-bottomright")->setText('('+std::to_string(Camera::getViewport().x)+','+std::to_string(Camera::getViewport().h)+')')->show();
        gui->getText("cam-pos")->setText("cam-pos ("+std::to_string(Camera::getX())+","+std::to_string(Camera::getY())+")")->show();
        // round scale
        std::stringstream l_scale;
        l_scale << std::fixed << std::setprecision(2) << (Camera::getScale());
        gui->getText("cam-scale")->setText("cam-scale: "+l_scale.str())->show();
        gui->getText("player-pos")->setText("("+std::to_string(m_player.getX())+","+std::to_string(m_player.getY())+")");
        gui->getText("player-pos")->setPos(l_ppos.x+l_ppos.w/2, l_ppos.y+l_ppos.h/2)->show();
        gui->getText("player-pos")->setScale(0.6f/Camera::getScale());
    } else {
        gui->getText("player-pos")->hide();
        gui->getText("d-topleft")->hide();
        gui->getText("d-bottomleft")->hide();
        gui->getText("d-topright")->hide();
        gui->getText("d-bottomright")->hide();
        gui->getText("cam-pos")->hide();
        gui->getText("cam-scale")->hide();
    }
}
void Game::end() {

}
