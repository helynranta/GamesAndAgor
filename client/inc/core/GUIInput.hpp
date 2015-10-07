#pragma once

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "core/GUIText.hpp"
#include "core/InputManager.hpp"
#include "core/Camera.hpp"

class GUIInput {
private:
  GUIText* m_text = new GUIText();
  InputManager* m_inputManager = nullptr;
  int m_x = 0;
  int m_y = 0;
  std::string m_inputText = "";
  unsigned int m_maxInputLength = 10;
public:
  inline GUIInput(InputManager* i) : m_inputManager(i) {;}
  inline virtual ~GUIInput() { delete m_text; }
  bool update();
  bool update(int x, int y);
  void draw(SDL_Renderer& renderer, Camera& camera, TTF_Font& font);
  /* GETTERS */
  inline const int& getX() const { return m_x; }
  inline const int& getY() const { return m_y; }
  inline const int& getWidth() const { return m_text->getWidth(); }
  inline const int& getHeight() const { return m_text->getHeight(); }
  inline const std::string& getText() const { return m_inputText; }
  inline void empty() { m_inputText = ""; }
  /* SETTERS */
  inline void setMaxLength(unsigned int x) { m_maxInputLength = x; }
};
