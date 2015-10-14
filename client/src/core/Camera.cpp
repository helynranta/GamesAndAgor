#include "core/Camera.hpp"

#include <iostream>
#include <string>

float Camera::m_x = 0;
float Camera::m_y = 0;
float Camera::m_scale = 1.0f;
// should be set only once plz
unsigned int Camera::m_width = 0;
unsigned int Camera::m_height = 0;
SDL_Rect Camera::m_viewport = {0,0,0,0};

void Camera::destroy() {}
SDL_Rect Camera::transformToWorldCordinates(SDL_Rect rect) {
    // flip Y axis
    rect.y = -rect.y;
    // apply scale to texture
    rect = {
        int(rect.x / m_scale), int(rect.y / m_scale),
        int(rect.w / m_scale), int(rect.h / m_scale)
    };
    // move (0,0) to be the center of the screen. Add camera position
    rect.x +=  m_width/2 - m_x/m_scale;
    rect.y +=  m_height/2 + m_y/m_scale;
    // make center of the texture real center
    rect.x -= rect.w/2;
    rect.y -= rect.h/2;
    return rect;
}
void Camera::update() {
  // set viewport (x,y) top left corner and (w,h) bottom right
  m_viewport = {
      int(m_x + (m_width * 0.5f * m_scale)), int(m_y + (m_height * 0.5f * m_scale)),
      int(m_x - (m_width * 0.5f * m_scale)), int(m_y - (m_height * 0.5f * m_scale))
  };
  // restrct scale
  if(m_scale < 0.01)
    m_scale = 0.01;
}
