#pragma once

#ifndef NDEBUG
#define DEBUG_ONLY(a) a
#else
#define DEBUG_ONLY(a)
#endif

#ifndef NDEBUG
#include <iostream>
#include <utility>
#include <GL/glew.h>
#include <SDL.h>
#endif

namespace hst {
namespace debug {

inline static void Log(const char* message) {
  DEBUG_ONLY(std::cout << message);
}

inline static void CheckSDL() {
  DEBUG_ONLY(
    while (auto error = SDL_GetError()) {
      if (*error == '\0')
        break;
      std::cout << "SDL error: " << error << '\n';
      SDL_ClearError();
    }
  )
}

inline static void CheckOpenGL() {
  DEBUG_ONLY(
    while (auto error = glGetError()) {
      std::cout << "OpenGL error: " << error << '\n';
    }
  )
}

} // namespace debug
} // namespace hst