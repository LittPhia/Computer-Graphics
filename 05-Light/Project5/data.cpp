#include "data.h"

using namespace std;
using namespace glm;

const int SCR_WIDTH = 1600;
const int SCR_HEIGHT = 1600;


float vertices[216] = {
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
							 
	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f
};


vec4 viewer_pos(1.0f, 3.0f, 4.0f, 1.0f);
vec4 object_pos(0.0f, 0.0f, 0.0f, 1.0f);

const vec4 initial_light_pos(1.2f, 1.0f, -0.8f, 1.0f);

GLfloat light_size = 0.2f;
GLfloat object_size = 1.0f;


const GLfloat light_color[3] = { 1.0f, 1.0f, 1.0f };
const GLfloat object_color[3] = { 1.0f, 0.5f, 0.31f };

GLfloat field_of_view = quarter_pi<GLfloat>();
GLfloat near_plane = 0.1f;
GLfloat far_plane = 100.0f;

Camera camera(viewer_pos, vec4(0.0f, 0.0f, 0.0f, 1.0f));


// ---------------------------------------------------------------------------------------------------
void loadVertexData(GLuint VAO, GLuint VBO, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	glEnableVertexAttribArray(index);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
}
