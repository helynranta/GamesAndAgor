#include "core/Window.hpp"

SDL_Renderer*   Window::renderer    = nullptr;
SDL_Window*     Window::window      = nullptr;

int Window::init(int width, int height) {
    int success = 1;
    window = SDL_CreateWindow("Agor and Gamus",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, 0);
    // check if window creat>ion failed
    if(window == nullptr) {
        std::cout << "Could not create window " << std::string(SDL_GetError()) << std::endl;
        success = 0;
    }
    // init renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // set clear color
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    // check if renderer init failed
    if(renderer == nullptr) {
        std::cout << "Could not create renderer " << std::string(SDL_GetError()) << std::endl;
        success = 0;
    }
    return success;
}
void Window::destroy() {
    // clear sdl
    SDL_DestroyRenderer(renderer);
    renderer  = nullptr;
    SDL_DestroyWindow(window);
    window    = nullptr;
}
