#version 400

layout (location=0) in vec3 vp; // vertice position
layout (location=1) in vec3 vc; // vertice color
out vec3 output_color;

void main() {
  gl_Position = vec4(vp, 1.0);
  output_color = vc;
}