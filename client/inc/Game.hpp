#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

#include "SDL2/SDL.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "core/GUIInput.hpp"
#include "core/InputManager.hpp"
#include "core/GUIText.hpp"
#include "core/InetConnection.hpp"
#include "core/ResourceManager.hpp"

#include "Player.hpp"

enum GameState { PLAY, PAUSE, EXIT };

class Game {
private:
    GameState m_gameState = GameState::EXIT;
    SDL_Window* m_window    = nullptr; // put this in own class
    SDL_Renderer* m_renderer = nullptr; // put this in window class
    InputManager* m_inputManager = nullptr; // put all inputs to one place (much wow)
    GUIText* m_guiText = nullptr;
    InetConnection* m_connection = new InetConnection();
    ResourceManager* R = nullptr;
    // player circle
    Player m_player;
    // camera whee
    Camera* m_camera = nullptr;
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
    // player name
    std::string m_nickname = "";
    std::string m_ip = "";
public:
    SDL_Texture* m_circle = nullptr;
    Game ();
    ~Game ();
    // same as start game, extend separate init if needed
    void run();
};
