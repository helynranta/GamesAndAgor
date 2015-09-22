#pragma once

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include "SDL2/SDL_events.h"

enum GameState { PLAY, PAUSE, EXIT };

class Game {
private:
    GameState m_gameState = GameState::EXIT;
    SDL_Window* m_window    = nullptr; // put this in own class
    SDL_Renderer* m_renderer = nullptr; // put this in window class
    // gameloop
    void update();
    // move this to input manager
    void processInput();
public:
    Game ();
    ~Game ();
    // same as start game, extend separate init if needed
    void run();
};
