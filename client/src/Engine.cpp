/*
*   Engine Class
*/

#include "Engine.hpp"

// statics
map<string, Scene*> Engine::m_scenes;
string Engine::m_currentScene = "";
GameState Engine::gameState = GameState::EXIT;
//debug
unsigned int Engine::debugKey = SDLK_o;
bool Engine::debugging = false;

Engine::Engine () {}
Engine::~Engine () {
    // close all scenes
    for ( auto& it : m_scenes) {
        delete it.second;
    }
    m_scenes.empty();

    R::destroy();
    Input::empty();
    InetConnection::disconnect();
    Window::destroy();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
int Engine::init() {
    int success = 1;
    // init whole sdl
    try {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    } catch (const InitError& err) {
        std::cerr << "Error while initializing SDL: " << err.what() << std::endl;
        success = 0;
    }
    // init img loader
    try{
        IMG_Init(IMG_INIT_PNG);
    } catch (const InitError& err) {
        std::cerr << "Could not initialize SDL_Image. Error: " << err.what() << std::endl;
        success = 0;
    }
    // init SDL_ttf
    try {
        TTF_Init();
    } catch(const InitError& err)
    {
        std::cerr << "Could not initialize SDL_ttf. Error: " << err.what() << std::endl;
        success = 0;
    }
    // create window
    success = Camera::init(800, 600);
    success = Window::init(800, 600);
    R::init(Window::getRenderer());
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
    while(gameState != GameState::EXIT)
    {
        Uint32 l_startTicks     = SDL_GetTicks();
        Uint32 l_frameTime      = l_startTicks - l_previousTicks;
        l_previousTicks         = l_startTicks;
        // get total deltaTime
        float l_totalDeltaTime = float(l_frameTime / DESIRED_FRAMETIME);
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
        draw();
        // if drawing happened too fast, we can sleep for a while
        Uint32 l_frameTicks = SDL_GetTicks() - l_startTicks;
        if( (1000.0f / 60.0f) > l_frameTicks)
           SDL_Delay( Uint32(1000.0f/ 60.0f - l_frameTicks));

        m_fps = (1000.0f / (SDL_GetTicks() - l_startTicks));
    }
    m_scenes[m_currentScene]->end();
}
void Engine::update() {
    if(gameState != GameState::PAUSE) {
        m_scenes[m_currentScene]->update(m_deltaTime);
        m_scenes[m_currentScene]->updateGUI();
        Camera::update();
    }
}
void Engine::processInput() {
    // update input manager
    Input::update();
    /*
    Poll all events
    Keycodes: https://wiki.libsdl.org/SDL_Keycode
    */
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_QUIT: // user quits window
                gameState = GameState::EXIT;
                break;
            case SDL_KEYDOWN: // key is down
                Input::pressKey(uint(e.key.keysym.sym));
                if(e.key.keysym.sym == SDLK_ESCAPE) // if user presses ESC
                    gameState = GameState::EXIT;
                break;
            case SDL_KEYUP:
                Input::releaseKey(uint(e.key.keysym.sym));
            default:
                break;
        }
    }
    if(Input::isKeyPressed(SDLK_p)) {
        if(gameState == PLAY) gameState = GameState::PAUSE;
        else gameState = GameState::PLAY;
    }
    if(int(debugKey) != -1) {
        if(Input::isKeyPressed(debugKey)) {
            if(debugging) debugging = false;
            else debugging = true;
        }
    }
}
void Engine::run(const std::string& name) {
    // if everyhing is ok, start gameloop
    if(init()) {
        std::cout << "Init Successful!" << std::endl;
        auto it = m_scenes.find(name);
        if(it != m_scenes.end()) {
            gameState = GameState::PLAY;
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
    SDL_SetRenderDrawColor(Window::getRenderer(), 50, 50, 50, 255);
    // clear screen
    SDL_RenderClear(Window::getRenderer());
    // draw current scene
    m_scenes[m_currentScene]->draw();
    m_scenes[m_currentScene]->drawGUI();
    // render buffer to screen
    SDL_RenderPresent(Window::getRenderer());
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
