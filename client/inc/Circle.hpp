#ifndef _CIRCLE_HPP
#define _CIRCLE_HPP

#include "SDL2/SDL.h"

#include "core/Camera.hpp"

class Circle {
protected:
    float m_x = 0;
    float m_y = 0;
    float m_r = 100;
    SDL_Rect m_destRect = {int(m_x), int(m_y), int(m_r*2), int(m_r*2)};
public:
    inline Circle () {;}
    inline Circle (int x, int y, int r) : m_x(x), m_y(y), m_r(r) {}
    virtual ~Circle () {;}

    void init();
    void update();
    void move(int x, int y); // for testing
    /* GETTERS */
    inline int getX () const { return m_x; }
    inline int getY () const { return m_y; }
    inline int getR () const { return m_r; }
    inline const SDL_Rect& getDestRect() const { return m_destRect; }

    inline void scale(float x) {
        m_r += x;
        if(m_r < 10) m_r = 10;
    }
};

#endif
