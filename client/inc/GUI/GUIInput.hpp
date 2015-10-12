#pragma once

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "core/InputManager.hpp"
#include "core/Camera.hpp"

#include "GUI/GUIElement.hpp"
#include "GUI/GUIText.hpp"

class GUIInput : public GUIElement {
private:
  GUIText* m_text = nullptr;
  InputManager* m_inputManager = nullptr;
  unsigned int m_maxInputLength = 10;
public:
  inline GUIInput(InputManager* i, SDL_Renderer* r, TTF_Font* f) : GUIElement(r), m_inputManager(i)
  {
      m_text = new GUIText(r, f);
  }
  inline virtual ~GUIInput() { delete m_text; }
  bool update();
  void draw();
  /* GETTERS */
  inline const std::string& getText() const { return m_text->getText(); }
  inline void empty() { m_text->setText(""); }
  inline const int& getWidth() const { return m_text->getWidth(); }
  inline const int& getHeight() const { return m_text->getHeight(); }
  /* SETTERS */
  inline void setMaxLength(unsigned int x) { m_maxInputLength = x;}
};
