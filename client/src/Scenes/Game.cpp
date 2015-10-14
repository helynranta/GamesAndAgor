#include "scenes/Game.hpp"

void Game::awake() {

    gui = new GUI(Window::getRenderer());
    m_player.init();

    //gui->add("TEST", new GUIText(engine->renderer, engine->R->getFont("res/fonts/OpenSans.ttf")));
    //gui->get("TEST")->setText("useita")->setX(200)->setY(200);
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

    SDL_Rect vw = camera->getViewport();
    guiText->renderText(10,10,"("+std::to_string(vw.w)+","+std::to_string(vw.y)+")");
    guiText->renderText(10,camera->getHeight() - 50,"("+std::to_string(vw.w)+","+std::to_string(vw.h)+")");
    guiText->renderText(camera->getWidth() - 110,10,
        "("+std::to_string(vw.x)+","+std::to_string(vw.y)+")"
        );
    guiText->renderText(camera->getWidth()-110, camera->getHeight()-50,
        "("+std::to_string(vw.x)+","+std::to_string(vw.h)+")"
        );
#endif
