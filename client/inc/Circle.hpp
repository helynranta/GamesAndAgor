#ifndef _CIRCLE_HPP
#define _CIRCLE_HPP

#include <string>

#include "SDL2/SDL.h"
#include "core/Vector2d.hpp"
#include "core/Camera.hpp"

using namespace std;

struct Color {
    int r = 0;
    int g = 0;
    int b = 0;
};

class Circle {
protected:
    // local 
    float m_x = 100;
    float m_y = 100;
    float m_r = 40;
    // server given attributes
    float m_sx = 0;
    float m_sy = 0;
    Vector2d m_dir;
    float m_st = 0;
    float m_sr = 100;
    string nick = ""; 

    SDL_Rect m_destRect = {int(m_x), int(m_y), int(m_r*2), int(m_r*2)};
    Color color;

    bool isStatic = true;
public:
    inline Circle (const string& n, bool s = true) : nick(n), isStatic(s) {;}
    inline Circle (int x, int y, int r) : m_x(x), m_y(y), m_r(r) {}
    virtual ~Circle () {;}

    void init();
    void update();
    void move(int x, int y); // for testing
    /* GETTERS */
    inline int getX () const { return m_x; }
    inline int getY () const { return m_y; }
    inline int getR () const { return m_r; }

    inline Vector2d getDir() const { return m_dir; }
    inline const string& getNick() const { return nick; }

    inline void setPos (int x, int y, float t) { m_sx = x; m_sy = y; m_st = t;}
    inline void setDir (int x, int y) {m_dir.x = x; m_dir.y = y;}
    inline void setR (int r) { m_r = r; }
    
    inline const SDL_Rect& getDestRect() const { return m_destRect; }

    inline void scale(float x) {
        m_r += x;
        if(m_r < 10) m_r = 10;
    }
    inline const Color& getColor() const {return color;}
};

#endif
