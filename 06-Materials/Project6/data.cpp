#include "data.h"

using namespace std;
using namespace glm;

const int SCR_WIDTH = 1600;
const int SCR_HEIGHT = 1600;

GLfloat vertices[216] = {
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

Lamp lamp {
	// PointLight //
	vec4(1.2f, 1.0f, 2.0f, 1.0f),			// position
	{ vec4(1.0f, 1.0f, 1.0f, 1.0f),			// color
	  vec4(0.2f, 0.2f, 0.2f, 1.0f),			// ambient
	  vec4(0.5f, 0.5f, 0.5f, 1.0f),			// diffuse
	  vec4(1.0f, 1.0f, 1.0f, 1.0f), },		// specular
	// PointLight END //

	0.2f					// zoom
};

Cube cube {
	vertices,				// vertices
	sizeof(vertices),		// number of vertices
	vec4(0.0f, 0.0f, 0.0f, 1.0f),			// position
	1.0f,					// zoom

	// Material //
	{ vec4(1.0f, 0.5f, 0.31f, 1.0f),		// color
	  vec4(1.0f, 1.0f, 1.0f, 1.0f),			// ambient
	  vec4(1.0f, 1.0f, 1.0f, 1.0f),			// diffuse
	  vec4(0.8f, 0.8f, 0.8f, 1.0f),			// specular
	  32.0f }								// shininess
	// Material END //
};

vec4 viewer_pos(0.0f, 0.0f, 3.0f, 1.0f);

Camera camera(viewer_pos, vec4(0.0f, 0.0f, 0.0f, 1.0f));
GLfloat field_of_view = quarter_pi<GLfloat>();
GLfloat near_plane = 0.1f;
GLfloat far_plane = 100.0f;


// ---------------------------------------------------------------------------------------------------
void loadVertexData(GLuint VAO, GLuint VBO, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	glEnableVertexAttribArray(index);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
}
