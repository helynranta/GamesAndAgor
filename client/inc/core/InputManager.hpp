#ifndef _INPUT_HPP
#define _INPUT_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>


#include "SDL2/SDL.h"

class InputManager {
friend class Engine;
private:
    /* private data */
    std::unordered_map<unsigned int, bool> m_keys;          // is key down map
    std::unordered_map<unsigned int, bool> m_previousKeys;  // was key previously down map
    bool wasKeyDown(unsigned int key);                      // this is dedicately used for keypress
    int m_lastCharacter = -1;
protected:
    /* protected data */
    inline InputManager() {;}
    inline ~InputManager() {;}
public:
    // public methods. SDL keysym are unsigned ints
    void pressKey(unsigned int key);
    void releaseKey(unsigned int key);
    bool isKeyDown(unsigned int key);
    bool isKeyPressed(unsigned int key);
    void update();
    inline int getchar() {
        if(m_lastCharacter == -1) return m_lastCharacter;
        int tmp = m_lastCharacter;
        m_lastCharacter = -1;
        return tmp;
    }
    inline void empty() {
        m_keys.empty();
        m_previousKeys.empty();

    }
};

#endif
