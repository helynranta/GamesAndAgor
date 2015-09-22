/*
*   Main Game Class
*
*/

#include "Game.hpp"

Game::Game () {}
Game::~Game () {}
void Game::run() {
    m_gameState = GameState::PLAY;
    update();
}
void Game::update() {
    while(m_gameState == GameState::PLAY) {

    }
}
