#version 450 core

in vec3 color_f;
in vec2 tex_coord_f;

out vec4 frag_color;

uniform sampler2D texture_1;

void main(){
	frag_color = texture(texture_1, tex_coord_f) * vec4(color_f, 1.0);
}