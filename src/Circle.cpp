#include "Circle.hpp"

#include <iostream>

void Circle::init() {}
void Circle::update() {
    m_destRect = {m_x, m_y, m_r*2, m_r*2};
}
void Circle::move(int x, int y) {
    m_x += x;
    m_y += y;
}
