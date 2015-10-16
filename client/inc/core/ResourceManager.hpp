#ifndef _RESOURCEMANAGER_HPP
#define _RESOURCEMANAGER_HPP

#include <map>
#include <iostream>
#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

using namespace std;

class R {
private:
    static map<string, SDL_Texture*> m_textureMap;
    static map<string, TTF_Font*>    m_fontMap;
    static SDL_Renderer* m_renderer;
public:
    static inline void init(SDL_Renderer* r) { m_renderer = r; }
    static SDL_Texture* getTexture(string path);
    static TTF_Font* getFont(string path, int x = 42);
    static void destroy();
};
#endif
