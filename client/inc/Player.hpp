#ifndef _PLAYER_HPP
#define _PLAYER_HPP

#include "Circle.hpp"

class Player : public Circle {
private:
    /* data */
    float m_speed       = 2.0f;
    float m_direction   = 1.0f;
public:
    inline Player () {}
    inline virtual ~Player () {}
    inline void init() {  }
    void update(float dT);
};
#endif
