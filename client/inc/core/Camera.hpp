#pragma once

#include <SDL2/SDL.h>

class Camera {
private:
    static float m_x;
    static float m_y;
    static float m_scale;
    // should be set only once plz
    static unsigned int m_width;
    static unsigned int m_height;
    static SDL_Rect m_viewport;

public:
    static inline int init(int width, int height) {
        m_width = width;
        m_height = height;
        return true;
    }
    static void destroy();
    static void update();
    static SDL_Rect transformToWorldCordinates(SDL_Rect rect);
    /* GETTERS */
    static inline const int getX()  { return m_x; }
    static inline const int getY()  { return m_y; }
    static inline const int getWidth()  { return m_width; }
    static inline const int getHeight()  { return m_height; }
    static inline const float getScale()  { return m_scale; }
    // camera viewport
    static inline const SDL_Rect getViewport()  {return m_viewport;}
    /* SETTERS */
    // move x amout
    static inline void moveX(float x) { m_x += x; }
    static inline void moveY(float y) { m_y += y; }
    static inline void movePos( float x, float y ) { m_x += x; m_y += y; }
    // set to camera to point
    static inline void setX(float x) { m_x = x; }
    static inline void setY(float y) { m_y = y; }
    static inline void setPos( float x, float y ) { m_x = x; m_y = y; }
    // set camera zoom scale
    static inline void scale(float scale) { m_scale -= scale/10.0f; } // divide by 10, so we dont use numbers like 0.001f
    static inline void setScale(float scale) { m_scale = scale; }
};
