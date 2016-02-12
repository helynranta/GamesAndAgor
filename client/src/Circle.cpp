#include "Circle.hpp"

#include <iostream>

#include "Engine.hpp"

void Circle::init() {}
void Circle::update() {

	int dt = (SDL_GetTicks() - (m_st - Engine::connection->getPing()))/1000;
	// predict latest position
	m_sx = m_sx + m_dir.x * dt;
	m_sy = m_sy + m_dir.y * dt;
	
	Vector2d* dxy = new Vector2d(m_x-m_sx, m_y-m_sy);
	
	//int length = dxy.length();
	
	m_x = (m_x - (dxy->x/2));
	m_y = (m_y - (dxy->y/2));
	//m_x = m_sx;
	//m_y = m_sy;
    delete dxy;

    m_destRect = {
        int(m_x), int(m_y),        	// make center of circle true center of drawed texture
        int(m_r*2), int(m_r*2) 			// width and height of circle are r*2
    };
}
void Circle::move(int x, int y) {
    m_x += x;
    m_y += y;
}
