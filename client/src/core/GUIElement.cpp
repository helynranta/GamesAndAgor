#include "core/GUIElement.hpp"

void GUIElement::drawBackground () {
    drawBackground(m_width, m_height);
}
void GUIElement::drawBackground (int width) {
    drawBackground(width, m_height);
}
void GUIElement::drawBackground (int width, int height) {
    m_bg_x = m_x;
    m_bg_y = m_y;
    if(m_align == TEXT_ALIGN::CENTER) {
        m_bg_x = m_x - m_width/2;
    }

    SDL_SetRenderDrawColor( m_renderer, 40, 40, 40, 255 );
    const SDL_Rect rect =  {m_bg_x, m_bg_y, width, height/2};
    SDL_RenderFillRect(m_renderer, &rect);
    // Set render color to blue ( rect will be rendered in this color )
    SDL_SetRenderDrawColor(m_renderer, 50, 50, 50, 255);
}
