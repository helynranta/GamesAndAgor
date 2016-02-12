#include "Circle.hpp"

#include <iostream>

#include "Engine.hpp"

void Circle::init() {}
void Circle::update() {

	int dt = (SDL_GetTicks() - m_st - Engine::connection->getPing())/1000;
	//int px = m_x + m_dir.x * dt;
	//int py = m_y + m_dir.y * dt;
	//m_x = (m_x - (m_sx));
	//m_x = m_sx;
	//m_y = m_sy;
    
    m_destRect = {
        int(m_sx), int(m_sy),        	// make center of circle true center of drawed texture
        int(m_r*2), int(m_r*2) 			// width and height of circle are r*2
    };
}
void Circle::move(int x, int y) {
    m_x += x;
    m_y += y;
}
