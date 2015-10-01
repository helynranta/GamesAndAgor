#pragma once

#include "Circle.hpp"

#include "core/InputManager.hpp"

class Player : public Circle {
private:
    /* data */
    InputManager* m_inputManager;
public:
    inline Player () : m_inputManager() {}
    inline virtual ~Player () {}
    inline void init(InputManager* i) { m_inputManager = i; }
    void update();
};
