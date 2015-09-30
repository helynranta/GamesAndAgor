#include "Camera.hpp"

#include <iostream>
#include <string>

int Camera::init() {
    return 0;
}
void Camera::destroy() {

}
void Camera::setScale(float scale) {
    m_scale -= scale;
    if (m_scale <= 0.001) m_scale = 0.001;
}
