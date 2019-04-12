#version 450 core

layout(location = 0) in vec3 pos_v;
layout(location = 1) in vec3 color_v;

out vec3 color_f;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = projection * view * model * vec4(pos_v, 1.0);
	color_f = color_v;
}
