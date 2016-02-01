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
    /*function<void()> f = bind([](int x)  {cout << x << endl;}, 1);
    map<int, function<void()>> _functionMap;
    _functionMap.insert({100,
        bind([](int x)  {
            cout << x << endl;
        }, 10)
    });
    for(auto& it : _functionMap) {
        it.second();
    }
    */

    Engine* engine = new Engine();
    engine->addScenes({
        {"Game",new Game()},
        {"NickDialog", new NickDialog()},
        {"IPDialog", new IPDialog()}
    });
    engine->run("IPDialog");
    delete engine;
    
    return 0;
}
