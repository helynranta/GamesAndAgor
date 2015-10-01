/*
*   Main Game Class
*
*/

#include "Game.hpp"

#include "SDL2/SDL_image.h"

Game::Game () {}
Game::~Game () {
    delete m_inputManager;

    SDL_DestroyTexture(m_circle);
    m_circle = nullptr;
    // clear sdl
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    m_renderer  = nullptr;
    m_window    = nullptr;

    IMG_Quit();
    SDL_Quit();
}
void Game::init() {
    // load texture
    // http://lazyfoo.net/tutorials/SDL/07_texture_loading_and_rendering/index.php
    SDL_Surface* loadSurface = IMG_Load("res/circle.png"); // load pixels to surface
    if(loadSurface == nullptr) {
        std::cout << "Unable to load circle texture. Error: " << std::string(IMG_GetError()) << std::endl;
    } else {
        m_circle = SDL_CreateTextureFromSurface(m_renderer, loadSurface);
        if(m_circle == nullptr) {
            std::cout << "Unable to create texture from circle.png" << std::string(SDL_GetError()) << std::endl;
        }
        SDL_FreeSurface(loadSurface);
    }
    m_inputManager = new InputManager();
    // give player inputManager
    player.init(m_inputManager);
}
void Game::update() {
    while(m_gameState == GameState::PLAY) {
        // clear screen
        SDL_RenderClear(m_renderer);
        // process inputs, put this in own class
        processInput();
        player.update();
        // CAMERA TESTING
        if(m_inputManager->isKeyDown(SDLK_q))
            m_camera.scale(-.1f);
        if(m_inputManager->isKeyDown(SDLK_e))
            m_camera.scale(.1f);
        if(m_inputManager->isKeyDown(SDLK_a))
            m_camera.moveX(-.1f);
        if(m_inputManager->isKeyDown(SDLK_d))
            m_camera.moveX(.1f);
        if(m_inputManager->isKeyDown(SDLK_w))
            m_camera.moveY(-.1f);
        if(m_inputManager->isKeyDown(SDLK_s))
            m_camera.moveY(.1f);
        // draw
        draw();
        // render buffer to screen
        SDL_RenderPresent(m_renderer);
    }
}
void Game::processInput() {
    // update input manager
    m_inputManager->update();
    /*
    Poll all events
    Keycodes: https://wiki.libsdl.org/SDL_Keycode
    */
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_QUIT: // user quits window
                m_gameState = GameState::EXIT;
                break;
            case SDL_KEYDOWN: // key is down
                m_inputManager->pressKey(e.key.keysym.sym);
                if(e.key.keysym.sym == SDLK_ESCAPE) // if user presses ESC
                    m_gameState = GameState::EXIT;
                break;
            case SDL_KEYUP:
                m_inputManager->releaseKey(e.key.keysym.sym);
        }
    }

}
void Game::run() {
    // set gamestate
    m_gameState = GameState::PLAY;
    // init whole sdl
    SDL_Init(SDL_INIT_EVERYTHING);
    // create window
    m_window = SDL_CreateWindow("Agor and Gamus",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_camera.getWidth(), m_camera.getHeight(), 0);
    // check if window creation failed
    if(m_window == nullptr) {
        std::cout << "Could not create window " << std::string(SDL_GetError()) << std::endl;
        m_gameState = GameState::EXIT;
    }
    // init renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    // set clear color
    SDL_SetRenderDrawColor(m_renderer, 50, 50, 50, 255);
    // check if renderer init failed
    if(m_renderer == nullptr) {
        std::cout << "Could not create renderer " << std::string(SDL_GetError()) << std::endl;
        m_gameState = GameState::EXIT;
    }
    // init img loader
    if ( !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) )
    {
        std::cout << "Could not initialize SDL_Image. Error: " << std::string(IMG_GetError()) << std::endl;
        m_gameState = GameState::EXIT;
    }
    init();
    // if everyhing is ok, start gameloop
    if(m_gameState == GameState::PLAY) {
        std::cout << "Init Successful!" << std::endl;
        update();
    }
}
void Game::draw() {
    // render circle to screen
    SDL_Rect player_position = m_camera.transformToWorldCordinates(player.getDestRect());
    SDL_RenderCopy(m_renderer, m_circle, NULL, &player_position );
}
