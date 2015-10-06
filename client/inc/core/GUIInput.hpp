#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "core/GUIText.hpp"
#include "core/InputManager.hpp"
#include "core/Camera.hpp"

class GUIInput {
private:
  GUIText* m_text                 = nullptr;
  InputManager* m_inputManager  = nullptr;
  int m_x = 0;
  int m_y = 0;
  std::string m_inputText = "";
  int m_maxInputLength = 10;
public:
  inline GUIInput(InputManager* i) : m_inputManager(i) {;}
  inline virtual ~GUIInput() {;}
  void update();
  void draw(SDL_Renderer& renderer, Camera& camera, TTF_Font& font);
  /* GETTERS */
  inline const int& getX() const { return m_x; }
  inline const int& getY() const { return m_y; }
  /* SETTERS */
  inline void setMaxLength(int x) { m_maxInputLength = x; }
};