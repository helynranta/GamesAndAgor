#pragma once

#include <map>
#include <iostream>
#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

using namespace std;

class ResourceManager {
private:
    map<string, SDL_Texture*> m_textureMap;
    map<string, TTF_Font*>    m_fontMap;
    SDL_Renderer* m_renderer = nullptr;
public:
    inline ResourceManager (SDL_Renderer* r) : m_renderer(r) {;}
    virtual ~ResourceManager ();

    SDL_Texture* getTexture(string path);
    TTF_Font* getFont(string path, int x = 42);
};
