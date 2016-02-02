#ifndef _PLAYER_HPP
#define _PLAYER_HPP

#include <string>

#include "Circle.hpp"


using namespace std;

struct Color {
    int r = 0;
    int g = 0;
    int b = 0;
};

class Player : public Circle {
private:
    /* data */
    float m_speed       = 2.0f;
    float m_direction   = 1.0f;
    Color color;
    string nick = "";
public:
    inline Player (const string& n, int r, int g, int b) {
        nick = n;
        color.r = r;
        color.g = g;
        color.b = b;
    }
    inline virtual ~Player () {

    }
    inline void init() {
    }
    void update(float dT);
    inline const Color& getColor() const {return color;}
    inline const string& getNick() const { return nick; }
};
#endif
