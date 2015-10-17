#include "core/ResourceManager.hpp"

map<string, SDL_Texture*>   R::m_textureMap;
map<string, TTF_Font*>      R::m_fontMap;
SDL_Renderer*               R::m_renderer    = nullptr;

void R::destroy() {
    // empty whole texture map
    for(auto& it : m_textureMap) {
        SDL_DestroyTexture(it.second);
        it.second = nullptr;
    }
    m_textureMap.empty();
    // empty whole font map
    for(auto& it : m_fontMap) {
        TTF_CloseFont(it.second);
        it.second = nullptr;
    }
    m_fontMap.empty();
}
SDL_Texture* R::getTexture(string path) {
    if(m_renderer == nullptr) {
        std::cout << "ResourceManager got renderer pointing to nullptr" << std::endl;
        return nullptr;
    }
    auto it = m_textureMap.find(path);
    // if texture is not found in map
    if(it == m_textureMap.end())
    {
        // http://lazyfoo.net/tutorials/SDL/07_texture_loading_and_rendering/index.php
        SDL_Surface* loadSurface = IMG_Load(path.c_str()); // load pixels to surface
        if(loadSurface == nullptr) {
            std::cout << "Unable to load texture:" << path << "\nError: " << std::string(IMG_GetError()) << std::endl;
        } else {

            SDL_Texture* l_texture = SDL_CreateTextureFromSurface(m_renderer, loadSurface);
            if(l_texture == nullptr) {
                std::cout << "Unable to create texture from circle.png" << std::string(SDL_GetError()) << std::endl;
            } else {
                m_textureMap.insert(make_pair(path, l_texture));
            }
            SDL_FreeSurface(loadSurface);
            return l_texture;
        }
    } else return it->second;
    return nullptr;
}
TTF_Font* R::getFont(string path, int x /* = 42 */) {
    auto it = m_fontMap.find(path);
    if(it == m_fontMap.end())
    {
        TTF_Font* l_font = TTF_OpenFont(path.c_str(), x);
        if(l_font == nullptr)
        {
            std::cout << "Could not load OpenSans font. Error: " << std::string(TTF_GetError()) << std::endl;
        } else {
            m_fontMap.insert(make_pair(path, l_font));
        }
        return l_font;
    } else {
        return it->second;
    }
    return nullptr;
}
