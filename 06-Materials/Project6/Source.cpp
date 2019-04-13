#include "class - camera/camera.h"
#include "class - shader/shader.h"

#include "data.h"
using namespace glm;

#include <ctime>
#include <iostream>
using namespace std;


void keyboardInput(GLFWwindow *window);
void framebufferSizeCallback(GLFWwindow *window, GLint width, GLint height);
void cursorPosCallback(GLFWwindow* window, GLdouble pos_x, GLdouble pos_y);

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
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Load glad pointer manager ------------------------------------- //
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Assemble shader program --------------------------------------- //
	Shader oshader, lshader;
	oshader.loadShaderFromFile(GL_VERTEX_SHADER, "object.vert");
	oshader.loadShaderFromFile(GL_FRAGMENT_SHADER, "object.frag");
	oshader.link();
	
	lshader.loadShaderFromFile(GL_VERTEX_SHADER, "light.vert");
	lshader.loadShaderFromFile(GL_FRAGMENT_SHADER, "light.frag");
	lshader.link();

	// Configure vertex attributes ----------------------------------- //
	GLuint object_VAO, light_VAO;
	GLuint VBO;
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, cube.nverts, cube.vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &light_VAO);
	loadVertexData(light_VAO, VBO, 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(0));
	
	glGenVertexArrays(1, &object_VAO);
	loadVertexData(object_VAO, VBO, 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(0));
	loadVertexData(object_VAO, VBO, 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	
	// Visualize ----------------------------------------------------- //
	glEnable(GL_DEPTH_TEST);
	
	mat4 light_model(1.0f);
	mat4 object_model(1.0f);
	
	mat4 view = mat4(1.0f);
	mat4 projection = perspective(field_of_view, (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, near_plane, far_plane);

	while (!glfwWindowShouldClose(window)) {
		keyboardInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		view = camera.getViewMatrix();
		light_model = translate(mat4(1.0f), vec3(lamp.position));
		light_model = scale(light_model, vec3(lamp.zoom));

		lshader.activate();
		lshader.setMat4fv("projection", projection);
		lshader.setMat4fv("view", view);
		lshader.setMat4fv("model", light_model);

		lshader.setVec4fv("light_color", lamp.light.color);
		glBindVertexArray(light_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		object_model = translate(mat4(1.0f), vec3(cube.position));
		object_model = scale(object_model, vec3(cube.zoom));

		oshader.activate();
		oshader.setMat4fv("projection", projection);
		oshader.setMat4fv("view", view);
		oshader.setMat4fv("model", object_model);
		oshader.setMat4fv("norm_matrix", inverse(transpose(object_model)));

		oshader.setVec4fv("material.color", cube.material.color);
		oshader.setVec4fv("material.ambient", cube.material.ambient);
		oshader.setVec4fv("material.diffuse", cube.material.diffuse);
		oshader.setVec4fv("material.specular", cube.material.specular);
		oshader.set1f("material.shininess", cube.material.shininess);

		oshader.setVec4fv("light.position", lamp.position);
		oshader.setVec4fv("light.color", lamp.light.color);
		oshader.setVec4fv("light.ambient", lamp.light.ambient);
		oshader.setVec4fv("light.diffuse", lamp.light.diffuse);
		oshader.setVec4fv("light.specular", lamp.light.specular);

		oshader.setVec4fv("viewer_pos", camera.getPosition());

		glBindVertexArray(object_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glBindVertexArray(NULL);

	// Shut down ////////////////////////////////////////////////////////
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &object_VAO);
	glDeleteVertexArrays(1, &light_VAO);

	glfwTerminate();
	return 0;
}


// ---------------------------------------------------------------------------------------------------
void keyboardInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	static double proc_time = 0.0;
	static double last_time = 0.0;
	proc_time = glfwGetTime() - last_time;
	last_time = glfwGetTime();

	int num = glfwGetKey(window, GLFW_KEY_W)
		+ glfwGetKey(window, GLFW_KEY_S)
		+ glfwGetKey(window, GLFW_KEY_A)
		+ glfwGetKey(window, GLFW_KEY_D)
		+ glfwGetKey(window, GLFW_KEY_SPACE);
	proc_time /= sqrt(num);


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.keyboardEvent(Camera::FORWARD, Camera::PRESSED, (GLfloat)proc_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.keyboardEvent(Camera::BACKWARD, Camera::PRESSED, (GLfloat)proc_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.keyboardEvent(Camera::LEFT, Camera::PRESSED, (GLfloat)proc_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.keyboardEvent(Camera::RIGHT, Camera::PRESSED, (GLfloat)proc_time);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.keyboardEvent(Camera::UP, Camera::PRESSED, (GLfloat)proc_time);

}

// ---------------------------------------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow *window, GLint width, GLint height) {
	glViewport(0, 0, width, height);
}

// ---------------------------------------------------------------------------------------------------
void cursorPosCallback(GLFWwindow *window, GLdouble pos_x, GLdouble pos_y) {
	static GLdouble last_x = 0.0, last_y = 0.0;
	static bool is_first_call = true;

	if (is_first_call) {
		last_x = pos_x, last_y = pos_y;
		is_first_call = false;
	}
	GLdouble offset_X = last_x - pos_x,
		offset_Y = pos_y - last_y;
	last_x = pos_x;
	last_y = pos_y;

	camera.mouseMovementEvent((GLfloat)offset_X, (GLfloat)offset_Y);
}
