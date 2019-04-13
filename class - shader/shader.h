// SHADER VERSION- 1.0.0
// work in Debug x64 mode

#pragma once
#define VERSION_1_0_0

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
	void set1b(const GLchar *name, const GLboolean value) {
		glUniform1i(glGetUniformLocation(ID, name), (GLint)value);
	}
	void set1i(const GLchar *name, const GLint value) {
		glUniform1i(glGetUniformLocation(ID, name), value);
	}
	void set1f(const GLchar *name, const GLfloat value) {
		glUniform1f(glGetUniformLocation(ID, name), value);
	}
	void set1d(const GLchar *name, const GLdouble value) {
		glUniform1d(glGetUniformLocation(ID, name), value);
	}

	void setVec3f(const GLchar *name, const GLfloat x, const GLfloat y, const GLfloat z) {
		glUniform3f(glGetUniformLocation(ID, name), x, y, z);
	}
	void setVec3fv(const GLchar *name, const GLfloat values[3]) {
		glUniform3fv(glGetUniformLocation(ID, name), 1, values);
	}

	void setVec4f(const GLchar *name, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w) {
		glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
	}
	void setVec4fv(const GLchar *name, const GLfloat values[4]) {
		glUniform4fv(glGetUniformLocation(ID, name), 1, values);
	}

	void setMat3fv(const GLchar *name, const GLfloat values[9]) {
		glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, values);
	}
	void setMat3dv(const GLchar *name, const GLdouble values[9]) {
		glUniformMatrix3dv(glGetUniformLocation(ID, name), 1, GL_FALSE, values);
	}

	void setMat4fv(const GLchar *name, const GLfloat values[16]) {
		glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, values);
	}
	void setMat4dv(const GLchar *name, const GLdouble values[16]) {
		glUniformMatrix4dv(glGetUniformLocation(ID, name), 1, GL_FALSE, values);
	}
private:
	GLuint ID;
};

