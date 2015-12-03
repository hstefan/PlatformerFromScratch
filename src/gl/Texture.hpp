#pragma once

#include "GL/glew.h"
#include "NonCopyable.hpp"

namespace hst {

struct Texture : private NonCopyable {
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