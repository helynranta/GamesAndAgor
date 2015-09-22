#pragma once

enum GameState { PLAY, PAUSE, EXIT };

class Game {
private:
    GameState m_gameState = GameState::EXIT;
public:
    Game ();
    ~Game ();
    // same as start game, extend separate init if needed
    void run();
    // gameloop
    void update();

};
