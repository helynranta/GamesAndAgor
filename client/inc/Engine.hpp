#ifndef _ENGINE_HPP
#define _ENGINE_HPP

#include <initializer_list>
#include <functional>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <stdarg.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "core/InitError.hpp"
// statics
#include "core/Camera.hpp"
#include "core/InputManager.hpp"
#include "core/ResourceManager.hpp"
#include "core/Window.hpp"

#include "GUI/GUIInput.hpp"
#include "GUI/GUIText.hpp"

#include "Inet/InetConnection.hpp"


enum GameState { PLAY, PAUSE, QUIT };

using namespace std;

class Scene;
class Engine {
private:
    /* private data */
    // whole fucking game init
    int init();
    // ask for nickname and ip
    void menu();
    // this runs game
    void gameLoop();
    // this does game update
    void update();
    // generic drawing function
    void draw();
    // updates input manager, reads ALL sdl input events
    void processInput();
    // time related
    float m_fps = 0;
    // delta time reference: http://gafferongames.com/game-physics/fix-your-timestep/
    float m_deltaTime = 0;
    static unsigned int debugKey;
    static bool debugging;
    // vector of lambda functions
    static map<int, function<void()>> _functions;
    static void invokeTimeout();
    static string nick;
protected:
    /* protected data */
    static map<string, Scene*> m_scenes;
    static string m_currentScene;
public:
    /* public data */
    Engine ();
    ~Engine ();
    void run(const std::string& name);
    /* static SCENE*/
    static bool startScene(string name);
    void addScene(pair<string,Scene*> scene);
    void addScenes(vector<pair<string,Scene*>> scenes);
    /* static GAME */
    static GameState gameState;
    static void setDebugKey(unsigned int key) { debugKey = key; }
    static void setDebugingMode(bool d) { debugging = d; }
    static bool isDebugging()  { return debugging; }

    static InetConnection* connection;
    static Camera* camera;
    static ResourceManager* R;
    static InputManager* input;
    static Window* window;
    static const string& getNick() { return nick; }
    static const void setNick(const string& n) { nick = n; }
    static inline void setTimeout(int _ms, function<void()> function) {
        _functions.insert({SDL_GetTicks()+_ms, function});
    }
};

#endif
