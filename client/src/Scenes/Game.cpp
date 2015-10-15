#include "scenes/Game.hpp"

void Game::awake() {
    m_player.init();
    gui->addText("PAUSED", new GUIText(Window::getRenderer(), R::getFont("res/fonts/OpenSans.ttf")));
    //gui->getText("PAUSED")->hide();
    gui->getText("PAUSED");
    gui->getText("PAUSED")->setX(Camera::getWidth()/2)->setY(Camera::getHeight()/2);
    gui->getText("PAUSED")->setAlign(TEXT_ALIGN::CENTER_XY);
    gui->getText("PAUSED")->setColor({200,10,10});
    // debug prints
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

    gui->getText("d-topleft")->setText('('+std::to_string(Camera::getViewport().w)+','+std::to_string(Camera::getViewport().y)+')');
    gui->getText("d-bottomleft")->setText('('+std::to_string(Camera::getViewport().w)+','+std::to_string(Camera::getViewport().h)+')');
    gui->getText("d-topright")->setText('('+std::to_string(Camera::getViewport().x)+','+std::to_string(Camera::getViewport().y)+')');
    gui->getText("d-bottomright")->setText('('+std::to_string(Camera::getViewport().x)+','+std::to_string(Camera::getViewport().h)+')');
}
void Game::end() {

}

// CAMERA TESTING
#if 0
    if(Input::isKeyDown(SDLK_q))
        engine->camera->scale(-0.5f);
    if(Input::isKeyDown(SDLK_e))
        engine->camera->scale(0.5f);
    if(Input::isKeyDown(SDLK_a))
        engine->camera->moveX(-1);
    if(Input::isKeyDown(SDLK_d))
        engine->camera->moveX(1);
    if(Input::isKeyDown(SDLK_w))
        engine->camera->moveY(1);
    if(Input::isKeyDown(SDLK_s))
        engine->camera->moveY(-1);
#endif
// debug prints
#if 0
    guiText->setColor({200,200,200});

    std::stringstream l_scale;
    l_scale << std::fixed << std::setprecision(2) << float(camera->getScale());
    guiText->renderText(camera->getWidth()/2-100,10, "Camera scale: "+l_scale.str());

    guiText->renderText(camera->getWidth()/2-100,40, "Camera pos: ("+std::to_string(camera->getX())+","+std::to_string(camera->getY())+")");
#endif
