#version 400

in vec3 output_color;
out vec4 frag_colour;

void main(){
    frag_colour = vec4(output_color.xyz, 1.0);
}