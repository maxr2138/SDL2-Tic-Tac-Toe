#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class RenderWindow {
    public:
        RenderWindow(const char* title, const int width, const int height);

        bool CreateWindow (const char* title, const int width, const int height); 

        SDL_Window* getWindow() const;


    
    
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;

    


};


