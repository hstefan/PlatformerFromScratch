#include "Shader.hpp"
#include <fstream>
#include <cassert>
#include <iostream>

using hst::Shader;
using hst::ShaderType;

static void PrintShaderInfoLog(GLuint id) {
  char buffer[512];
  glGetShaderInfoLog(id, 512, nullptr, buffer);
  std::cerr << buffer;
}
void Shader::ContentsFromFile(std::string filename) {
  std::ifstream file(filename);
  assert(file && "Shader source file not found");
  if (file)
    ContentsFromStream(file);
}

void Shader::ContentsFromStream(std::istream& stream) {
  std::string contentStr(std::istreambuf_iterator<char>(stream), {});
  ContentsFromStr(contentStr);
}

bool Shader::Compile() {
  assert(contentVersion_ != 0 && "Attempt to compile shader with not content");
  if (compiledVersion_ != contentVersion_) {
    // Attemps to compile shader
    glCompileShader(id_);

    // Check whether compilation was sucessful
    GLint status;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
      PrintShaderInfoLog(id_);
      return false;
    }

    compiledVersion_ = contentVersion_;
  }
  return true;
}