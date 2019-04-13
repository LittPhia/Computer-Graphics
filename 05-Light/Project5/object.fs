#version 450 core

in vec3 normal_f;
in vec3 frag_pos_f;

out vec4 frag_color;

uniform vec3 viewer_pos;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 object_color;


void main(){
    vec3 frag_norm = normalize(normal_f);
    float ambient_strength = 0.1;
    float specular_strength = 0.4;
    
    vec3 ambient = ambient_strength * light_color;
    
    vec3 light_dir = normalize(light_pos - frag_pos_f);
    float diffuse_cos = max(dot(frag_norm, light_dir), 0.0);
    vec3 diffuse = diffuse_cos * light_color;

    vec3 viewer_dir = normalize(viewer_pos - frag_pos_f);
    vec3 reflect_dir = vec3(0.0);
    float shine_strength = 0.0;
    if (dot(light_dir, frag_norm) > 0.0) {
        reflect_dir = reflect(-light_dir, frag_norm);
        shine_strength = pow(max(dot(viewer_dir, reflect_dir), 0.0), 32);
    }
    vec3 specular = shine_strength * specular_strength * light_color;

    vec3 result = (ambient + diffuse + specular) * object_color;
	frag_color = vec4(result, 1.0);
}