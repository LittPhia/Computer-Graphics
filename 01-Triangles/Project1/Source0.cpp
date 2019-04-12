#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace main_0 {
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow *window);
	void assemble_shaders();
	void import_data();
	void display();

	const unsigned int SCR_WIDTH = 1600;
	const unsigned int SCR_HEIGHT = 1200;


	/*
					   |
				   0.5 |
					   |
		---------------|--------------
		 -1   -0.5    0|    0.5    1
					   |
				  -0.5 |
					   |
	*/
	GLfloat vertices_0[] = {
		-1.0,  0.5, 0.0,
		-0.5,  0.5, 0.0,
		-1.0,  1.0, 0.0,

		 0.5,  0.5, 0.0,
		 1.0,  0.5, 0.0,
		 1.0,  1.0, 0.0,
	};

	GLfloat vertices_1[] = {
		-0.5,  0.0, 0.0,
		 0.5,  0.5, 0.0,
		 0.0,  0.5, 0.0,

		-0.5, -0.5, 0.0,
		 0.5, -0.5, 0.0,
		 0.0,  0.0, 0.0
	};


	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	GLuint VBOs[2], VAOs[2], EBO;

	const GLchar *const vsrc =
		"#version 450 core\n"

		"layout(location = 0) in vec3 aPos;"

		"out vec4 vertex_color;"
		"uniform vec4 ucolor;"

		"void main() {"
		"	gl_Position = vec4(aPos, 1.0);"
		"	vertex_color = ucolor;"
		"}";

	const GLchar *const fsrc =
		"#version 450 core\n"

		"out vec4 FragColor;"
		"in vec4 vertex_color;"

		"void main() {"
		"	FragColor = vertex_color;"
		"}";


	GLuint id_vertex_shader,
		id_fragment_shader,
		shader_program;


	int main_0() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
		if (window == NULL) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		assemble_shaders();
		import_data();

		while (!glfwWindowShouldClose(window)) {
			processInput(window);
			display();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(2, VAOs);
		glDeleteBuffers(2, VBOs);
		glDeleteBuffers(2, &EBO);

		glfwTerminate();
		return 0;
	}

	// ---------------------------------------------------------------------------------------------------------
	void processInput(GLFWwindow *window) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}


	void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	void assemble_shaders() {
		int suc;
		char infoLog[512];

		id_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(id_vertex_shader, 1, &vsrc, NULL);
		glCompileShader(id_vertex_shader);

		glGetShaderiv(id_vertex_shader, GL_COMPILE_STATUS, &suc);
		if (!suc) {
			glGetShaderInfoLog(id_vertex_shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		id_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(id_fragment_shader, 1, &fsrc, NULL);
		glCompileShader(id_fragment_shader);
		glGetShaderiv(id_fragment_shader, GL_COMPILE_STATUS, &suc);
		if (!suc) {
			glGetShaderInfoLog(id_fragment_shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		shader_program = glCreateProgram();
		glAttachShader(shader_program, id_vertex_shader);
		glAttachShader(shader_program, id_fragment_shader);
		glLinkProgram(shader_program);

		glGetProgramiv(shader_program, GL_LINK_STATUS, &suc);
		if (!suc) {
			glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}


		glDeleteShader(id_vertex_shader);
		glDeleteShader(id_fragment_shader);
	}

	void import_data() {
		glGenVertexArrays(2, VAOs);
		glGenBuffers(2, VBOs);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAOs[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_0), vertices_0, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
		glEnableVertexAttribArray(0);

		glBindVertexArray(VAOs[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void display() {
		static GLfloat ucolor[3][4] = {
			{ 1.0, 0.0, 0.0, 1.0 },
			{ 1.0, 1.0, 0.0, 1.0 },
			{ 0.0, 0.0, 1.0, 1.0 }
		};
		static GLint i = 0;

		glClearColor(0.2, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		GLfloat time_value = glfwGetTime();
		GLint ucolor_location = glGetUniformLocation(shader_program, "ucolor");
		glUseProgram(shader_program);

		glUniform4fv(ucolor_location, 1, ucolor[i++ % 3]);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}