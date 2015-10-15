#include "GUI/GUIInput.hpp"

bool GUIInput::update() {
    // ask for input
    m_text->setX(m_x)->setY(m_y)->setAlign(m_align);
    std::string tmp = "";
    int l_key = Input::getchar();
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
void GUIInput::draw() {
    if(!m_hidden)
    {
        drawBackground(400, 64);
        if(m_text == nullptr) {
            std::cout << "Could not allocate memory for input text" << std::endl;
        }
        else {
            if(m_text->getText().length() != 0){
                m_text->renderText(m_x, m_y, m_align);
            }
        }
    }
}
