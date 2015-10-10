#include "core/GUIText.hpp"

void GUIText::createTexture(const std::string& text) {
    // rebuild texture if needed
    if(m_renderer == nullptr) {
        std::cout << "Renderer not defined in GUIText" << std::endl;
        std::cout << "intention: createTexture("<< text <<")" << std::endl;
        return;
    }
    if (m_font == nullptr) {
        std::cout << "Font not defined in GUIText" << std::endl;
        std::cout << "intention: createTexture("<< text <<")" << std::endl;
        return;
    }
    if(text.length() == 0) return;
    if(m_text != text || m_texture == nullptr || m_shouldUpdate)
    {
        m_text = text;
        free();
        SDL_Surface* surface = TTF_RenderText_Solid(m_font, m_text.c_str(), m_color);
        if( surface == nullptr) {
                std::cout << "Unable to render text surface. Error: " << std::string(TTF_GetError()) << std::endl;
        } else {
            m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
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
}
void GUIText::renderText(int x, int y, const std::string& text, TEXT_ALIGN text_align /*= TEXT_ALIGN::LEFT*/) {
    createTexture(text);
    renderText(x, y, text_align);
}
void GUIText::renderText(int x, int y, TEXT_ALIGN text_align /*= TEXT_ALIGN::LEFT*/) {
    createTexture(m_text);
    m_x = x;
    m_y = y;
    if(text_align == TEXT_ALIGN::CENTER) {
        m_x -= m_width/4;
    } else if(text_align == TEXT_ALIGN::CENTER_XY) {
        m_x -= m_width/4;
        m_y -= m_height/4;
    } else if(text_align == TEXT_ALIGN::RIGHT) {
        m_x -= m_width/2;
    }
    if(m_texture != nullptr) {
        SDL_Rect renderQuad = {m_x, m_y, int(m_width*m_scale), int(m_height*m_scale)};
        SDL_RenderCopy(m_renderer, m_texture, NULL, &renderQuad);
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
