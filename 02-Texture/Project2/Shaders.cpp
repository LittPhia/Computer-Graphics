#include <fstream>
#include <sstream>

#include "Shaders.h"
#include "stb_image.h"

using namespace std;

Shaders::Shaders() {
	ID = glCreateProgram();
}

Shaders::Shaders(const GLchar *vertex_shader_code_path, const GLchar *fragment_shader_code_path) {
	ID = glCreateProgram();
	assembleVertexShader(vertex_shader_code_path);
	assembleFragmentShader(fragment_shader_code_path);
}

Shaders::~Shaders() {
	glDeleteProgram(ID);
}

void Shaders::assembleVertexShader(const GLchar *vertex_shader_code_path) {
	string vs_code;
	ifstream vs_file;

	vs_file.exceptions(ifstream::failbit | ifstream::badbit);
	try {
		vs_file.open(vertex_shader_code_path);

		stringstream vs_stream;
		vs_stream << vs_file.rdbuf();
		vs_file.close();

		vs_code = vs_stream.str();
	}
	catch (ifstream::failure error) {
		cerr << "ERROR::SHADER::FILE_READ_NOT_SUCCESSFUL" << endl
			<< error.what() << endl;
		return;
	}
	const char* vsrc = vs_code.c_str();

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vsrc, NULL);
	glCompileShader(vertex_shader);
	checkCompileErrors(vertex_shader, "VERTEX");

	glAttachShader(ID, vertex_shader);
	glDeleteShader(vertex_shader);
}

void Shaders::assembleFragmentShader(const GLchar *fragment_shader_code_path) {
	string fs_code;
	ifstream fs_file;

	fs_file.exceptions(ifstream::failbit | ifstream::badbit);
	try {
		fs_file.open(fragment_shader_code_path);

		stringstream fs_stream;
		fs_stream << fs_file.rdbuf();
		fs_file.close();
		fs_code = fs_stream.str();
	}
	catch (ifstream::failure error) {
		cerr << "ERROR::SHADER::FILE_READ_NOT_SUCCESSFUL" << endl
			<< error.what() << endl;
		return;
	}
	const char* fsrc = fs_code.c_str();

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fsrc, NULL);
	glCompileShader(fragment_shader);
	checkCompileErrors(fragment_shader, "FRAGMENT");

	glAttachShader(ID, fragment_shader);
	glDeleteShader(fragment_shader);
}

void Shaders::link() {
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
}

void Shaders::activate() {
	glUseProgram(ID);
}

void Shaders::deactivate() {
	glUseProgram(NULL);
}

GLuint Shaders::getProgramID() {
	return ID;
}

void Shaders::setUniformBool(const string &name, GLboolean value) {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (GLint)value);
}

void Shaders::setUniformInt(const string &name, GLint value) {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shaders::setUniformFloat(const string &name, GLfloat value) {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shaders::setUniformDouble(const string &name, GLdouble value) {
	glUniform1d(glGetUniformLocation(ID, name.c_str()), value);
}

void Shaders::checkCompileErrors(GLuint shader, string type) {
	int suc;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &suc);
		if (!suc) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << endl
				<< infoLog << "\n -- --------------------------------------------------- -- " << endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &suc);
		if (!suc) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << endl
				<< infoLog << "\n -- --------------------------------------------------- -- " << endl;
		}
	}
}
