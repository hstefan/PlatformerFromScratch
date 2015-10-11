#include <SDL.h>
#include <chrono>
#include <thread>
#include <iostream>

void Update(float dt, bool* quit) {
  SDL_Event evt;
  while (SDL_PollEvent(&evt)) {
    switch (evt.type) {
    case SDL_KEYDOWN:
      *quit = evt.key.keysym.scancode == SDL_SCANCODE_ESCAPE;
      break;
    case SDL_QUIT:
      *quit = true;
      break;
    }
  }
}

void RunGameLoop() {
  using namespace std::chrono;
  using clock = high_resolution_clock;
  using FloatSeconds = duration<float>;

  // target update frequency is 60hz
  const auto period = 1.f / 60.f;
  const auto targetPeriod = duration_cast<clock::duration>(FloatSeconds(period));

  auto ti = clock::now();
  auto dt = clock::duration::zero();
  auto quit = false;
  while (!quit) {
    // update our entities or whatever we may have
    Update(duration_cast<FloatSeconds>(dt).count(), &quit);

    // calculate timing related operations
    const auto tf = clock::now();
    dt = tf - ti;
    const auto idleDuration = targetPeriod - dt;
    // check in case our iteration lags behind
    if (idleDuration.count() > 0)
      std::this_thread::sleep_for(idleDuration);
    ti = tf;
  }
}

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

  RunGameLoop();

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}