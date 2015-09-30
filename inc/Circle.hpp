#pragma once

#include "SDL2/SDL.h"

class Circle {
private:
    float m_x = 100;
    float m_y = 100;
    unsigned int m_r = 100;
    SDL_Rect m_destRect = {m_x, m_y, m_r*2, m_r*2};
public:
    inline Circle () {;}
    inline Circle (int x, int y, int r) : m_x(x), m_y(y), m_r(r) {}
    virtual ~Circle () {;}

    void init();
    void update();
    void move(int x, int y);
    /* GETTERS */
    inline const int getX () const { return m_x; }
    inline const int getY () const { return m_y; }
    inline const int getR () const { return m_r; }
    inline const SDL_Rect& getDestRect() const { return m_destRect; }
};
