#pragma once

#include <iostream>
#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_events.h"

#include "core/InputManager.hpp"

#include "Player.hpp"

enum GameState { PLAY, PAUSE, EXIT };

class Game {
private:
    GameState m_gameState = GameState::EXIT;
    SDL_Window* m_window    = nullptr; // put this in own class
    SDL_Renderer* m_renderer = nullptr; // put this in window class
    InputManager* m_inputManager = nullptr; // put all inputs to one place (much wow)

    void init();
    // gameloop
    void update();
    // generic drawing function
    void draw();
    // move this to input manager
    void processInput();

    Player player;
    Camera m_camera = Camera(800, 640);
public:
    SDL_Texture* m_circle = nullptr;
    Game ();
    ~Game ();
    // same as start game, extend separate init if needed
    void run();
};
