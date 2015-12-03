#include <SDL.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <array>
#include "GL/glew.h"
#include "gl/Texture.hpp"
#include "gl/Shader.hpp"
#include "gl/ShaderProgram.hpp"
#include "gl/BufferObject.hpp"
#include "gl/VertexArray.hpp"
#include "debug/Debug.hpp"

enum class GameState {
  Running,
  Quit,
};

// just a basic struct for managing our "scene"
struct GameScene {
  GameScene();

  GameState State;
  hst::Shader VertShader;
  hst::Shader FragShader;
  hst::ShaderProgram ShaderProg;
  hst::VertexArray VAO;
  hst::BufferObject VBO;
};

GameScene::GameScene()
  : State(GameState::Running),
    VertShader(hst::ShaderType::Vertex),
    FragShader(hst::ShaderType::Fragment),
    ShaderProg(), VBO(), VAO() {
  VAO.Bind();

  // upload vertex data to our VBO
  std::array<float, 6> vertices {
    -0.0f,  0.5f,
    0.5f,  -0.5f,
    -0.5f, -0.5f,
  };
  VBO.UploadData(vertices);

  // load and compile vertex shader
  VertShader.ContentsFromFile("data/shader/Unlit.vert");
  VertShader.Compile();
  hst::debug::CheckOpenGL();

   // load and compile fragment shader
  FragShader.ContentsFromFile("data/shader/Unlit.frag");
  FragShader.Compile();
  hst::debug::CheckOpenGL();

  // attach shaders to shader program
  ShaderProg.AttachShader(VertShader.GetId());
  ShaderProg.AttachShader(FragShader.GetId());
  hst::debug::CheckOpenGL();

  // bind output location
  ShaderProg.BindFragDataLocation(0, "OutColor");

  // link shader program
  ShaderProg.Link();

  // points the position to the actual data
  auto posAttrib = ShaderProg.GetAttrib("position");
  posAttrib.Pointer(2, GL_FLOAT, GL_FALSE, 0, nullptr);
  posAttrib.Enable();
}

void Update(float dt, GameScene* data) {
  // Handle events and input
  SDL_Event evt;
  while (SDL_PollEvent(&evt)) {
    switch (evt.type) {
    case SDL_KEYDOWN:
      if (evt.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
        data->State = GameState::Quit;
      }
      break;
    case SDL_QUIT:
      data->State = GameState::Quit;
      break;
    }
  }
  data->VAO.Bind();
  data->ShaderProg.Use();
  glDrawArrays(GL_TRIANGLES, 0, 3);
  hst::debug::CheckOpenGL();
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

  GameScene gameData;

  while (gameData.State == GameState::Running) {
    // clear window
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // update our entities or whatever we may have
    Update(duration_cast<FloatSeconds>(dt).count(), &gameData);
    hst::debug::CheckOpenGL();

    //swap buffers
    SDL_GL_SwapWindow(window);

    hst::debug::CheckSDL();
    hst::debug::CheckOpenGL();

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
    SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
  hst::debug::CheckSDL();

  if (*window == nullptr) {
    //TODO: error reporting mechanisms
    std::cout << "Unable to create window " << SDL_GetError() << "\n";
    return false;
  }

  auto context = SDL_GL_CreateContext(*window);

  glewExperimental = GL_TRUE;
  const auto glewCode = glewInit();
  if (glewCode != GLEW_OK) {
    std::cerr << "Unable to initialize glew " << glewGetErrorString(glewCode);
    return false;
  }
  hst::debug::CheckOpenGL();

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