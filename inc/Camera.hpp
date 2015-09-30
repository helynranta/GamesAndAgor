#pragma once

#include <SDL2/SDL.h>



class Camera {
private:
    int m_x = 0;
    int m_y = 0;
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
    /* GETTERS */
    inline const int getX() const { return m_x; }
    inline const int getY() const { return m_y; }
    inline const int getW() const { return m_width; }
    inline const int getH() const { return m_height; }
    inline const float getScale() const { return m_scale; }
    /* SETTERS */
    inline void setX(int x) { m_x = x; }
    inline void setY(int y) { m_x = y; }
    void setScale(float scale);
};
