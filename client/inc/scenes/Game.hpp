#ifndef _GAME_HPP
#define _GAME_HPP

#include <iostream>
#include <iomanip> // std::setprecision
#include <sstream> // std::stringstream
#include <string>
#include <vector>

#include "Engine.hpp"

#include "Player.hpp"

class Game : public Scene {
private:
    // player circle
    Player m_player;
public:
    inline Game () : Scene() {}
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

#endif
