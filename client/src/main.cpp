#include <iostream>
#include <string>

#include "Engine.hpp"

#include "scenes.hpp"

using namespace std;

int main(void) {
    Engine* engine = new Engine();
    import_scenes();
    engine->addScenes(scenes);
    engine->run("Game");
    delete engine;
    return 1;
}
