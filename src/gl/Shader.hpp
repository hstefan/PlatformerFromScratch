#pragma once

#include <string>
#include <istream>
#include "GL/glew.h"
#include "NonCopyable.hpp"
#include "debug/Debug.hpp"

namespace hst {

enum class ShaderType : GLenum {
  Fragment = GL_FRAGMENT_SHADER,
  Vertex = GL_VERTEX_SHADER,
  Geometry = GL_GEOMETRY_SHADER,
};

struct Shader : private NonCopyable {
  inline Shader(ShaderType type);
  inline virtual ~Shader();
  void ContentsFromFile(std::string filename);
  inline void ContentsFromStr(std::string content);
  void ContentsFromStream(std::istream& stream);
  bool Compile();
  inline GLuint GetId() const;
private:
  GLuint id_;
  unsigned compiledVersion_;
  unsigned contentVersion_;
};

Shader::Shader(ShaderType type)
  : id_(0), contentVersion_(0), compiledVersion_(0) {
  id_ = glCreateShader(static_cast<GLenum>(type));
}

Shader::~Shader() {
  if (id_ != 0)
    glDeleteShader(id_);
}

GLuint hst::Shader::GetId() const {
  return id_;
}

void Shader::ContentsFromStr(std::string content) {
  auto rawStr = content.c_str();
  glShaderSource(id_, 1, &rawStr, nullptr);
  ++contentVersion_;
}

} //namespace hst