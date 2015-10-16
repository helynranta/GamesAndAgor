#ifndef _GUI_HPP
#define  _GUI_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <SDL2/SDL.h>

#include "GUI/GUIElement.hpp"
#include "GUI/GUIText.hpp"
#include "GUI/GUIInput.hpp"
#include "core/Camera.hpp"

using namespace std;

class GUI {
private:
    SDL_Renderer* m_renderer = nullptr;
    Camera* m_camera = nullptr;
    map<string, GUIText*> m_GUITexts;
    map<string, GUIInput*> m_GUIInputs;
public:
    GUI (SDL_Renderer* r);
    virtual ~GUI ();

    void draw();
    void update();
    inline void addText(string name, GUIText* element) {
        if(element != nullptr) {
            m_GUITexts.insert(make_pair(name, element));
            element->setText(name);
        }
    }
    inline void addInput(string name, GUIInput* element) {
        if(element != nullptr)
            m_GUIInputs.insert(make_pair(name, element));
    }
    inline GUIText* getText(const string& name) {
        auto it = m_GUITexts.find(name);
        if(it != m_GUITexts.end()) {
            return it->second;
        }
        else std::cout << "no element found in gui by name " << name << std::endl;
        return nullptr;
    }
    inline GUIInput* getInput(const string& name) {
        auto it = m_GUIInputs.find(name);
        if(it != m_GUIInputs.end()) {
            return it->second;
        }
        else std::cout << "no element found in gui by name " << name << std::endl;
        return nullptr;
    }
};

#endif
