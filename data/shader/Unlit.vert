#version 150

in vec2 position;
out vec4 Color;

void main() {
  gl_Position = vec4(position, 0.0, 1.0);
  Color = vec4(1.0, 1.0, 1.0, 1.0);
}
