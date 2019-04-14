#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

class Light {
public:
	enum Type {
		ARBITRARY,
		POINTLIGHT,
		DIRECTIONAL,
		FLASHLIGHT
	};
	enum AttenuationdModel {
		D7, D13, D20, D32,
		D50, D65, D100, D160,
		D200, D325, D600, D3250,
		DINFTY,
	};

public:
	static const GLfloat attenuation_lib[DINFTY + 1][3];

	// Function creator not recommended
	static Light Creator(Type type,
			glm::vec4 position,
			glm::vec4 direction,
			GLfloat cutoff,
			GLfloat outer_cutoff,
			const GLfloat attenuation[3],
			glm::vec4 bg_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4 diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4 specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	static Light PointLightCreator(glm::vec4 position,
			const GLfloat attenuation[3] = attenuation_lib[DINFTY],
			glm::vec4 bg_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4 diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4 specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	static Light DirectLightCreator(glm::vec4 direction,
			glm::vec4 bg_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4 diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4 specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	
	static Light FlashLightCreator(
			glm::vec4 position,
			glm::vec4 direction,
			GLfloat cutoff,
			GLfloat outer_cutoff,
			const GLfloat attenuation[3] = attenuation_lib[D3250],
			glm::vec4 bg_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4 diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4 specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));


public:
	Light();
	Light(Type type,
			glm::vec4 bg_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4 diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4 specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

public:
	glm::vec4 getPosition() const {
		return position_;
	}
	glm::vec4 getDirection() const {
		return direction_;
	}
	glm::vec4 getColor() const {
		return color_;
	}
	glm::vec4 getAmbient() const {
		return ambient_;
	}
	glm::vec4 getDiffuse() const {
		return diffuse_;
	}
	glm::vec4 getSpecular() const {
		return specular_;
	}
	GLfloat getCutoff() const {
		return cutoff_;
	}
	GLfloat getOuterCutoff() const {
		return outer_cutoff_;
	}
	const GLfloat *getAttenuation3f() const {
		return attenuation_;
	}

private:
	glm::vec4 position_;
	glm::vec4 direction_;
	glm::vec4 color_;
	glm::vec4 ambient_, diffuse_, specular_;
	GLfloat cutoff_;
	GLfloat outer_cutoff_;
	GLfloat attenuation_[3];
};

