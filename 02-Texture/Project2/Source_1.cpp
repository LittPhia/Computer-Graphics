#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "stb_image.h"
#include "Shaders.h"

using namespace std;


const GLuint SCR_WIDTH = 1200;
const GLuint SCR_HEIGHT = 800;

GLfloat mix_value = 0.2;


/*
						 1.0+
							|
							|
						    |
				 D		 0.5+          A
						    |
						    |
	                        |
	  |----------|----------+----------|----------|
	-1.0       -0.5        0|         0.5        1.0
						    |
						    |
				 C	    -0.5+          B
							|
							|
							|
					    -1.0+
*/

GLfloat vertices[] = {
	 1.0,  1.0, 0.0,	1.0, 0.0, 0.0,	0.3, 0.3,		// A - red
	 1.0, -1.0, 0.0,	0.0, 1.0, 0.0,	0.3, 0.0,		// B - green
	-1.0,  1.0, 0.0,	1.0, 1.0, 0.0,	0.0, 0.3,		// D - yellow

	 1.0, -1.0, 0.0,	0.0, 1.0, 0.0,	0.3, 0.0,		// B - green
	-1.0, -1.0, 0.0,	0.0, 0.0, 1.0,	0.0, 0.0,		// C - blue
	-1.0,  1.0, 0.0,	1.0, 1.0, 0.0,	0.0, 0.3		// D - yellow
};

void process_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void bindTexture(GLenum target, GLuint texture, GLenum actived_texture, GLint params[2][2]);
void loadVertexData(GLuint VAO, GLuint VBO);
void loadTexture(const char *tex_filename, GLenum target, GLint level, GLint internalformat, GLint border, GLenum format, GLenum type);
void display(Shaders *shader, GLuint *VAO);


// ---------------------------------------------------------------------------------------------
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	Shaders shader;
	shader.assembleVertexShader("vertex.sl");
	shader.assembleFragmentShader("fragment.sl");
	shader.link();


	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	loadVertexData(VAO, VBO);

	glDeleteBuffers(1, &VBO);


	GLuint textures[2];
	glGenTextures(2, textures);

	GLint params[2][2] = {
		{ GL_REPEAT, GL_REPEAT },
		{ GL_LINEAR, GL_LINEAR }
	};

	params[0][0] = params[0][1] = GL_CLAMP_TO_EDGE;
	bindTexture(GL_TEXTURE_2D, textures[0], GL_TEXTURE0, params);
	loadTexture("container.jpg", GL_TEXTURE_2D, 0, GL_RGB, 0, GL_RGB, GL_UNSIGNED_BYTE);

	params[0][0] = params[0][1] = GL_MIRRORED_REPEAT;
	params[1][0] = params[1][1] = GL_NEAREST;
	bindTexture(GL_TEXTURE_2D, textures[1], GL_TEXTURE1, params);
	loadTexture("awesomeface.png", GL_TEXTURE_2D, 0, GL_RGBA, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	

	shader.activate();
	shader.setUniformInt("texture_1", 0);
	shader.setUniformInt("texture_2", 1);


	while (!glfwWindowShouldClose(window)) {
		process_input(window);

		shader.setUniformFloat("mix_value", mix_value);
		display(&shader, &VAO);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(2, textures);

	glfwTerminate();
	return 0;
}


// ---------------------------------------------------------------------------------------------
void bindTexture(GLenum target, GLuint texture, GLenum actived_texture, GLint params[2][2]) {
	glActiveTexture(actived_texture);
	glBindTexture(target, texture);
	
	GLfloat borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, params[0][0]);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, params[0][1]);

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, params[1][0]);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, params[1][1]);
}

void loadVertexData(GLuint VAO, GLuint VBO) {
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
}

void loadTexture(const char *tex_filename, GLenum target, GLint level, GLint internalformat, GLint border, GLenum format, GLenum type) {
	GLint width, height, channels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(tex_filename, (int*)&width, (int*)&height, (int*)&channels, 0);
	if (data != nullptr) {
		glTexImage2D(target, level, internalformat, width, height, border, format, type, data);
		glGenerateMipmap(target);
	}
	else {
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);
}

void display(Shaders *shader, GLuint *VAO) {
	glClearColor(0.2, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(*VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);
	glBindVertexArray(NULL);
}

// ---------------------------------------------------------------------------------------------------------
void process_input(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mix_value += 0.001;
		if (mix_value > 1.0) {
			mix_value = 1.0;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mix_value -= 0.001;
		if (mix_value < 0.0) {
			mix_value = 0.0;
		}
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}