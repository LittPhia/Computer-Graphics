/*
	Version - 1.0.0
	work in Debug x64 mode
*/

#pragma once
#define VERSION_1_0_0

#ifdef DEBUG_X64
#pragma comment(lib, "shader64d.lib")

#elif defined RELEASE_X64
#pragma comment(lib, "shader64r.lib")

#elif defined DEBUG_X86
#pragma comment(lib, "shader86d.lib")

#elif defined RELEASE_X86
#pragma comment(lib, "shader86r.lib")

#else
#pragma comment(lib, "shader.lib")

#endif


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3.lib")

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
	void set1b(const GLchar *name, GLboolean value) {
		glUniform1i(glGetUniformLocation(ID, name), (GLint)value);
	}
	void set1i(const GLchar *name, GLint value) {
		glUniform1i(glGetUniformLocation(ID, name), value);
	}
	void set1f(const GLchar *name, GLfloat value) {
		glUniform1f(glGetUniformLocation(ID, name), value);
	}
	void set1d(const GLchar *name, GLdouble value) {
		glUniform1d(glGetUniformLocation(ID, name), value);
	}

	void setVec3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z) {
		glUniform3f(glGetUniformLocation(ID, name), x, y, z);
	}
	void setVec3fv(const GLchar *name, GLfloat values[3]) {
		glUniform3fv(glGetUniformLocation(ID, name), 1, values);
	}

	void setVec4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
		glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
	}
	void setVec4fv(const GLchar *name, GLfloat values[4]) {
		glUniform4fv(glGetUniformLocation(ID, name), 1, values);
	}

	void setMat4fv(const GLchar *name, GLfloat values[16]) {
		glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, values);
	}
	void setMat4dv(const GLchar *name, GLdouble values[16]) {
		glUniformMatrix4dv(glGetUniformLocation(ID, name), 1, GL_FALSE, values);
	}
private:
	GLuint ID;
};

