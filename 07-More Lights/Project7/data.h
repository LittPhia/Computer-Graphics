#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>
#include <iostream>

#include "class - camera/camera.h"
#include "class - light/light.h"

#define N_LAMPS 5
#define N_CUBES 10

// /////////////////////////////////////////////////////////////////////// //

extern const int SCR_WIDTH;
extern const int SCR_HEIGHT;

extern GLfloat vertices[288];

struct Lamp {
	glm::vec4 position;
	Light light;
	GLfloat sizezoom;
};
struct Material {
	glm::vec4 bg_color;
	GLfloat shininess;
};
struct RotatingCube {
	Material *cubes_material;

	glm::vec4 position;
	glm::vec4 rotation_axis;
	GLfloat angle_per_second;
	GLfloat sizezoom;
};

extern Material cubes_material;

extern Camera camera;
extern Lamp lamps[N_LAMPS];
extern RotatingCube cubes[N_CUBES];

extern GLfloat field_of_view;
extern GLfloat near_plane;
extern GLfloat far_plane;

// /////////////////////////////////////////////////////////////////////// //

extern void loadVertexData(GLuint VAO, GLuint VBO, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
extern void loadTextureData(const char *tex_filename, GLenum target, GLint level, GLint border, GLenum type);
extern void bindTexture(GLuint VAO, GLenum target, GLuint texture, GLenum actived_texture);

extern void initLamps();
extern void initCubes();

extern glm::vec4 normalize4(GLfloat x, GLfloat y, GLfloat z);
extern glm::vec4 normalize4(const glm::vec4 &v);
extern GLfloat randf();
