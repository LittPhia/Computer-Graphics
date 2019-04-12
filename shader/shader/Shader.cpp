#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;


Shader::Shader(const GLchar *vertex_shader_code_path, const GLchar *fragment_shader_code_path) {
	ID = glCreateProgram();
	loadShaderFromFile(GL_VERTEX_SHADER, vertex_shader_code_path);
	loadShaderFromFile(GL_FRAGMENT_SHADER, fragment_shader_code_path);
}


void Shader::loadShader(GLenum type, const GLchar *shader_code) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shader_code, NULL);
	glCompileShader(shader);
	checkCompileErrors(shader, type);

	glAttachShader(ID, shader);
	glDeleteShader(shader);
}

void Shader::loadShaderFromFile(GLenum type, const GLchar *shader_code_path) {
	GLuint shader = glCreateShader(type);

	string vs_code;
	ifstream vs_file;

	vs_file.exceptions(ifstream::failbit | ifstream::badbit);
	try {
		vs_file.open(shader_code_path);

		stringstream vs_stream;
		vs_stream << vs_file.rdbuf();
		vs_file.close();

		vs_code = vs_stream.str();
	}
	catch (ifstream::failure error) {
		cerr << "ERROR::SHADER::FILE_READ_NOT_SUCCESSFUL" << endl
			<< error.what() << endl;
	}
	const GLchar* src = (GLchar *)vs_code.c_str();

	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	checkCompileErrors(shader, type);

	glAttachShader(ID, shader);
	glDeleteShader(shader);
}

void Shader::link() {
	glLinkProgram(ID);
	checkCompileErrors(ID, GL_PROGRAM);
}

void Shader::activate() {
	glUseProgram(ID);
}

void Shader::deactivate() {
	glUseProgram(NULL);
}

void Shader::checkCompileErrors(GLuint shader, GLenum type) {
	int success;
	char infoLog[512];
	if (type != GL_PROGRAM) {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << endl
				<< infoLog << "\n -- --------------------------------------------------- -- " << endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << endl
				<< infoLog << "\n -- --------------------------------------------------- -- " << endl;
		}
	}
}

