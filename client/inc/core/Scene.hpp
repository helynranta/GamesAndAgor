#pragma once

#include "GUI/GUI.hpp"

class Engine;
class Scene {
protected:
    GUI* gui = nullptr;
public:
    inline Scene() {}
    inline virtual ~Scene() {;}
    virtual void awake() = 0;
    virtual void update(float dt) = 0;
    virtual void end() = 0;
    virtual void draw() = 0;
    inline void drawGUI() { if(gui != nullptr) gui->draw(); }
    inline void updateGUI() { if(gui != nullptr) gui->update(); }
};
