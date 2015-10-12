#include "scenes/Game.hpp"

void Game::awake() {
    gui = new GUI(engine->renderer);
    std::cout << "Game awake" << std::endl;
    m_player.init(engine->inputManager);

    //gui->add("TEST", new GUIText(engine->renderer, engine->R->getFont("res/fonts/OpenSans.ttf")));
    //gui->get("TEST")->setText("useita")->setX(200)->setY(200);
}
void Game::update(float dt) {
    m_player.update(dt);
    // this is how camera behaves in real gameplay
    if(engine->inputManager->isKeyDown(SDLK_q))
        m_player.scale(-1.0f);
    if(engine->inputManager->isKeyDown(SDLK_e))
        m_player.scale(1.0f);
    engine->camera->setPos(m_player.getX(), m_player.getY());
    engine->camera->setScale(float(m_player.getR())/100);
}
void Game::draw() {
    SDL_Rect l_ppos = engine->camera->transformToWorldCordinates(m_player.getDestRect());
    SDL_RenderCopy(engine->renderer, engine->R->getTexture("res/circle.png"), NULL, &l_ppos );
}
void Game::end() {
    std::cout << "Game ends" << std::endl;
}

// CAMERA TESTING
#if 0
    if(engine->inputManager->isKeyDown(SDLK_q))
        engine->camera->scale(-0.5f);
    if(engine->inputManager->isKeyDown(SDLK_e))
        engine->camera->scale(0.5f);
    if(engine->inputManager->isKeyDown(SDLK_a))
        engine->camera->moveX(-1);
    if(engine->inputManager->isKeyDown(SDLK_d))
        engine->camera->moveX(1);
    if(engine->inputManager->isKeyDown(SDLK_w))
        engine->camera->moveY(1);
    if(engine->inputManager->isKeyDown(SDLK_s))
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
