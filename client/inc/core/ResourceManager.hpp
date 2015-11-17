#ifndef _RESOURCEMANAGER_HPP
#define _RESOURCEMANAGER_HPP

#include <map>
#include <iostream>
#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

using namespace std;

class ResourceManager {
friend class Engine;
private:
    /* private data */
    map<string, SDL_Texture*> m_textureMap;
    map<string, TTF_Font*>    m_fontMap;
    SDL_Renderer* m_renderer = nullptr;
protected:
    /* protected data */
    inline ResourceManager() {;}
    inline ~ResourceManager() {;}
public:
    /* public data */
    inline void init(SDL_Renderer* r) { m_renderer = r; }
    SDL_Texture* getTexture(string path);
    TTF_Font* getFont(string path, int x = 42);
    void destroy();
};
#endif
