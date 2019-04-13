#version 450 core
layout(location = 0) in vec3 pos_v;
layout(location = 1) in vec3 normal_v;

out vec3 normal_f;
out vec3 frag_pos_f;

uniform mat4 model;
uniform mat3 norm_matrix;
uniform mat4 view;
uniform mat4 projection;

void main(){
	frag_pos_f = vec3(model * vec4(pos_v, 1.0));
	normal_f = norm_matrix * normal_v;

	gl_Position = projection * view * vec4(frag_pos_f, 1.0);
}
