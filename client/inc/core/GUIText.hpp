#pragma once

#include <iostream>
#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

class GUIText {
private:
    SDL_Texture* m_texture = nullptr;
    int m_width = 0.0f;
    int m_height = 0.0f;
    std::string m_text = "";
    SDL_Color m_color;
    bool m_shouldUpdate = false;
    float m_scale = 0.5f;
public:
    inline GUIText () {;}
    inline ~GUIText () { free(); }
    void createTexture(const std::string& text, SDL_Renderer& renderer, TTF_Font& font);
    void renderText(int x, int y, const std::string& text, SDL_Renderer& renderer, TTF_Font& font);
    void renderText(int x, int y, SDL_Renderer& renderer);
    void free();
    /* GETTERS */
    inline const int getWidth() const { return m_width; }
    inline const int getHeight() const { return m_height; }
    inline const float getScale() const { return m_scale; }
    inline const SDL_Texture* getTexture() const { return m_texture; }
    /* SETTERS */
    inline void setColor(SDL_Color color) { m_color = color; m_shouldUpdate = true;}
    inline void setScale(float scale) { m_scale = scale; }
};
