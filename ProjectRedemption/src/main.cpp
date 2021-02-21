#include <SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <iostream>
#include <stdlib.h>

const auto SCREEN_WIDTH = 800;
const auto SCREEN_HEIGHT = 600;

static SDL_Window* window;
static SDL_Surface* screen;
static bool running;
static SDL_Event evt;

static void Shutdown();

static void MainLoop()
{
    while (SDL_PollEvent(&evt))
    {
        if (evt.type == SDL_QUIT)
        {
#ifdef __EMSCRIPTEN__
            Shutdown();
            break;
#else
            running = false;
#endif
        }
    }

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xFF, 0x00, 0xFF)); // screen.fill
    SDL_UpdateWindowSurface(window); // display.flip
}

static void Shutdown()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Program Terminated." << std::endl;
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: %s\n" << SDL_GetError() << std::endl;
        return -1;
    }

    window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    screen = SDL_GetWindowSurface(window);
    running = true;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(MainLoop, 0, true);
#else
    while (running)
    {
        MainLoop();
        SDL_Delay(16);
    }

    Shutdown();
#endif

    return 0;
}