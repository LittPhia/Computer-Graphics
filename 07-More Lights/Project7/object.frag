#version 450 core
struct Material {
	vec4 color;
	sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
	float emission_scale;
    float shininess;
};
struct Light {
    vec4 location;
    vec4 position;
    vec4 direction;
	float cos_cutoff;
	float cos_outer_cutoff;
	vec4 color;
	float[3] attenuation;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

in vec4 normalF;
in vec4 frag_posF;
in vec2 tex_coordsF;
out vec4 frag_color;

uniform vec4 viewer_pos;
uniform Material material;

#define N_LIGHTS 5
uniform Light lights[N_LIGHTS];
uniform Light camera_spotlight;

//////////////////////////////////////////////////////////////////////////////////////////////

vec4 normalize4(vec4 a){
	return vec4(normalize(a.xyz), 0.0);
}

vec4 ambient(Light light) {
	vec4 tex_acolor = material.color * texture(material.diffuse, tex_coordsF).rgba;
	return tex_acolor * light.ambient * light.color;
}

vec4 diffuse(Light light, vec4 light_dir) {
    vec4 frag_norm = normalize4(normalF);
	vec4 tex_dcolor = material.color * texture(material.diffuse, tex_coordsF).rgba;
	
    float diffuse_cos = max(dot(frag_norm, -light_dir), 0.0);
    return tex_dcolor * diffuse_cos * light.diffuse * light.color;
}

vec4 specular(Light light, vec4 light_dir) {
    vec4 frag_norm = normalize4(normalF);
	vec4 spec_part = texture(material.specular, tex_coordsF).rgba;
	vec4 diff_part = vec4(0.1) * texture(material.diffuse, tex_coordsF).rgba;
	vec4 tex_scolor = vec4((spec_part + diff_part).xyz, 1.0);

	vec4 frag_to_viewer = normalize4(viewer_pos - frag_posF);
    vec4 reflect_dir = vec4(0.0);
    float shine_strength = 0.0;
	if (dot(-light_dir, frag_norm) > 0.0) {
		reflect_dir = normalize4(reflect(light_dir, frag_norm));
		float cos_theta = max(dot(frag_to_viewer, reflect_dir), 0.0);
        shine_strength = pow(cos_theta, material.shininess);
    }
    return shine_strength * tex_scolor * light.specular * light.color;
}


vec4 calculate_color(Light light){
	vec4 light_dir = vec4(0.0);
	float distance = length(light.location - frag_posF);
	float attenuation = 1.0 / (light.attenuation[0] + light.attenuation[1] * distance + light.attenuation[2] * distance * distance);

    if (light.position == vec4(0.0)) {								// directional light
		light_dir = normalize4(light.direction);
		return attenuation * (ambient(light) + diffuse(light, light_dir) + specular(light, light_dir));
	}
	else {															// not directional light
		if (light.direction == vec4(0.0)) {					// point light
			light_dir = normalize4(frag_posF - light.position);
			return attenuation * (ambient(light) + diffuse(light, light_dir) + specular(light, light_dir));
		}
		else {												//	flash light
			vec4 frag_to_light = normalize4(light.location - frag_posF);
			light_dir = normalize4(light.direction);
			float cos_theta = dot(frag_to_light, -light_dir);
			float intensity = clamp((cos_theta - light.cos_outer_cutoff) / (light.cos_cutoff - light.cos_outer_cutoff), 0.0, 1.0);
		
			return intensity
				* attenuation
				* (ambient(light) + diffuse(light, light_dir) + specular(light, light_dir));
		}
	}
}


void main(){
	frag_color = vec4(0.0);
	for(int i = 0; i < N_LIGHTS; i++) {
		frag_color += calculate_color(lights[i]);
	}
	frag_color += calculate_color(camera_spotlight);
	frag_color.w = 1.0;

	if (material.emission_scale >= 0.01) {	
		vec4 tex_ecolor = vec4(vec3(material.emission_scale) * texture(material.emission, tex_coordsF).rgb, 1.0);
		if (max(max(tex_ecolor.r, tex_ecolor.g), tex_ecolor.b) > 1.8)
			frag_color = frag_color * tex_ecolor;
		else
			frag_color = vec4(vec3(frag_color + tex_ecolor), 1.0);
	}
}