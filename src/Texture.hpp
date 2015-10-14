#pragma once

#include "GL/glew.h"

namespace hst {
struct Texture {
  Texture(const char* filename);
  virtual ~Texture();

  GLuint GetId() const;
protected:
  void LoadTexture(const char* filename);
private:
  int width_;
  int height_;
  int comp_;
  GLuint texId_;
};
} //namespace hst