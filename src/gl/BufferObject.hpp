#pragma once

#include <cassert>
#include "GL/glew.h"
#include "NonCopyable.hpp"

namespace hst {

struct BufferObject : private NonCopyable {
  BufferObject();
  virtual ~BufferObject();
  GLuint GetId() const;
  template<class T, unsigned Sz>
  void UploadData(const std::array<T, Sz>& data);
private:
  GLuint id_;
};

BufferObject::BufferObject()
  : id_(GL_FALSE) {
  glGenBuffers(1, &id_);
  assert(id_ != GL_FALSE && "Unable to create buffer object");
}

BufferObject::~BufferObject() {
  if (id_ != 0)
    glDeleteBuffers(1, &id_);
}

GLuint hst::BufferObject::GetId() const
{
  return id_;
}

template<class T, unsigned Sz>
void BufferObject::UploadData(const std::array<T, Sz>& bufferData) {
  glBindBuffer(GL_ARRAY_BUFFER, id_);
  // For now we'll assume every buffer is GL_STATIC_DRAW
  glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(T), bufferData.data(), GL_STATIC_DRAW);
}

} //namespace hst