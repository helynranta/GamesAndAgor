#pragma once

#include "SDL2/SDL.h"

#include "Camera.hpp"

class Circle {
private:
    float m_x = 0;
    float m_y = 0;
    float m_r = 100;
    SDL_Rect m_destRect = {(int)m_x, (int)m_y, (int)m_r*2, (int)m_r*2};
public:
    inline Circle () {;}
    inline Circle (int x, int y, int r) : m_x(x), m_y(y), m_r(r) {}
    virtual ~Circle () {;}

    void init();
    void update(Camera cam);
    void move(int x, int y); // for testing
    /* GETTERS */
    inline const int getX () const { return m_x; }
    inline const int getY () const { return m_y; }
    inline const int getR () const { return m_r; }
    inline const SDL_Rect& getDestRect() const { return m_destRect; }
};
