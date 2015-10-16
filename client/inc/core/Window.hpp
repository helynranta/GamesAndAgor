#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <iostream>

#include <SDL2/SDL.h>

class Window {
private:
    // Camera now owns the renderer
    static SDL_Renderer* renderer;
    static SDL_Window* window;
public:
    static int init(int width, int height);
    static SDL_Renderer* getRenderer() { return renderer; }
    static void destroy();
};
#endif
