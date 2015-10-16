#pragma once

#include "SDL2/SDL.h"

enum TEXT_ALIGN { LEFT, RIGHT, CENTER, CENTER_XY, OVER, OVER_RIGHT };

class GUIElement {
protected:
    int m_x = 0;
    int m_y = 0;
    int m_bg_x = 0;
    int m_bg_y = 0;
    int m_width = 0.0f;
    int m_height = 0.0f;
    float m_scale = 0.5f;
    SDL_Renderer* m_renderer = nullptr;
    TEXT_ALIGN m_align = TEXT_ALIGN::LEFT;
    bool m_hidden = false;
public:
    inline GUIElement (SDL_Renderer* r) : m_renderer(r) {}
    inline virtual ~GUIElement () {}
    inline virtual bool update() = 0;
    virtual void draw() = 0;
    // GETTERS
    inline const int& getX() const { return m_x; }
    inline const int& getY() const { return m_y; }
    inline const int& getWidth() const { return m_width; }
    inline const int& getHeight() const { return m_height; }
    inline const float& getScale() const { return m_scale; }
    void drawBackground ();
    void drawBackground (int width);
    void drawBackground (int width, int height);
    inline const bool isHidden() const { return m_hidden; }
    /* SETTERS */
    inline GUIElement* setX ( int x ) {
        m_x = x;
        return this;
    }
    inline GUIElement* setY ( int y ) {
        m_y = y;
        return this;
    }
    inline GUIElement* setPos(int x, int y) {
        m_x = x;
        m_y = y;
        return this;
    }
    inline GUIElement* setAlign( TEXT_ALIGN a ) {
        m_align = a;
        return this;
    }
    inline GUIElement* setScale( float s ) {
        m_scale = s;
        return this;
    }
    inline void setHidden(bool h) { m_hidden = h; };
    inline GUIElement* hide() {
        m_hidden = true;
        return this;
    }
    inline GUIElement* show() {
        m_hidden = false;
        return this;
    }
};
