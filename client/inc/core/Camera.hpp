#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include <SDL2/SDL.h>

class Camera {
friend class Engine;
private:
    /* private data */
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_scale = 1.0f;
    // should be set only once plz
    unsigned int m_width = 0;
    unsigned int m_height = 0;
    SDL_Rect m_viewport = {0,0,0,0};
protected:
    /* protected data */
    inline Camera() {;}
    inline ~Camera() {;}
public:
    /* public data */
    inline int init(unsigned int width, unsigned int height) {
        m_width = width;
        m_height = height;
        return true;
    }
    void destroy();
    void update();
    SDL_Rect transformToWorldCordinates(SDL_Rect rect);
    /* GETTERS */
    inline float getX()  { return m_x; }
    inline float getY()  { return m_y; }
    inline float getWidth()  { return m_width; }
    inline float getHeight()  { return m_height; }
    inline float getScale()  { return m_scale; }
    // camera viewport
    inline const SDL_Rect getViewport()  {return m_viewport;}
    /* SETTERS */
    // move x amout
    inline void moveX(float x) { m_x += x; }
    inline void moveY(float y) { m_y += y; }
    inline void movePos( float x, float y ) { m_x += x; m_y += y; }
    // set to camera to point
    inline void setX(float x) { m_x = x; }
    inline void setY(float y) { m_y = y; }
    inline void setPos( float x, float y ) { m_x = x; m_y = y; }
    // set camera zoom scale
    inline void scale(float scale) { m_scale -= scale/10.0f; } // divide by 10, so we dont use numbers like 0.001f
    inline void setScale(float scale) { m_scale = scale; }
};
#endif
