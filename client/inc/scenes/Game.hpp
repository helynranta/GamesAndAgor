#ifndef _GAME_HPP
#define _GAME_HPP

#include <iostream>
#include <iomanip> // std::setprecision
#include <sstream> // std::stringstream
#include <string>
#include <vector>

#include "Engine.hpp"
#include "core/Scene.hpp"

#include "Player.hpp"
#include "Chat.hpp"


class Game : public Scene {
private:
    // player circle
    Player m_player = Player("player", 255, 255, 255);
    vector<Player> m_enemies;
    vector<Player> m_statics;
    Chat* chat = nullptr;
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
