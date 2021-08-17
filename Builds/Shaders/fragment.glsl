#version 400

in vec3 output_color;
in vec2 tex_coord;

uniform sampler2D texture;

out vec4 frag_colour;

void main(){
    frag_colour = texture(texture, tex_coord) * vec4(output_color.xyz, 1);
}