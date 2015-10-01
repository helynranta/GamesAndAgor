#include "Circle.hpp"

#include <iostream>

void Circle::init() {}
void Circle::update() {
    m_destRect = {
        int(m_x), int(m_y),        // make center of circle true center of drawed texture
        int(m_r*2), int(m_r*2) };  // width and height of circle are r*2
    //m_destRect = {m_x, m_y, (int)m_r, (int)m_r};  // tmp rect for testing
}
void Circle::move(int x, int y) {
    m_x += x;
    m_y += y;
}
