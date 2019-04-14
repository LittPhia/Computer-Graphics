#include "light.h"
using namespace glm;

const GLfloat Light::attenuation_lib[Light::DINFTY + 1][3] = {
	{ 1.0f, 0.7f, 1.8f },
	{ 1.0f, 0.35f, 0.44f },
	{ 1.0f, 0.22f, 0.20f },
	{ 1.0f, 0.14f, 0.07f },
	{ 1.0f, 0.09f, 0.032f },
	{ 1.0f, 0.07f, 0.017f },
	{ 1.0f, 0.045f, 0.0075f },
	{ 1.0f, 0.027f, 0.0028f },
	{ 1.0f, 0.022f, 0.0019f },
	{ 1.0f, 0.014f, 0.0007f },
	{ 1.0f, 0.007f, 0.0002f },
	{ 1.0f, 0.0014f, 0.000007f },
	{ 1.0f, 0.0f, 0.0f }
};

// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////// //

Light Light::Creator
(	Type type,
	vec4 position,
	vec4 direction,
	GLfloat cutoff,
	GLfloat outer_cutoff,
	const GLfloat attenuation[3],
	vec4 bg_color,
	vec4 ambient,
	vec4 diffuse,
	vec4 specular) {

	switch (type) {
	case POINTLIGHT:
		return PointLightCreator(position, attenuation, bg_color, ambient, diffuse, specular);
		break;
	case DIRECTIONAL:
		return DirectLightCreator(direction, bg_color, ambient, diffuse, specular);
		break;
	case FLASHLIGHT:
		return FlashLightCreator(position, direction, cutoff, outer_cutoff, attenuation, bg_color, ambient, diffuse, specular);
		break;
	default:	case ARBITRARY:
		Light new_light = PointLightCreator(position, attenuation, bg_color, ambient, diffuse, specular);
		new_light.direction_ = direction;
		new_light.cutoff_ = cutoff;
		new_light.outer_cutoff_ = outer_cutoff;
		return new_light;
	}
}

Light Light::PointLightCreator
(	vec4 position,
	const GLfloat attenuation[3],
	vec4 bg_color, 
	vec4 ambient,
	vec4 diffuse,
	vec4 specular) {

	Light new_light;
	new_light.position_ = position;
	new_light.direction_ = vec4(0.0f);
	new_light.color_ = bg_color;
	new_light.ambient_ = ambient;
	new_light.diffuse_ = diffuse;
	new_light.specular_ = specular;
	new_light.cutoff_ = 0.0f;
	new_light.outer_cutoff_ = 0.0f;
	new_light.attenuation_[0] = attenuation[0];
	new_light.attenuation_[1] = attenuation[1];
	new_light.attenuation_[2] = attenuation[2];

	return new_light;
}

Light Light::DirectLightCreator
(	vec4 direction,
	vec4 bg_color,
	vec4 ambient,
	vec4 diffuse,
	vec4 specular ) {

	Light new_light;
	new_light.position_ = vec4(0.0f);
	new_light.direction_ = direction;
	new_light.color_ = bg_color;
	new_light.ambient_ = ambient;
	new_light.diffuse_ = diffuse;
	new_light.specular_ = specular;
	new_light.cutoff_ = 0.0f;
	new_light.outer_cutoff_ = 0.0f;
	new_light.attenuation_[0] = attenuation_lib[DINFTY][0];
	new_light.attenuation_[1] = attenuation_lib[DINFTY][1];
	new_light.attenuation_[2] = attenuation_lib[DINFTY][2];

	return new_light;
}

Light Light::FlashLightCreator
(	vec4 position,
	vec4 direction,
	GLfloat cutoff,
	GLfloat outer_cutoff,
	const GLfloat attenuation[3],
	vec4 bg_color,
	vec4 ambient,
	vec4 diffuse,
	vec4 specular) {

	Light new_light;
	new_light.position_ = position;
	new_light.direction_ = direction;
	new_light.color_ = bg_color;
	new_light.ambient_ = ambient;
	new_light.diffuse_ = diffuse;
	new_light.specular_ = specular;
	new_light.cutoff_ = cutoff;
	new_light.outer_cutoff_ = outer_cutoff;
	new_light.attenuation_[0] = attenuation[0];
	new_light.attenuation_[1] = attenuation[1];
	new_light.attenuation_[2] = attenuation[2];

	return new_light;
}

Light::Light() {
	color_ = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	ambient_ = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	diffuse_ = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	specular_ = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

Light::Light
(	Type type,
	vec4 bg_color,
	vec4 ambient,
	vec4 diffuse,
	vec4 specular) {

	switch (type) {
	case Light::POINTLIGHT:
		*this = PointLightCreator(bg_color, attenuation_lib[DINFTY], bg_color, ambient, diffuse, specular);
		break;
	case Light::DIRECTIONAL:
		*this = DirectLightCreator(vec4(0.0f, -1.0f, 0.0f, 0.0f), bg_color, ambient, diffuse, specular);
		break;
	case Light::FLASHLIGHT:
		*this = PointLightCreator(vec4(0.0f, 0.0f, 0.0f, 1.0f), attenuation_lib[DINFTY], bg_color, ambient, diffuse, specular);
		break;
	default:	case Light::ARBITRARY:
		*this = Creator(type, vec4(0.0f), vec4(0.0f), 0.0f, 0.0f, attenuation_lib[DINFTY], bg_color, ambient, diffuse, specular);
		break;
	}
	
}
