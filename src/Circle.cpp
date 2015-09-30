#include "Circle.hpp"

#include <iostream>

void Circle::init() {}
void Circle::update(Camera camera) {
    m_destRect = {
        m_x + camera.getX() + camera.getW()/2 - (int)m_r,
        m_y + camera.getY() + camera.getH()/2 - (int)m_r,
        (int)m_r*2 * camera.getScale(),
        (int)m_r*2 * camera.getScale()
    };
}
void Circle::move(int x, int y) {
    m_x += x;
    m_y += y;
}
