#pragma once

#include <initializer_list>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

#include "SDL2/SDL.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "core/InputManager.hpp"
#include "core/Scene.hpp"
#include "core/ResourceManager.hpp"

#include "GUI/GUIInput.hpp"
#include "GUI/GUIText.hpp"

#include "Inet/InetConnection.hpp"

enum GameState { PLAY, PAUSE, EXIT };
enum GameScene { IP, NICK, INIT, GAME };

using namespace std;

class Engine {
public:
    GameState m_gameState = GameState::EXIT;
    SDL_Window* m_window    = nullptr; // put this in own class
    SDL_Renderer* renderer = nullptr; // put this in window class
    Camera* camera = nullptr;
    // whole fucking game init
    int init();
    // ask for nickname and ip
    void menu();
    // this runs game
    void gameLoop();
    // this does game update
    void update();
    // generic drawing function
    void draw();
    // updates input manager, reads ALL sdl input events
    void processInput();
    // time related
    float m_fps;
    // delta time reference: http://gafferongames.com/game-physics/fix-your-timestep/
    float m_deltaTime;
    string m_currentScene = "";

    Engine ();
    ~Engine ();
    void run(const std::string& name);
    inline Camera* getCamera() const { return camera; }
    inline SDL_Renderer* getRenderer() const { return renderer; }
    map<string, Scene*> m_scenes;
    inline void addScene(pair<string,Scene*> scene) {
        m_scenes.insert(scene);
    }
    bool startScene(string name);
};
