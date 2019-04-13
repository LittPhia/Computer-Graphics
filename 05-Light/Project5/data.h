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

extern GLfloat vertices[216];
extern Camera camera;

extern glm::vec4 viewer_pos;
extern glm::vec4 object_pos;

extern const glm::vec4 initial_light_pos;

extern GLfloat light_size;
extern GLfloat object_size;

extern GLfloat field_of_view;
extern GLfloat near_plane;
extern GLfloat far_plane;

extern const GLfloat light_color[3];
extern const GLfloat object_color[3];

// /////////////////////////////////////////////////////////////////////// //

extern void loadVertexData(GLuint VAO, GLuint VBO, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

