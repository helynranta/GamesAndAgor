#include <iostream>
#include <string>

#include "Engine.hpp"

#include "scenes.h"

using namespace std;

int main(void) {
    Engine* engine = new Engine();

    pair<string, Scene*> menu("Menu", new Menu());
    pair<string, Scene*> game("Game", new Game());

    engine->addScene(menu);
    engine->addScene(game);

    engine->run("Menu");
    delete engine;
    return 1;
}
