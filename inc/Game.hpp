#pragma once

#include <iostream>
#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "core/InputManager.hpp"
#include "core/GUIText.hpp"

#include "Player.hpp"

enum GameState { PLAY, PAUSE, EXIT };

class Game {
private:
    GameState m_gameState = GameState::EXIT;
    SDL_Window* m_window    = nullptr; // put this in own class
    SDL_Renderer* m_renderer = nullptr; // put this in window class
    InputManager* m_inputManager = nullptr; // put all inputs to one place (much wow)
    TTF_Font* m_font = nullptr;
    GUIText* m_fpsText = nullptr;
    // whole fucking game init
    int init();
    // this runs game
    void gameLoop();
    // calculate and show fps
    void calculateFPS();
    // this does game update
    void update();
    // generic drawing function
    void draw();
    // updates input manager, reads ALL sdl input events
    void processInput();
    // does nothing atm
    void updateGUI();
    // player circle
    Player player;
    // camera whee
    Camera m_camera = Camera(800, 640);
    // time related
    float m_fps;
    // delta time reference: http://gafferongames.com/game-physics/fix-your-timestep/
    float m_deltaTime;
public:
    SDL_Texture* m_circle = nullptr;
    Game ();
    ~Game ();
    // same as start game, extend separate init if needed
    void run();
};
