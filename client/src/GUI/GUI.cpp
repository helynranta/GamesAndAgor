#include "GUI/GUI.hpp"

GUI::GUI (SDL_Renderer* r) : m_renderer(r) {
    //if(m_renderer == nullptr)
    //    std::cout << "GUI was initialized with renderer pointer leading to NULL" << std::endl;
    //if(m_camera == nullptr)
    //    std::cout << "GUI was initialized with camera pointer leading to NULL" << std::endl;
}
GUI::~GUI () {
    empty();
}
void GUI::draw() {
    for ( auto& it : m_GUITexts) {
        it.second->draw();
    }
    for ( auto& it : m_GUIInputs) {
        it.second->draw();
    }
}
void GUI::update() {
    for ( auto& it : m_GUIInputs) {
        it.second->update();
    }
    for ( auto& it : m_GUITexts) {
        it.second->update();
    }
}
void GUI::empty() {
    for (auto it : m_GUITexts )
    {
        if(it.second != nullptr) delete it.second;
    }
    m_GUITexts.clear();
    for (auto it : m_GUIInputs )
    {
        if(it.second != nullptr) delete it.second;
    }
    m_GUIInputs.clear();
}
