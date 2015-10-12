#pragma once

#include "SDL2/SDL.h"

enum TEXT_ALIGN { LEFT, RIGHT, CENTER, CENTER_XY };

class GUIElement {
protected:
    int m_x = 0;
    int m_y = 0;
    int m_bg_x = 0;
    int m_bg_y = 0;
    int m_width = 0.0f;
    int m_height = 0.0f;
    SDL_Renderer* m_renderer = nullptr;
    TEXT_ALIGN m_align = TEXT_ALIGN::LEFT;
public:
    inline GUIElement (SDL_Renderer* r) : m_renderer(r) {}
    inline virtual ~GUIElement () {}
    virtual bool update() = 0;
    // GETTERS
    inline const int& getX() const { return m_x; }
    inline const int& getY() const { return m_y; }
    void drawBackground ();
    void drawBackground (int width);
    void drawBackground (int width, int height);
};
