#pragma once

#include <SDL2/SDL.h>

class Camera {
private:
    float m_x = 0;
    float m_y = 0;
    int m_vw = 800;     // viewport width
    int m_vh = 640;     // viewport height
    float m_scale = 1.0f;
    // should be set only once plz
    unsigned int m_width = 800;
    unsigned int m_height = 640;

public:
    inline Camera() {;}
    inline Camera(int width, int height)
    : m_width(width), m_height(height) {;}
    virtual ~Camera () {}

    int init();
    void destroy();
    SDL_Rect transformToWorldCordinates(SDL_Rect rect);
    /* GETTERS */
    inline const int getX() const { return m_x; }
    inline const int getY() const { return m_y; }
    inline const int getWidth() const { return m_vw; }
    inline const int getHeight() const { return m_vh; }

    inline const float getScaleX() const { return m_width/m_vw; }
    /* SETTERS */
    inline void setX(int x) { m_x += x; }
    inline void setY(int y) { m_y += y; }
    inline void setPos( int x, int y ) { m_x = x; m_y = y; }
    void setScale(float scale);
};
