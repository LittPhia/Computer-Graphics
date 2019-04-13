#include "class - camera/camera.h"
#include "class - shader/shader.h"

#include "data.h"
using namespace glm;

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
	oshader.loadShaderFromFile(GL_VERTEX_SHADER, "object.vs");
	oshader.loadShaderFromFile(GL_FRAGMENT_SHADER, "object.fs");
	oshader.link();
	
	lshader.loadShaderFromFile(GL_VERTEX_SHADER, "light.vs");
	lshader.loadShaderFromFile(GL_FRAGMENT_SHADER, "light.fs");
	lshader.link();

	// Configure vertex attributes ----------------------------------- //
	GLuint object_VAO, light_VAO;
	GLuint VBO;
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &light_VAO);
	loadVertexData(light_VAO, VBO, 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(0));
	
	glGenVertexArrays(1, &object_VAO);
	loadVertexData(object_VAO, VBO, 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(0));
	loadVertexData(object_VAO, VBO, 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	
	// Visualize ----------------------------------------------------- //
	glEnable(GL_DEPTH_TEST);

	
	mat4 light_model(1.0f);
	mat4 object_model = translate(mat4(1.0f), vec3(object_pos));
		object_model = scale(object_model, vec3(object_size));
	
	mat4 view = mat4(1.0f);
	mat4 projection = perspective(field_of_view, (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, near_plane, far_plane);
	
	while (!glfwWindowShouldClose(window)) {
		keyboardInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = camera.getViewMatrix();
		light_model = rotate(mat4(1.0f), (GLfloat)glfwGetTime(), vec3(0.0, 1.0, 0.0));
		light_model = translate(light_model, vec3(initial_light_pos));
		light_model = scale(light_model, vec3(light_size));

		lshader.activate();
		lshader.setMat4fv("projection", value_ptr(projection));
		lshader.setMat4fv("view", value_ptr(view));
		lshader.setMat4fv("model", value_ptr(light_model));

		lshader.setVec3fv("light_color", light_color);
		glBindVertexArray(light_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		
		vec3 light_pos = light_model * initial_light_pos;

		oshader.activate();
		oshader.setMat4fv("projection", value_ptr(projection));
		oshader.setMat4fv("view", value_ptr(view));
		oshader.setMat4fv("model", value_ptr(object_model));
		oshader.setMat3fv("norm_matrix", value_ptr(inverse(transpose(mat3(object_model)))));

		oshader.setVec3fv("viewer_pos", value_ptr(vec3(camera.getPosition())));
		oshader.setVec3fv("light_pos", value_ptr(light_pos));
		oshader.setVec3fv("object_color", object_color);
		oshader.setVec3fv("light_color", light_color);
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
		+ glfwGetKey(window, GLFW_KEY_D);
	proc_time /= sqrt(num);


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.keyboardEvent(Camera::FORWARD, Camera::PRESSED, (GLfloat)proc_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.keyboardEvent(Camera::BACKWARD, Camera::PRESSED, (GLfloat)proc_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.keyboardEvent(Camera::LEFT, Camera::PRESSED, (GLfloat)proc_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.keyboardEvent(Camera::RIGHT, Camera::PRESSED, (GLfloat)proc_time);

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
