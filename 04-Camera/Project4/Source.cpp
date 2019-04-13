#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image.h>

#include "shader.h"
#include "camera.h"

#define M_PI 3.141592653590

using namespace std;
using namespace glm;


const GLint SCR_WIDTH = 2048;
const GLint SCR_HEIGHT = 1920;

GLdouble field_of_view = M_PI / 2.0;
Camera camera;


/*
		                     1.0+
		                   H----+-------------G
		                  /|    |   -0.5     /|
		                 / |    |    /      / |
		                /  |   0.5+---+----+--+
		               /   |    |  /      /   |
		              /    |    | /      /    |
		             B-----+----+/------A     |
		  |----------|-----+----+-------+--+--+-------|
		-1.0       -0.5    |   0|       | 0.5 |      1.0
		             |     E--/-+-------+-----F
		             |    /  /  |       |    /
		             |   /  -0.5+       |   /
		             |  /  /    |       |  /
		             | /  /     |       | /
		             |/  0.5    |       |/
		             C-------1.0+-------D
*/
const GLfloat vertices[] = {
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	// 0 - A - red
	-0.5f,  0.5f,  0.5f, 	0.0f, 1.0f, 0.0f,	// 1 - B - green
	-0.5f, -0.5f,  0.5f, 	0.0f, 0.0f, 1.0f,	// 2 - C - blue
	 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 0.0f,	// 3 - D - black
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 1.0f,	// 4 - E - cyan
	 0.5f, -0.5f, -0.5f, 	1.0f, 0.0f, 1.0f,	// 5 - F - purple
	 0.5f,  0.5f, -0.5f, 	1.0f, 1.0f, 0.0f,	// 6 - G - yellow
	-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f	// 7 - H - white
};

const GLuint indices[] = {
	0, 1, 2,
	2, 3, 0,

	4, 5, 6,
	6, 7, 4,

	2, 3, 4,
	4, 5, 3,

	0, 1, 7,
	7, 6, 0,

	1, 2 ,4,
	4, 7, 1,

	0, 3, 5,
	5, 6, 0
};

GLfloat grid[] = {
	-100.0,  0.0,  0.0,		1.0, 1.0, 1.0,
	 100.0,  0.0,  0.0,		1.0, 1.0, 1.0,
	 0.0, -100.0,  0.0,		1.0, 1.0, 1.0,
	 0.0,  100.0,  0.0,		1.0, 1.0, 1.0,
	 0.0,  0.0, -100.0,		1.0, 1.0, 1.0,
	 0.0,  0.0,  100.0,		1.0, 1.0, 1.0
};




// ---------------------------------------------------------------------------------------------------

void keyboard_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, GLint width, GLint height);
void cursor_pos_callback(GLFWwindow* window, GLdouble pos_x, GLdouble pos_y);
void scroll_callback(GLFWwindow *window, GLdouble offset_x, GLdouble offset_y);


void loadCubeData(GLuint VAO, GLuint VBO);
void loadCubeIndicesData(GLuint VAO, GLuint EBO);
void loadGridData(GLuint VAO, GLuint VBO);

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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetScrollCallback(window, scroll_callback);
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// Load glad pointer manager -------------------------------------- //
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	// Assemble shader program ---------------------------------------- //
	Shader shader_A;
	shader_A.loadShaderFromFile(GL_VERTEX_SHADER, "vertex.sl");
	shader_A.loadShaderFromFile(GL_FRAGMENT_SHADER, "fragment.sl");
	shader_A.link();
	shader_A.activate();
	
	// Configure vertex attributes ------------------------------------ //
	GLuint VAO[2], VBO[2], EBO;

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	loadCubeData(VAO[0], VBO[0]);
	loadCubeIndicesData(VAO[0], EBO);
	loadGridData(VAO[1], VBO[1]);

	glBindVertexArray(NULL);

	// Visualize ------------------------------------------------------ //
	glBindVertexArray(VAO[0]);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		keyboard_input(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		glBindVertexArray(VAO[0]);
		mat4 view(1.0), model(1.0), projection(1.0f);


		projection = perspective((GLfloat)field_of_view, (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.01f, 1000.0f);
		view = camera.getViewMatrix();

		shader_A.setUniformMatrix4fv("projection", value_ptr(projection));
		shader_A.setUniformMatrix4fv("view", value_ptr(view));
		shader_A.setUniformMatrix4fv("model", value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void *)(0));
		
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_LINES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glBindVertexArray(NULL);

	// Shut down --------------------------------------------------------- //
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(2, VAO);
	
	glfwTerminate();
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------------------------------
void keyboard_input(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.keyboardEvent(GLFW_KEY_UP, GLFW_PRESS);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.keyboardEvent(GLFW_KEY_DOWN, GLFW_PRESS);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.keyboardEvent(GLFW_KEY_LEFT, GLFW_PRESS);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.keyboardEvent(GLFW_KEY_RIGHT, GLFW_PRESS);
}

// ---------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, GLint width, GLint height) {
	glViewport(0, 0, width, height);
}

// ---------------------------------------------------------------------------------------------------
void cursor_pos_callback(GLFWwindow *window, GLdouble pos_x, GLdouble pos_y) {
	static GLdouble last_X = 0.0, last_Y = 0.0;
	static bool is_first_call = true;
	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (is_first_call) {
			last_X = pos_x, last_Y = pos_y;
			is_first_call = false;
		}
		GLdouble offset_X = pos_x - last_X,
			offset_Y = last_Y - pos_y;
		last_X = pos_x;
		last_Y = pos_y;

		camera.mouseMovementEvent((GLfloat)offset_X, (GLfloat)offset_Y);
	}
	else {
		is_first_call = true;
	}
}

// ---------------------------------------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, GLdouble offset_x, GLdouble offset_y) {
	field_of_view -= Camera::MouseSensitivity * 50.0 * pi<GLdouble>() * offset_y;
	if (field_of_view < pi<GLdouble>() / 36.0)
		field_of_view = pi<GLdouble>() / 36.0;
	else if (field_of_view > 24.0 / 36.0 * pi<GLdouble>())
		field_of_view = 24.0 / 36.0 * pi<GLdouble>();
}

// ---------------------------------------------------------------------------------------------------
void loadCubeData(GLuint VAO, GLuint VBO) {
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(NULL);
}

// ---------------------------------------------------------------------------------------------------
void loadCubeIndicesData(GLuint VAO, GLuint EBO) {
	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glBindVertexArray(NULL);
}

// ---------------------------------------------------------------------------------------------------
void loadGridData(GLuint VAO, GLuint VBO) {
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grid), grid, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(NULL);
}
