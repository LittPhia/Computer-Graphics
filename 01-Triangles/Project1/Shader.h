#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	Shader();
	Shader(const GLchar* vshader_code_path, const GLchar* fshader_code_path);

	void assemble(const GLchar* vshader_code_path, const GLchar* fshader_code_path);
	void activate();
	void set_bool(const std::string &name, GLboolean value) const;
	void set_int(const std::string &name, GLint value) const;
	void set_float(const std::string &name, GLfloat value) const;
	void set_double(const std::string &name, GLdouble value) const;

private:
	void checkCompileErrors(unsigned int shader, std::string type);

public:
	GLint ID;
};

