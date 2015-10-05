#include "core/GUIText.hpp"

void GUIText::renderText(int x, int y, const std::string& text, SDL_Renderer& renderer, TTF_Font& font) {
    // rebuild texture if needed
    if(m_text != text || m_texture == nullptr || m_shouldUpdate)
    {
        m_text = text;
        free();
        SDL_Surface* surface = TTF_RenderText_Solid(&font, m_text.c_str(), m_color);
        if( surface == nullptr) {
                std::cout << "Unable to render text surface. Error: " << std::string(TTF_GetError()) << std::endl;
        } else {
            m_texture = SDL_CreateTextureFromSurface(&renderer, surface);
            if ( m_texture == nullptr ) {
                std::cout << "Unable to create texture from rendered text. Error: " << std::string(SDL_GetError()) << std::endl;
            } else {
                m_width = surface->w;
                m_height = surface->h;
                m_shouldUpdate = false;
            }
            SDL_FreeSurface(surface);
        }
    }
    // draw to renderer
    if(m_texture != nullptr) {
        SDL_Rect renderQuad = {x, y, int(m_width*m_scale), int(m_height*m_scale)};
        SDL_RenderCopy(&renderer, m_texture, NULL, &renderQuad);
    }
}
void GUIText::free() {
    if(m_texture != nullptr) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
        m_width = 0;
        m_height = 0;
    }
}
