#include <SDL.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <GL/glew.h>

void CheckSDLError() {
#ifndef NDEBUG
  auto error = SDL_GetError();
  if (*error != '\0')
  {
    std::cout << "SDL error: " << error << "\n";
    SDL_ClearError();
  }
#endif
}

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

void RunGameLoop(SDL_Window* window) {
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
    // clear window
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    CheckSDLError();

    // update our entities or whatever we may have
    Update(duration_cast<FloatSeconds>(dt).count(), &quit);

    //swap buffers
    SDL_GL_SwapWindow(window);
    CheckSDLError();

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

bool InitializeContext(SDL_Window** window) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  *window = SDL_CreateWindow("A Platformer From Scratch",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    1280,
    720,
    SDL_WINDOW_OPENGL);
  CheckSDLError();

  if (window == nullptr) {
    //TODO: error reporting mechanisms
    std::cout << "Unable to create window " << SDL_GetError() << "\n";
    return false;
  }

  auto context = SDL_GL_CreateContext(*window);
  SDL_GL_MakeCurrent(*window, context);

  return true;
}

int main(int, char*[]) {
  SDL_Window* window = nullptr;

  if (InitializeContext(&window)) {
    RunGameLoop(window);
    SDL_GL_DeleteContext(SDL_GL_GetCurrentContext());
    SDL_DestroyWindow(window);
  }

  SDL_Quit();

  return 0;
}