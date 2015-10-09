#include "core/GUIInput.hpp"

bool GUIInput::update() {
    // ask for input
    std::string tmp = "";
    int l_key = m_inputManager->getchar();
    if(l_key != -1)
    {
        std::string tmp = SDL_GetKeyName(l_key);
        if(tmp == "Backspace" && m_inputText.length() > 0) {
            m_inputText.pop_back();
        }
        else if (tmp == "Return") {
            return true;
        }
        else if(tmp.length() == 1) {
            if(m_maxInputLength == 0)
                m_inputText += tmp;
            else if(m_inputText.length() < m_maxInputLength)
                m_inputText += tmp;
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
void GUIInput::draw(SDL_Renderer& renderer, Camera& camera) {
    if(m_text == nullptr) {
        std::cout << "Could not allocate memory for input text" << std::endl;
    }
    else if(m_inputText.length() != 0){
        m_text->createTexture(m_inputText);
        m_text->renderText(m_x, m_y);
    }
}
