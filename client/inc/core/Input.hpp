#ifndef _INPUT_HPP
#define _INPUT_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>


#include "SDL2/SDL.h"

class Input {
private:
    static std::unordered_map<unsigned int, bool> m_keys;          // is key down map
    static std::unordered_map<unsigned int, bool> m_previousKeys;  // was key previously down map
    static bool wasKeyDown(unsigned int key);                      // this is dedicately used for keypress
    static int m_lastCharacter;
public:
    // public methods. SDL keysym are unsigned ints
    static void pressKey(unsigned int key);
    static void releaseKey(unsigned int key);
    static bool isKeyDown(unsigned int key);
    static bool isKeyPressed(unsigned int key);
    static void update();
    static inline int getchar() {
        if(m_lastCharacter == -1) return m_lastCharacter;
        int tmp = m_lastCharacter;
        m_lastCharacter = -1;
        return tmp;
    }
    static inline void empty() {
        m_keys.empty();
        m_previousKeys.empty();

    }
};

#endif
