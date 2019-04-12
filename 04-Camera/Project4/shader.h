#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Shader {
public:
	Shader() {
		ID = glCreateProgram();
	}
	Shader(const GLchar *vertex_shader_code_path, const GLchar *fragment_shader_code_path);
	~Shader() {
		glDeleteProgram(ID);
	}

public:
	void loadShader(GLenum type, const GLchar *shader_code);
	void loadShaderFromFile(GLenum type, const GLchar *shader_code_path);

	void link();
	void activate();
	void deactivate();
	GLuint getProgramID() {
		return ID;
	}

private:
	void checkCompileErrors(GLuint shader, GLenum type);

public:
	void setUniform1b(const GLchar *name, GLboolean value) {
		glUniform1i(glGetUniformLocation(ID, name), (GLint)value);
	}
	void setUniform1i(const GLchar *name, GLint value) {
		glUniform1i(glGetUniformLocation(ID, name), value);
	}
	void setUniform1f(const GLchar *name, GLfloat value) {
		glUniform1f(glGetUniformLocation(ID, name), value);
	}
	void setUniform1d(const GLchar *name, GLdouble value) {
		glUniform1d(glGetUniformLocation(ID, name), value);
	}
	void setUniformMatrix4fv(const GLchar *name, GLfloat value[16]) {
		glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, value);
	}
	void setUniformMatrix4dv(const GLchar *name, GLdouble value[16]) {
		glUniformMatrix4dv(glGetUniformLocation(ID, name), 1, GL_FALSE, value);
	}
private:
	GLuint ID;
};