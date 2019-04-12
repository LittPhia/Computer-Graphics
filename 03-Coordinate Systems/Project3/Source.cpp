#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image.h>

#include "shader.h"


using namespace std;
using namespace glm;


const int SCR_WIDTH = 2736;
const int SCR_HEIGHT = 1824;



/*
						 1.0+
					   E----+-------------F
					  /|    |    0.5     /|
					 / |	|    /      / |
				 	/  |   0.5+---+----+--+
				   /   |    |  /      /   |
				  /    |    | /      /    |
				 B-----+----+/------A     |
	  |----------|-----+----+-------+--+--+-------|
	-1.0       -0.5    |   0|       | 0.5 |      1.0
				 |     H--/-+-------+-----G
				 |	  /  /	|       |    /
				 |	 /  -0.5+       |   /
				 |  /  /	|       |  /
				 | /  /		|       | /
				 |/ -0.5	|       |/
				 C-------1.0+-------D
				  
*/
const GLfloat vertices[] = {
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// 0 - A - red
	-0.5f,  0.5f,  0.5f, 	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		// 1 - B - green
	-0.5f, -0.5f,  0.5f, 	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		// 2 - C - blue
	 0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// 3 - D - yellow
				   
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// 4 - A - red
	 0.5f, -0.5f,  0.5f, 	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		// 5 - D - green
	 0.5f, -0.5f, -0.5f, 	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		// 6 - G - blue
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// 7 - F - yellow
				  
	 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// 8 - G - red
	-0.5f, -0.5f, -0.5f, 	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		// 9 - H - green
	-0.5f,  0.5f, -0.5f, 	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		// 10 - E - blue
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// 11 - F - yellow
				   
	-0.5f, -0.5f,  0.5f,	1.0f, 0.0f,  0.0f,	1.0f, 1.0f,		// 12 - C - red
	-0.5f,  0.5f,  0.5f, 	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		// 13 - B - green
	-0.5f,  0.5f, -0.5f, 	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		// 14 - E - blue
	-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// 15 - H - yellow
				  
	-0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// 16 - E - red
	-0.5f,  0.5f,  0.5f, 	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		// 17 - B - green
	 0.5f,  0.5f,  0.5f, 	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		// 18 - A - blue
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// 19 - F - yellow
				  
	 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// 20 - G - red
	 0.5f, -0.5f,  0.5f, 	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		// 21 - D - green
	-0.5f, -0.5f,  0.5f, 	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		// 22 - C - blue
	-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	1.0f, 0.0f		// 23 - H - yellow
};

const GLuint indices[] = {
	0, 1, 2,
	2, 3, 0,

	4, 5, 6,
	6, 7, 4,

	8, 9, 10,
	10, 11, 8,

	12, 13, 14,
	14, 15, 12,

	16, 17 ,18,
	18, 19, 16,

	20, 21, 22,
	22, 23, 20
};


const size_t n_cubes = 10;
const vec3 cube_positions[n_cubes] = {
  vec3( 0.0f,  0.0f,  0.0f),
  vec3( 2.0f,  5.0f, -15.0f),
  vec3(-1.5f, -2.2f, -2.5f),
  vec3(-3.8f, -2.0f, -12.3f),
  vec3( 2.4f, -0.4f, -3.5f),
  vec3(-1.7f,  3.0f, -7.5f),
  vec3( 1.3f, -2.0f, -2.5f),
  vec3( 1.5f,  2.0f, -2.5f),
  vec3( 1.5f,  0.2f, -1.5f),
  vec3(-1.3f,  1.0f, -1.5f)
};


void process_input(GLFWwindow *window);
void reshape(GLFWwindow *window, GLint width, GLint height);

void loadVertexData(GLuint VAO, GLuint VBO);
void loadIndexData(GLuint VAO, GLuint EBO);
void loadTexture(const char *tex_filename, GLenum target, GLint level, GLint internalformat, GLint border, GLenum format);
void bindTexture(GLenum target, GLuint texture, GLenum actived_texture, GLfloat border_color[4], GLint params[2][2]);

// ---------------------------------------------------------------------------------------------------
int main() {
	// Create window ////////////////////////////////////////////////////
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TURE);
#endif

	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
	if (window == nullptr) {
		std::cerr << "Fail to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, reshape);
	
	// Load glad pointer manager ////////////////////////////////////////
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Assemble shader program //////////////////////////////////////////
	Shader shader_A;
	shader_A.loadShaderFromFile(GL_VERTEX_SHADER, "vertex.sl");
	shader_A.loadShaderFromFile(GL_FRAGMENT_SHADER, "fragment.sl");
	shader_A.link();
	shader_A.activate();

	// Configure vertex attributes //////////////////////////////////////
	GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	loadVertexData(VAO, VBO);
	loadIndexData(VAO, EBO);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glBindVertexArray(NULL);

	// Configure texture attributes /////////////////////////////////////
	GLuint texture;
	GLint params[2][2] = { { GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT }, { GL_LINEAR, GL_LINEAR} };

	glGenTextures(1, &texture);
	bindTexture(GL_TEXTURE_2D, texture, GL_TEXTURE0, nullptr, params);
	loadTexture("awesomeface.png", GL_TEXTURE_2D, 0, GL_RGBA, 0, GL_RGBA);
	shader_A.setUniform1i("texture_1", 0);

	// Visualize ////////////////////////////////////////////////////////
	glBindVertexArray(VAO);
	glEnable(GL_DEPTH_TEST);

	mat4 view(1.0), projection(1.0);

	projection = perspective(pi<GLfloat>() / 4.0f, (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
	view = translate(view, vec3(0.0f, 0.0f, -3.0f));

	shader_A.setUniformMatrix4fv("projection", value_ptr(projection));
	shader_A.setUniformMatrix4fv("view", value_ptr(view));

	while (!glfwWindowShouldClose(window)) {
		process_input(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat angle = -(GLfloat)glfwGetTime();
		for (size_t i = 0; i < n_cubes; i++) {
			mat4 model(1.0);
			model = translate(model, cube_positions[i]);
			model = rotate(model, angle*(i + 1), vec3(0.5f, 1.0f, 0.0f));

			shader_A.setUniformMatrix4fv("model", value_ptr(model));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void *)(0));
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glBindVertexArray(NULL);

	// Shut down ////////////////////////////////////////////////////////
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(1, &texture);

	glfwTerminate();
	return 0;
}


// ---------------------------------------------------------------------------------------------------
void process_input(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// ---------------------------------------------------------------------------------------------------
void reshape(GLFWwindow *window, GLint width, GLint height) {
	glViewport(0, 0, width, height);
}

// ---------------------------------------------------------------------------------------------------
void loadVertexData(GLuint VAO, GLuint VBO) {
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(NULL);
}

// ---------------------------------------------------------------------------------------------------
void loadIndexData(GLuint VAO, GLuint EBO) {
	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glBindVertexArray(NULL);
}


// ---------------------------------------------------------------------------------------------------
void loadTexture(const char *tex_filename, GLenum target, GLint level, GLint internalformat, GLint border, GLenum format) {
	GLsizei width, height, nr_channels;
	
	stbi_set_flip_vertically_on_load(true);
	GLubyte *data = stbi_load(tex_filename, (int*)&width, (int*)&height, (int*)&nr_channels, 0);
	if (data != nullptr) {
		glTexImage2D(target, level, internalformat, width, height, border, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(target);
	}
	else {
		cerr << "Failed to load texture from: " << tex_filename << endl;
	}
	stbi_image_free(data);
}

// ---------------------------------------------------------------------------------------------------
// param = { { param GL_TEXTURE_WRAP_S,		param GL_TEXTURE_WRAP_T },
//           { param GL_TEXTURE_MIN_FILTER,	param GL_TEXTURE_MAG_FILTER } };
void bindTexture(GLenum target, GLuint texture, GLenum actived_texture, GLfloat border_color[4], GLint params[2][2]) {
	glActiveTexture(actived_texture);
	glBindTexture(target, texture);

	if (border_color != nullptr)
		glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, border_color);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, params[0][0]);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, params[0][1]);

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, params[1][0]);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, params[1][1]);
}
