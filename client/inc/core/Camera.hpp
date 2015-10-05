#pragma once

#include <SDL2/SDL.h>

class Camera {
private:
    float m_x = 0;
    float m_y = 0;
    float m_scale = 1.0f;
    // should be set only once plz
    unsigned int m_width = 0;
    unsigned int m_height = 0;
    SDL_Rect m_viewport = {0,0,0,0};

public:
    inline Camera() {;}
    inline Camera(int width, int height) : m_width(width), m_height(height) {;}
    virtual ~Camera () {}

    int init();
    void destroy();
    void update();
    SDL_Rect transformToWorldCordinates(SDL_Rect rect);
    /* GETTERS */
    inline const int getX() const { return m_x; }
    inline const int getY() const { return m_y; }
    inline const int getWidth() const { return m_width; }
    inline const int getHeight() const { return m_height; }

    inline const float getScale() const { return m_scale; }
    /* SETTERS */
    // move x amout
    inline void moveX(float x) { m_x += x; }
    inline void moveY(float y) { m_y += y; }
    inline void movePos( float x, float y ) { m_x += x; m_y += y; }
    // set to point
    inline void setX(float x) { m_x = x; }
    inline void setY(float y) { m_y = y; }
    inline void setPos( float x, float y ) { m_x = x; m_y = y; }
    // scale
    inline void scale(float scale) { m_scale -= scale; }
    inline void setScale(float scale) { m_scale = scale; }
    inline const SDL_Rect getViewport() {return m_viewport;}
};
