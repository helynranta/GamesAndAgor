#pragma once

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "core/Input.hpp"
#include "core/Camera.hpp"

#include "GUI/GUIElement.hpp"
#include "GUI/GUIText.hpp"

class GUIInput : public GUIText {
protected:
  unsigned int m_maxInputLength = 10;
  bool cursorEnabled = true;
  bool cursorVisible = true;
  bool drawBG = true;
  std::string cursorChar = "[]";
  int m_bg_height = 64;
  SDL_Color cursorColor = {100,100,100,255};
public:
  inline GUIInput(SDL_Renderer* r, TTF_Font* f) : GUIText(r,f)
  {
      m_font = f;
  }
  inline virtual ~GUIInput() { ; }
  bool update() override;
  void draw() override;
  void drawCursor();
  /* GETTERS */
  /* SETTERS */
  inline void setCursorVisibility(bool b) { cursorEnabled = b; }
  inline void setMaxLength(unsigned int x) { m_maxInputLength = x;}
};
