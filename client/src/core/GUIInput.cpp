#include "core/GUIInput.hpp"

bool GUIInput::update() {
    // ask for input
    std::string tmp = "";
    int l_key = m_inputManager->getchar();
    if(l_key != -1)
    {
        std::string l_text = m_text->getText();
        std::string tmp = SDL_GetKeyName(l_key);
        if(tmp == "Backspace" && l_text.length() > 0) {
            l_text.pop_back();
            m_text->setText(l_text);
        }
        else if (tmp == "Return") {
            return true;
        }
        else if(tmp.length() == 1) {
            if(m_maxInputLength == 0)
                m_text->setText(l_text+tmp);
            else if(l_text.length() < m_maxInputLength)
                m_text->setText(l_text+tmp);
        }
    }
    return false;
}
bool GUIInput::update(int x, int y) {
    m_x = x;
    m_y = y;
    if(update()) {
        return true;
    }
    return false;
}
void GUIInput::draw(TEXT_ALIGN align /*= TEXT_ALIGN::LEFT*/) {
    m_align = align;
    drawBackground(400, 64);
    if(m_text == nullptr) {
        std::cout << "Could not allocate memory for input text" << std::endl;
    }
    else {
        if(m_text->getText().length() != 0){
            m_text->renderText(m_x, m_y, align);
        }
    }
}
