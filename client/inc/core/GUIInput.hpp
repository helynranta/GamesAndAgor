#pragma once

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "core/GUIElement.hpp"
#include "core/GUIText.hpp"
#include "core/InputManager.hpp"
#include "core/Camera.hpp"

class GUIInput : public GUIElement {
private:
  GUIText* m_text = nullptr;
  InputManager* m_inputManager = nullptr;
  unsigned int m_maxInputLength = 10;
public:
  inline GUIInput(InputManager* i, SDL_Renderer* r, TTF_Font* f) : m_inputManager(i)
  {
      m_text = new GUIText(r, f);
  }
  inline virtual ~GUIInput() { delete m_text; }
  bool update() override;
  bool update(int x, int y);
  void draw(TEXT_ALIGN align = TEXT_ALIGN::LEFT);
  /* GETTERS */
  inline const std::string& getText() const { return m_text->getText(); }
  inline void empty() { m_text->setText(""); }
  inline const int& getWidth() const { return m_text->getWidth(); }
  inline const int& getHeight() const { return m_text->getHeight(); }
  /* SETTERS */
  inline void setMaxLength(unsigned int x) { m_maxInputLength = x; }
};
