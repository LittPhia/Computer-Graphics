#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shaders {
public:
	Shaders();
	Shaders(const GLchar *vertex_shader_code_path, const GLchar *fragment_shader_code_path);
	~Shaders();

	void assembleVertexShader(const GLchar *vertex_shader_code_path);
	void assembleFragmentShader(const GLchar *fragment_shader_code_path);
	

	void link();
	void activate();
	void deactivate();
	GLuint getProgramID();

	void setUniformBool(const std::string &name, GLboolean value);
	void setUniformInt(const std::string &name, GLint value);
	void setUniformFloat(const std::string &name, GLfloat value);
	void setUniformDouble(const std::string &name, GLdouble value);

private:
	void checkCompileErrors(GLuint shader, std::string type);

private:
	GLuint ID;
};

