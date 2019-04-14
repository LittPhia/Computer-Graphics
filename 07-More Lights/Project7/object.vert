#version 450 core
layout(location = 0) in vec3 posV;
layout(location = 1) in vec3 normalV;
layout(location = 2) in vec2 tex_coordsV;

out vec4 normalF;
out vec4 frag_posF;
out vec2 tex_coordsF;

uniform mat4 model;
uniform mat4 norm_matrix;
uniform mat4 view;
uniform mat4 projection;

vec4 normalize4(vec4 a){
	return vec4(normalize(a.xyz), 0.0);
}

void main(){
	frag_posF = model * vec4(posV, 1.0);
	normalF = normalize4(norm_matrix * vec4(normalV, 0.0));
	tex_coordsF = tex_coordsV;

	gl_Position = projection * view * frag_posF;
}
