#version 400

layout (location=0) in vec3 vp; // vertice position
layout (location=1) in vec3 vc; // vertice color
layout (location=2) in vec2 st; // vertice st

out vec3 output_color;
out vec2 tex_coord;

void main() {
  gl_Position = vec4(vp, 1.0);
  output_color = vc;
  tex_coord = st;
}