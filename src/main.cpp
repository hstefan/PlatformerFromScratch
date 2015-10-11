#include <SDL.h>
#include <iostream>

int main(int, char*[]) {
  SDL_Init(SDL_INIT_VIDEO);
  
  auto window = SDL_CreateWindow("A Platformer From Scratch",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    1280,
    720,
    SDL_WINDOW_OPENGL);
  
  if (window == nullptr) {
    //TODO: error reporting mechanisms
    std::cout << "Unable to create window " << SDL_GetError() << "\n";
    return 1;
  }

  SDL_Delay(2000);

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}