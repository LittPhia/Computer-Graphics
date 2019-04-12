#version 450 core

layout(location = 0) in vec3 pos_v;
layout(location = 1) in vec3 color_v;
layout(location = 2) in vec2 tex_coord_v;

out vec3 color_f;
out vec2 tex_coord_f;

void main() {
	gl_Position = vec4(pos_v, 1.0);
	color_f = color_v;
	tex_coord_f = tex_coord_v; 
}