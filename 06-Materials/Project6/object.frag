#version 450 core
struct Material{
	vec4 color;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};
struct Light {
    vec4 position;
	vec4 color;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

in vec4 normalF;
in vec4 frag_posF;
out vec4 frag_color;

uniform vec4 viewer_pos;

uniform Material material;
uniform Light light;


void main(){
    vec4 frag_norm = normalize(normalF);
    vec4 mixed_color = material.color * light.color;
    vec4 ambient = material.ambient * light.ambient * mixed_color;
    
    vec4 light_dir = normalize(light.position - frag_posF);
    float diffuse_cos = max(dot(frag_norm, light_dir), 0.0);
    vec4 diffuse = material.diffuse * diffuse_cos * light.diffuse * mixed_color;

	vec4 viewer_dir = normalize(viewer_pos - frag_posF);
    vec4 reflect_dir = vec4(0.0);
    float shine_strength = 0.0;
	if (dot(light_dir, frag_norm) > 0.0) {
		reflect_dir = reflect(-light_dir, frag_norm);
        shine_strength = pow(max(dot(viewer_dir, reflect_dir), 0.0),  material.shininess);
    }
    vec4 specular = shine_strength * material.specular * light.specular * mixed_color;

	frag_color = ambient + diffuse + specular;
}