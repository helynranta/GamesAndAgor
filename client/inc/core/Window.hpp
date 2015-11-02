#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <iostream>

#include <SDL2/SDL.h>

class Window {
friend class Engine;
private:
    /* private data */
    SDL_Renderer* renderer = nullptr;
    SDL_Window* window = nullptr;
protected:
    /* protected data */
    inline Window() {;}
    inline ~Window() {;}
public:
    /* public data */
    int init(int width, int height);
    SDL_Renderer* getRenderer() { return renderer; }
    void destroy();
};
#endif
