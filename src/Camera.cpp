#include "Camera.hpp"

#include <iostream>
#include <string>

int Camera::init() {
    return 0;
}
void Camera::destroy() {}
void Camera::setScale(float scale) {
    m_scale -= scale;
}
SDL_Rect Camera::transformToWorldCordinates(SDL_Rect rect) {
    // flip y axis to move up
    rect.y = -rect.y;
    // make center of screen to be zero
    rect.x += float(m_width)/2.0f;
    rect.y += float(m_height)/2.0f;
    // add scale
    rect = {
        // this line makes center of texture the actual x and y
        int(rect.x-rect.w*0.5f/m_scale), int(rect.y-rect.h*0.5f/m_scale),
        // and this scales texture compared to viewport scale
        int(rect.w / m_scale), int(rect.h / m_scale)
    };
    //add camera position
    rect = {
        int(rect.x+m_x), int(rect.y+m_y),
        int(rect.w), int(rect.h)
    };
    return rect;
}
