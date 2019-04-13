#version 450 core
layout(location = 0) in vec3 pos_v;
layout(location = 1) in vec3 normal_v;

out vec3 light_color_f;

uniform vec3 viewer_pos;
uniform vec3 light_pos;
uniform vec3 light_color;

uniform mat4 model;
uniform mat3 norm_matrix;
uniform mat4 view;
uniform mat4 projection;

void main() {
	vec3 vertex_norm = normalize(norm_matrix * normal_v);	
    float ambient_strength = 0.1;
    float specular_strength = 0.5;
    

    vec3 ambient = ambient_strength * light_color;
    
    vec3 light_dir = normalize(light_pos - pos_v);
    float diffuse_cos = max(dot(vertex_norm, light_dir), 0.0);
    vec3 diffuse = diffuse_cos * light_color;

    vec3 viewer_dir = normalize(viewer_pos - pos_v);
    vec3 reflect_dir = vec3(0.0);
    float shine_strength = 0.0;
    if (dot(light_dir, vertex_norm) > 0.0) {
        reflect_dir = reflect(-light_dir, vertex_norm);
        shine_strength = pow(max(dot(viewer_dir, reflect_dir), 0.0), 32);
    }
    vec3 specular = shine_strength * specular_strength * light_color;

    light_color_f = ambient + diffuse + specular;
	

	gl_Position = projection * view * vec4(pos_v, 1.0);
}
