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
    Player* m_player;
    vector<Player> m_enemies;
    vector<Player> m_statics;
    Chat* chat = nullptr;
public:
    inline Game () : Scene() {}
    inline ~Game () {}
    // game init
    void awake(void);
    // this runs game
    void gameLoop(void);
    // this does game update
    void update(float dt);
    void updateChat(void);
    void handleMessages(void);
    // end
    void end(void);
    // generic drawing function
    void draw(void);
};

#endif
