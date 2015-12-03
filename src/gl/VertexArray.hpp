#pragma once

#include <cassert>
#include "NonCopyable.hpp"
#include "GL/glew.h"

namespace hst {

struct VertexArray : private NonCopyable {
  VertexArray();
  virtual ~VertexArray();
  GLuint GetId() const;
  void Bind();
private:
  GLuint id_;
};

VertexArray::VertexArray()
  : id_(GL_INVALID_VALUE) {
  glGenVertexArrays(1, &id_);
  assert(id_ != GL_INVALID_VALUE && "Unable to create vertex array");
}

VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &id_);
}

GLuint VertexArray::GetId() const {
  return id_;
}

void VertexArray::Bind() {
  glBindVertexArray(id_);
}

}