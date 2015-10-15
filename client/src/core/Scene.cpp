#include "core/Scene.hpp"

#include "Engine.hpp"

Scene::Scene() {
    gui = new GUI(Window::getRenderer());
}
Scene::~Scene() {
    delete gui;
}
