#version 450 core

in vec3 color_f;
out vec4 frag_color;

void main(){
	frag_color = vec4(color_f, 1.0);
}