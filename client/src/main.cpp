#include <iostream>
#include <string>

#include "Engine.hpp"

#include "scenes/Game.hpp"
#include "scenes/IPDialog.hpp"
#include "scenes/NickDialog.hpp"

using namespace std;

#include <functional>
#include <memory>
#include <map>

int main(void) {
    Engine* engine = new Engine();
    engine->addScenes({
        {"Game",new Game()},
        {"NickDialog", new NickDialog()},
        {"IPDialog", new IPDialog()}
    });
    engine->run("Game");
    delete engine;
    
    return 0;
}
