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
    map<uint16_t, Circle*> m_enemies;
    map<uint16_t, Circle*> m_statics;

    vector<Circle*> drawables;

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
    void doGameUpdate(void);
    // end
    void end(void);
    // generic drawing function
    void draw(void);
};

#endif
