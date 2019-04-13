#version 450 core

in vec3 light_color_f;
out vec4 frag_color;
uniform vec3 object_color;

void main(){
	frag_color = vec4(light_color_f * object_color, 1.0);
}