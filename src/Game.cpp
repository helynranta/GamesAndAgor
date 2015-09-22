/*
*   Main Game Class
*
*/

#include "Game.hpp"

Game::Game () {}
Game::~Game () {
    // clear sdl
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    // delete pointers
    delete m_window;
    delete m_renderer;
}
void Game::update() {
    while(m_gameState == GameState::PLAY) {
        // clear screen
        SDL_RenderClear(m_renderer);
        // process inputs, put this in own class
        processInput();
        // render buffer to screen
        SDL_RenderPresent(m_renderer);
    }
}
void Game::processInput() {
    SDL_Event e;
    /* Poll all events
    Keycodes: https://wiki.libsdl.org/SDL_Keycode
    */
    while (SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_QUIT: // user quits window
                m_gameState = GameState::EXIT;
                break;
            case SDL_KEYDOWN: // key is down
                if(e.key.keysym.sym == SDLK_ESCAPE) // if user presses ESC
                    m_gameState = GameState::EXIT;
                break;
        }
    }
}
void Game::run() {
    // set gamestate
    m_gameState = GameState::PLAY;
    // init whole sdl
    SDL_Init(SDL_INIT_EVERYTHING);
    // create window
    m_window = SDL_CreateWindow("WINDOW",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480, 0);
    // check if window creation failed
    if(m_window == nullptr) {
        std::cout << "Could not create window " << std::string(SDL_GetError()) << std::endl;
        m_gameState = GameState::EXIT;
    }
    // init renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    // set clear color
    SDL_SetRenderDrawColor(m_renderer, 50, 50, 50, 255);
    // check if renderer init failed
    if(m_renderer == nullptr) {
        std::cout << "Could not create renderer " << std::string(SDL_GetError()) << std::endl;
        m_gameState = GameState::EXIT;
    }
    // if everyhing is ok, start gameloop
    if(m_gameState == GameState::PLAY) update();
}
