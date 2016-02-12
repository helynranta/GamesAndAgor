#ifndef _PLAYER_HPP
#define _PLAYER_HPP

#include <string>

#include "Circle.hpp"
#include "Engine.hpp"

using namespace std;

class Player : public Circle {
private:
    /* data */
    float m_speed = 2.0f;
    bool takeInput = true;
    Move* playermsg = nullptr;
public:
    inline Player (const string& n, int r, int g, int b) : Circle(n, false) {
        color.r = r;
        color.g = g;
        color.b = b;
    }
    inline virtual ~Player () {}

    inline void init() {}
    void update(float dT);
    inline void setTakeInput(bool i) {takeInput = i;}
};
#endif
