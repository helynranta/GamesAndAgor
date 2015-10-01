#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

#include "SDL2/SDL.h"

class InputManager {
private:
    std::unordered_map<unsigned int, bool> m_keys;          // is key down map
    std::unordered_map<unsigned int, bool> m_previousKeys;  // was key previously down map
    bool wasKeyDown(unsigned int key);                      // this is dedicately used for keypress
public:
    inline InputManager () { ; }
    inline virtual ~InputManager () { ; }
    // public methods. SDL keysym are unsigned ints
    void pressKey(unsigned int key);
    void releaseKey(unsigned int key);
    bool isKeyDown(unsigned int key);
    bool isKeyPressed(unsigned int key);
    void update();
};
