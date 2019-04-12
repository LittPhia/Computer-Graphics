#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec4 vertex_color;

uniform float Offset_X;

void main() {
	gl_Position = vec4(aPos.x + Offset_X, aPos.y, aPos.z, 1.0);
	vertex_color = vec4(aPos, 1.0);
}