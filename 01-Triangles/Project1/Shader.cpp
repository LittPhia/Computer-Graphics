#include "Shader.h"

using namespace std;

Shader::Shader() {}

Shader::Shader(const GLchar *vshader_code_path, const GLchar *fshader_code_path) {
	assemble(vshader_code_path, fshader_code_path);
}

void Shader::assemble(const GLchar *vshader_code_path, const GLchar *fshader_code_path) {
	string vertex_code, fragment_code;
	ifstream vshader_file, fshader_file;

	vshader_file.exceptions(ifstream::failbit | ifstream::badbit);
	fshader_file.exceptions(ifstream::failbit | ifstream::badbit);

	try {
		vshader_file.open(vshader_code_path);
		fshader_file.open(fshader_code_path);

		stringstream vshader_stream, fshader_stream;
		vshader_stream << vshader_file.rdbuf();
		fshader_stream << fshader_file.rdbuf();

		vshader_file.close();
		fshader_file.close();

		vertex_code = vshader_stream.str();
		fragment_code = fshader_stream.str();
	}
	catch (ifstream::failure error) {
		cerr << "ERROR::SHADER::FILE_READ_NOT_SUCCESSFUL" << endl;
	}
	const char* vshader_code = vertex_code.c_str();
	const char* fshader_code = fragment_code.c_str();



	GLuint vertex_shader, fragment_shader;

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vshader_code, NULL);
	glCompileShader(vertex_shader);
	checkCompileErrors(vertex_shader, "VERTEX");


	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fshader_code, NULL);
	glCompileShader(fragment_shader);
	checkCompileErrors(fragment_shader, "FRAGMENT");


	ID = glCreateProgram();
	glAttachShader(ID, vertex_shader);
	glAttachShader(ID, fragment_shader);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void Shader::activate() {
	glUseProgram(ID);
}

void Shader::set_bool(const string &name, GLboolean value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (GLint)value);
}

void Shader::set_int(const string &name, GLint value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_float(const string &name, GLfloat value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_double(const string &name, GLdouble value) const {
	glUniform1d(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::checkCompileErrors(unsigned int shader, string type) {
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
