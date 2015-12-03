#pragma once

#include <iostream>
#include <cassert>
#include <vector>
#include "GL/glew.h"
#include "NonCopyable.hpp"

namespace hst {

struct ShaderProgram;

struct ShaderAttrib {
  inline ShaderAttrib(const ShaderProgram* prog, const char* name);
  inline ShaderAttrib(const ShaderProgram* prog, GLint index);
  inline void Enable();
  inline void Pointer(GLuint size, GLenum type,
    GLboolean normalized, GLsizei stride, const GLvoid* pointer);
private:
  const ShaderProgram* const program_;
  GLint index_;
};

struct ShaderProgram : private NonCopyable {
  inline ShaderProgram();
  virtual ~ShaderProgram();
  inline GLuint GetId() const;
  inline void AttachShader(GLuint shader);
  inline void DetachShader(GLuint shader);
  inline void Link();
  inline void Use();
  inline void BindFragDataLocation(unsigned colorNumber, const char* loc);
  inline ShaderAttrib GetAttrib(const char* name) const;
private:
  GLuint id_;
};

ShaderAttrib::ShaderAttrib(const ShaderProgram* prog, const char* name)
  : program_(prog), index_(GL_FALSE) {
  index_ = glGetAttribLocation(program_->GetId(), name);
  assert(index_ != -1 && "Attribute not found");
}

ShaderAttrib::ShaderAttrib(const ShaderProgram* prog, GLint index)
  : program_(prog), index_(index) {
}

void ShaderAttrib::Enable() {
  glEnableVertexAttribArray(index_);
}

void ShaderAttrib::Pointer(GLuint size, GLenum type,
  GLboolean normalized, GLsizei stride, const GLvoid* pointer) {
  glVertexAttribPointer(index_, size, type, normalized, stride, pointer);
}

ShaderProgram::ShaderProgram() {
  id_ = glCreateProgram();
  assert(id_ != GL_FALSE && "Unable to create shader program");
}

ShaderProgram::~ShaderProgram() {
  if (id_ != GL_FALSE) {
    glDeleteProgram(id_);
  }
}

GLuint ShaderProgram::GetId() const {
  return id_;
}

void ShaderProgram::AttachShader(GLuint shader) {
  glAttachShader(id_, shader);
}

void ShaderProgram::DetachShader(GLuint shader) {
  glDetachShader(id_, shader);
}

void ShaderProgram::Link() {
  glLinkProgram(id_);

  auto linked = GL_FALSE;
  glGetProgramiv(id_, GL_LINK_STATUS, &linked);
  if (linked == GL_FALSE) {
    // If unsucessful display log
    GLint logLength;
    glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &logLength);

    std::vector<GLchar> log(logLength);
    glGetProgramInfoLog(id_, logLength, &logLength, log.data());
    // Display log error
    std::cerr << log.data();
  }
}

void ShaderProgram::Use() {
  glUseProgram(id_);
}

void ShaderProgram::BindFragDataLocation(unsigned colorNumber, const char* name) {
  glBindFragDataLocation(id_, colorNumber, name);
}

inline ShaderAttrib ShaderProgram::GetAttrib(const char * name) const {
  return ShaderAttrib(this, name);
}

} // namespace hst
