#include "core/Scene.hpp"

#include "Engine.hpp"

Scene::Scene() {
    gui = new GUI(Engine::window->getRenderer());
}
Scene::~Scene() {
    delete gui;
}
