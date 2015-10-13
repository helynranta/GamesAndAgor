#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Engine.hpp"

#include "Player.hpp"

class Game : public Scene {
private:
    // player circle
    Player m_player;
public:
    inline Game (Engine* e) : Scene(e) {}
    inline ~Game () {}
    // game init
    void awake();
    // this runs game
    void gameLoop();
    // this does game update
    void update(float dt);
    // end
    void end();
    // generic drawing function
    void draw();
};
