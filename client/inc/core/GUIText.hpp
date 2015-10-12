#pragma once

#include <iostream>
#include <string>

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "core/GUIElement.hpp"

class GUIText : public GUIElement {
private:
    SDL_Texture* m_texture = nullptr;
    TTF_Font* m_font = nullptr;
    std::string m_text = "";
    SDL_Color m_color = {200, 200, 200};
    bool m_shouldUpdate = false;
    float m_scale = 0.5f;
public:
    inline GUIText (SDL_Renderer* r, TTF_Font* f) : GUIElement(r), m_font(f) {;}
    inline ~GUIText () { free(); }
    void createTexture(const std::string& text);
    void renderText(int x, int y, const std::string& text, TEXT_ALIGN text_align = TEXT_ALIGN::LEFT);
    void renderText(int x, int y, TEXT_ALIGN text_align = TEXT_ALIGN::LEFT);
    void free();
    inline bool update() { return true; }
    /* GETTERS */
    inline const int& getWidth() const { return m_width; }
    inline const int& getHeight() const { return m_height; }
    inline const float& getScale() const { return m_scale; }
    inline const SDL_Texture* getTexture() const { return m_texture; }
    inline const std::string& getText() const { return m_text; }
    /* SETTERS */
    inline void setColor(SDL_Color color) { m_color = color; m_shouldUpdate = true;}
    inline void setColor(Uint8 r, Uint8 g, Uint8 b) {
        m_color = { r, g, b };
    }
    inline void setScale(float scale) { m_scale = scale; }
    inline void setText(std::string text) { m_text = text; m_shouldUpdate = true;}
};
