#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "class - camera/camera.h"

// /////////////////////////////////////////////////////////////////////// //

extern const int SCR_WIDTH;
extern const int SCR_HEIGHT;

extern glm::vec4 viewer_pos;

struct PointLight {
	glm::vec4 color;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
};
struct Lamp {
	glm::vec4 position;
	PointLight light;
	GLfloat zoom;
};
extern Lamp lamp;

struct Material {
	glm::vec4 color;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	GLfloat shininess;
};
struct Cube {
	GLfloat *vertices;
	GLuint nverts;
	glm::vec4 position;
	GLfloat zoom;
	Material material;
};
extern Cube cube;


extern Camera camera;
extern GLfloat field_of_view;
extern GLfloat near_plane;
extern GLfloat far_plane;

// /////////////////////////////////////////////////////////////////////// //

extern void loadVertexData(GLuint VAO, GLuint VBO, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

