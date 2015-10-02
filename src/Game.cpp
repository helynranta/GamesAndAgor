/*
*   Main Game Class
*
*/

#include "Game.hpp"

Game::Game () {}
Game::~Game () {
    TTF_CloseFont(m_font);
    m_font = nullptr;

    delete m_inputManager;

    SDL_DestroyTexture(m_circle);
    m_circle = nullptr;
    // clear sdl
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    m_renderer  = nullptr;
    m_window    = nullptr;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
int Game::init() {
    int success = 1;
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
        success = 0;
    }
    // init renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    // set clear color
    SDL_SetRenderDrawColor(m_renderer, 50, 50, 50, 255);
    // check if renderer init failed
    if(m_renderer == nullptr) {
        std::cout << "Could not create renderer " << std::string(SDL_GetError()) << std::endl;
        success = 0;
    }
    // init img loader
    if ( !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) )
    {
        std::cout << "Could not initialize SDL_Image. Error: " << std::string(IMG_GetError()) << std::endl;
        success = 0;
    }
    // init SDL_ttf
    if(TTF_Init() == -1)
    {
        std::cout << "Could not initialize SDL_ttf. Error: " << std::string(TTF_GetError()) << std::endl;
        success = 0;
    } else {
        m_font = TTF_OpenFont("res/Fonts/OpenSans.ttf", 64);
        if(m_font == nullptr)
        {
            std::cout << "Could not load OpenSans font. Error: " << std::string(TTF_GetError()) << std::endl;
            success = 0;
        } else {
            // tmp test init fps counter

        }
    }
    // load texture
    // http://lazyfoo.net/tutorials/SDL/07_texture_loading_and_rendering/index.php
    SDL_Surface* loadSurface = IMG_Load("res/circle.png"); // load pixels to surface
    if(loadSurface == nullptr) {
        std::cout << "Unable to load circle texture. Error: " << std::string(IMG_GetError()) << std::endl;
        success = 0;
    } else {
        m_circle = SDL_CreateTextureFromSurface(m_renderer, loadSurface);
        if(m_circle == nullptr) {
            std::cout << "Unable to create texture from circle.png" << std::string(SDL_GetError()) << std::endl;
            success = 0;
        }
        SDL_FreeSurface(loadSurface);
    }
    m_inputManager = new InputManager();
    // give player inputManager
    player.init(m_inputManager);
    // create gui text for fps counter
    m_fpsText = new GUIText();
    m_fpsText->setColor({200,200,200});
    // return what ever happened
    return success;
}
void Game::gameLoop() {

    const int MAX_PHYSICS_STEPS = 6;
    const float DESIRED_FRAMETIME = 1000.0f / 60.0f;
    const float MAX_DELTA_TIME = 1.0f;
    // count deltaTime
    Uint32 l_previousTicks = SDL_GetTicks();
    // main loop starts here
    while(m_gameState == GameState::PLAY)
    {
        Uint32 l_startTicks     = SDL_GetTicks();
        Uint32 l_frameTime      = l_startTicks - l_previousTicks;
        l_previousTicks         = l_startTicks;
        // get total deltaTime
        float l_totalDeltaTime = (float) l_frameTime / DESIRED_FRAMETIME;
        // if you lagg, you lagg... dont overdo it
        int i = 0;
        while (l_totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
        {
            m_deltaTime = std::min(l_totalDeltaTime, MAX_DELTA_TIME);
            // call update the game
            update();
            l_totalDeltaTime -= m_deltaTime;
            i++;
        }
        // clear screen
        SDL_RenderClear(m_renderer);
        // process SDL events
        processInput();
        // everything draw related
        draw();
        // render buffer to screen
        SDL_RenderPresent(m_renderer);
        // if drawing happened too fast, we can sleep for a while
        Uint32 l_frameTicks = SDL_GetTicks() - l_startTicks;
        if( (1000.0f / 60.0f) > l_frameTicks)
           SDL_Delay( (int)(1000.0f/ 60.0f - l_frameTicks));

        m_fps = (1000.0f / (SDL_GetTicks() - l_startTicks));
    }
}
void Game::update() {

    player.update(m_deltaTime);
    // CAMERA TESTING
    /*
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
    */
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
    // if everyhing is ok, start gameloop
    if(init()) {
        std::cout << "Init Successful!" << std::endl;
        gameLoop();
    } else {
        std::cout << "Something happened during game initialization, stop game..." << std::endl;
    }
}
void Game::draw() {
    // render circle to screen
    // m_fpsText.renderText("FPS:", {0,0,0}, {0,0,0});
    SDL_Rect player_position = m_camera.transformToWorldCordinates(player.getDestRect());
    SDL_RenderCopy(m_renderer, m_circle, NULL, &player_position );

    static int lastUpdate = 0;
    if(lastUpdate < 10000) {
        m_fpsText->renderText(10,10, std::string("FPS: ")+std::to_string(int(m_fps)) , *m_renderer, *m_font);
    }

}
