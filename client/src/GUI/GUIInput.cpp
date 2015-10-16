#include "GUI/GUIInput.hpp"

bool GUIInput::update() {
    // ask for input
    std::string tmp = "";
    int l_key = Input::getchar();
    if(l_key != -1)
    {
        std::string tmp = SDL_GetKeyName(l_key);
        if(tmp == "Backspace" && m_text.length() > 0) {
            m_text.pop_back();
        }
        else if (tmp == "Return") {
            return true;
        }
        else if(tmp.length() == 1) {
            if(m_maxInputLength == 0)
                m_text = m_text+tmp;
            else if(m_text.length() < m_maxInputLength)
                m_text = m_text+tmp;
        }
    }
    return false;
}
void GUIInput::draw() {
    if(!m_hidden)
    {
        if(drawBG)
            drawBackground(800, m_bg_height);
        cursorVisible = floor(sin(float(SDL_GetTicks())/300.0f)+1);
        if(m_text.length() != 0 || cursorVisible){
            if(cursorEnabled && cursorVisible)
                drawCursor();
            if(m_text.length() > 0)
                renderText(m_x, m_y);
            else
                m_width = 0;
        }
    }
}
void GUIInput::drawCursor() {
    SDL_SetRenderDrawColor(m_renderer, cursorColor.r, cursorColor.g, cursorColor.b, cursorColor.a);
    const SDL_Rect cursor = {
        int(m_x+m_width*m_scale), m_y,
        int(m_bg_height*m_scale*0.75), int(m_bg_height*m_scale)
    };
    SDL_RenderFillRect(m_renderer, &cursor);
    SDL_SetRenderDrawColor(m_renderer, 50, 50, 50, 255);
}
