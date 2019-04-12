#version 450 core

in vec3 color_f;
in vec2 tex_coord_f;

out vec4 frag_color;

uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform float mix_value;

void main() {
	frag_color = mix(texture(texture_1, tex_coord_f),
					texture(texture_2, vec2(1.0 - tex_coord_f.x, tex_coord_f.y)),
					mix_value) * vec4(color_f, 1.0);
}
