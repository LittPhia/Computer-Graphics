#version 450 core
layout(location = 0) in vec3 posV;
layout(location = 1) in vec3 normalV;

out vec4 normalF;
out vec4 frag_posF;

uniform mat4 model;
uniform mat4 norm_matrix;
uniform mat4 view;
uniform mat4 projection;

void main(){
	frag_posF = model * vec4(posV, 1.0);
	normalF = norm_matrix * vec4(normalV, 0.0);

	gl_Position = projection * view * model * frag_posF;
}
