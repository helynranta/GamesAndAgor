#include "core/GUIInput.hpp"



void GUIInput::update() {
  // ask for input
  std::string tmp;
  int l_key = m_inputManager->getchar();
  if(l_key != -1)
  {
      std::string tmp = SDL_GetKeyName(l_key);
      if(tmp == "Backspace" && m_inputText.length() > 0) {
        m_inputText.pop_back();
        
      }
          
      else if(tmp.length() == 1) {
        if(m_maxInputLength == -1)
          m_inputText += tmp;
        else if(m_inputText.length() < m_maxInputLength)
          m_inputText += tmp;
      }
          
  }
}
void GUIInput::draw(SDL_Renderer& renderer, Camera& camera, TTF_Font& font) {
  if(m_inputText.length() != 0){
    m_text->createTexture(m_inputText , *m_renderer, *font);
    m_text->renderText(m_x, m_y, *m_renderer);
  }
}