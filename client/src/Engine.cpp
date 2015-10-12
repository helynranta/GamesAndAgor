/*
*   Engine Class
*/

#include "Engine.hpp"

Engine::Engine () {}
Engine::~Engine () {
    // close all scenes
    for ( auto& it : m_scenes) {
        delete it.second;
    }
    m_scenes.empty();
    // delete resource manager
    delete R;
    R = nullptr;
    // delete connection handler
    delete m_connection;
    m_connection = nullptr;
    // delete input amanger
    delete inputManager;
    inputManager = nullptr;
    // delete camera
    delete camera;
    camera = nullptr;
    // clear sdl
    SDL_DestroyRenderer(renderer);
    renderer  = nullptr;
    SDL_DestroyWindow(m_window);
    m_window    = nullptr;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
int Engine::init() {
    int success = 1;
    // set gamestate
    m_gameState = GameState::PLAY;
    // init whole sdl
    SDL_Init(SDL_INIT_EVERYTHING);
    //
    camera = new Camera(800, 640);
    // create window
    m_window = SDL_CreateWindow("Agor and Gamus",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        camera->getWidth(), camera->getHeight(), 0);
    // check if window creat>ion failed
    if(m_window == nullptr) {
        std::cout << "Could not create window " << std::string(SDL_GetError()) << std::endl;
        success = 0;
    }
    // init renderer
    renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    // set clear color
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    // check if renderer init failed
    if(renderer == nullptr) {
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
    }
    R             = new ResourceManager(renderer);
    inputManager  = new InputManager();
    m_connection  = new InetConnection();
    // return what ever happened
    return success;
}
void Engine::gameLoop() {

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
        // process SDL events
        processInput();
        // everything draw related
        camera->update();
        draw();
        // if drawing happened too fast, we can sleep for a while
        Uint32 l_frameTicks = SDL_GetTicks() - l_startTicks;
        if( (1000.0f / 60.0f) > l_frameTicks)
           SDL_Delay( (int)(1000.0f/ 60.0f - l_frameTicks));

        m_fps = (1000.0f / (SDL_GetTicks() - l_startTicks));
    }
    m_scenes[m_currentScene]->end();
}
void Engine::update() {
    m_scenes[m_currentScene]->update(m_deltaTime);
}
void Engine::processInput() {
    // update input manager
    inputManager->update();
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
                inputManager->pressKey(e.key.keysym.sym);
                if(e.key.keysym.sym == SDLK_ESCAPE) // if user presses ESC
                    m_gameState = GameState::EXIT;
                break;
            case SDL_KEYUP:
                inputManager->releaseKey(e.key.keysym.sym);
        }
    }

}
void Engine::run(const std::string& name) {
    // if everyhing is ok, start gameloop
    if(init()) {
        std::cout << "Init Successful!" << std::endl;
        auto it = m_scenes.find(name);
        if(it != m_scenes.end()) {
            std::cout << "Starting first scene: " << m_currentScene << std::endl;
            startScene(name);
            gameLoop();
        }
        else
            std::cout << "Couldn't find scene with name: " << name << " at source..." << std::endl;
    } else {
        std::cout << "Something happened during game initialization, stop game..." << std::endl;
    }
}
void Engine::draw() {
    // set clear color
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    // clear screen
    SDL_RenderClear(renderer);
    // draw current scene
    m_scenes[m_currentScene]->draw();
    m_scenes[m_currentScene]->drawGUI();
    // render buffer to screen
    SDL_RenderPresent(renderer);
}
bool Engine::startScene(string name){
    auto it = m_scenes.find(name);
    if(it == m_scenes.end()) {
        std::cout << "Cannot start undefined scene: " << name << std::endl;
        return false;
    }
    if(m_currentScene != "") {
        m_scenes[m_currentScene]->end();
    }
    m_currentScene = name;
    m_scenes[m_currentScene]->awake();
    return true;
}
