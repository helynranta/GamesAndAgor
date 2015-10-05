#include "core/Camera.hpp"

#include <iostream>
#include <string>

int Camera::init() {
    return 0;
}
void Camera::destroy() {}
SDL_Rect Camera::transformToWorldCordinates(SDL_Rect rect) {
    // flip y axis to move up
    rect.y = -rect.y;
    // make center of screen to be zero
    rect.x += float(m_width)/2.0f;
    rect.y += float(m_height)/2.0f;
    // add scale
    rect = {
        // this line makes center of texture the actual x and y
        int(rect.x - m_x * m_scale), int(rect.y + m_y*m_scale),
        // and this scales texture compared to viewport scale
        int(rect.w), int(rect.h)
    };
    //add camera position
    /*
    rect = {
        int(rect.x-m_x / m_scale), int(rect.y+m_y / m_scale),
        int(rect.w), int(rect.h)
    };
    */
    return rect;
}
void Camera::update() {
  m_viewport = {
      int(m_x - m_width * 0.5f * m_scale), int(m_y - m_height * 0.5f * m_scale),
      int(m_x + m_width * 0.5f * m_scale), int(m_y + m_height * 0.5f * m_scale)
  };
  
}