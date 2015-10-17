#ifndef _SCENE_HPP
#define _SCENE_HPP

#include "GUI/GUI.hpp"

class Engine;
class Scene {
protected:
    GUI* gui = nullptr;
public:
    Scene();
    virtual ~Scene();
    virtual void awake() = 0;
    virtual void update(float dt) = 0;
    virtual void end() = 0;
    virtual void draw() = 0;
    inline GUI* getGUI() { return gui; }
    inline void drawGUI() { if(gui != nullptr) gui->draw(); }
    inline void updateGUI() { if(gui != nullptr) gui->update(); }
    inline void onPause() {;}
    inline void onExit() {;}    
};
#endif
