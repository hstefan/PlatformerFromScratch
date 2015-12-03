#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using hst::Texture;

Texture::Texture(const char * filename)
  : width_(0), height_(0), comp_(0), texId_(0) {
  LoadTexture(filename);
}

hst::Texture::~Texture() {
  if (texId_ != 0) {
    glDeleteTextures(1, &texId_);
  }
}

GLuint hst::Texture::GetId() const
{
  return texId_;
}

void Texture::LoadTexture(const char* filename) {
  auto buffer = stbi_load(filename, &width_, &height_, &comp_, STBI_rgb);

  if (buffer != nullptr) {
    // generates new texture
    glGenTextures(1, &texId_);
    glBindTexture(GL_TEXTURE_2D, texId_);

    // by default we are using linear filtering for minify and nearest for
    // magnify
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // RGB for 3 components, RGBA in case of 4
    if (comp_ == 3)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    else if (comp_ == 4)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // free local buffer, already uploaded to GPU
    stbi_image_free(buffer);
  }
}