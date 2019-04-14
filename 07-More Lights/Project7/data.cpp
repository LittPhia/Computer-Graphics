#include "data.h"

using namespace std;
using namespace glm;

const int SCR_WIDTH = 1600;
const int SCR_HEIGHT = 1600;

GLfloat vertices[] = {
	 // positions			// normals				// texture coords
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
							 
	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,		0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,		1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,		1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,		1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,		0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,		0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,		1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
							 
	-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
							 
	-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,		0.0f, 1.0f
};

Material cubes_material = {
	vec4(1.0f, 1.0f, 1.0f, 1.0f),	// bg_color
	64.0f							// shininess
};

Lamp lamps[N_LAMPS];
RotatingCube cubes[N_CUBES];

Camera camera(vec4(0.0f, 0.0f, 3.0f, 1.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f));
GLfloat field_of_view = quarter_pi<GLfloat>();
GLfloat near_plane = 0.0625f;
GLfloat far_plane = 2048.0f;


// ---------------------------------------------------------------------------------------------------
void loadVertexData(GLuint VAO, GLuint VBO, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	glEnableVertexAttribArray(index);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
}

void loadTextureData(const char *tex_filename, GLenum target, GLint level, GLint border, GLenum type) {
	GLint width, height, channels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(tex_filename, (int*)&width, (int*)&height, (int*)&channels, 0);
	if (data != nullptr) {
		GLenum format;
		switch (channels) {
		case 1:		format = GL_RED;	break;
		case 3:		format = GL_RGB;	break;
		case 4:		format = GL_RGBA;	break;
		default:
			cout << "Failed to load texture : coan not recongnize channel >>> " << channels << endl;
			break;
		}
		glTexImage2D(target, level, format, width, height, border, format, type, data);
		glGenerateMipmap(target);
	}
	else {
		cout << "Failed to load texture : can not open file >>> " << tex_filename << endl;
	}
	stbi_image_free(data);
}

void bindTexture(GLuint VAO, GLenum target, GLuint texture, GLenum actived_texture) {
	glBindVertexArray(VAO);
	glActiveTexture(actived_texture);
	glBindTexture(target, texture);

	GLfloat borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindVertexArray(NULL);
}

void initLamps() {
	const vec4 point_light_positions[N_LAMPS - 1] = {
		vec4( 0.7f,  0.2f,  2.0f,  1.0f),
		vec4( 2.3f, -3.3f, -4.0f,  1.0f),
		vec4(-4.0f,  2.0f, -12.0f, 1.0f),
		vec4( 0.0f,  0.0f, -3.0f,  1.0f)
	};
	const vec4 point_light_colors[N_LAMPS - 1] = {
		vec4(0.7f, 0.2f, 1.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f),
		vec4(1.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.6f, 0.6f, 0.8f, 1.0f)
	};


	lamps[N_LAMPS - 1] = {							// SUN
		vec4(0.0f, 1000.0f, 0.0f, 1.0f),	// position
		Light::DirectLightCreator
		(vec4(0.0f, -1.0f, 0.0f, 0.0f),
		vec4(0.7f, 0.7, 0.7f, 1.0f),
		vec4(0.3f, 0.3f, 0.3f, 1.0f),
		vec4(0.5f, 0.5f, 0.5f, 1.0f),
		vec4(1.0f, 1.0f, 1.0f, 1.0f)),
		20.0f								// sizezoom
	};

	for (size_t i = 0; i < N_LAMPS - 1; i++) {
		lamps[i] = {						// LAMPS
			point_light_positions[i],		// position
			Light::PointLightCreator
				(point_light_positions[i],
				Light::attenuation_lib[Light::D65],
				point_light_colors[i],
				vec4(0.1f, 0.1f, 0.1f, 1.0f),
				vec4(0.5f, 0.5f, 0.5f, 1.0f),
				vec4(1.0f, 1.0f, 1.0f, 1.0f)),
			0.1f							// sizezoom
		};
	}
}

void initCubes() {
	const vec4 cubes_position[N_CUBES] = {
		vec4( 0.0f,  0.0f,  0.0f,  1.0f),
		vec4( 2.0f,  5.0f, -15.0f, 1.0f),
		vec4(-1.5f, -2.2f, -2.5f,  1.0f),
		vec4(-3.8f, -2.0f, -12.3f, 1.0f),
		vec4( 2.4f, -0.4f, -3.5f,  1.0f),
		vec4(-1.7f,  3.0f, -7.5f,  1.0f),
		vec4( 1.3f, -2.0f, -2.5f,  1.0f),
		vec4( 1.5f,  2.0f, -2.5f,  1.0f),
		vec4( 1.5f,  0.2f, -1.5f,  1.0f),
		vec4(-1.3f,  1.0f, -1.5f,  1.0f)
	};

	for (size_t i = 0; i < N_CUBES; i++) {
		cubes[i] = {
			&cubes_material,					// cubes_material
			cubes_position[i],					// position
			normalize4(randf(), randf(), randf()),		// rotation_axis
			randf(),									// angle_per_second
			1.0f,								// sizezoom

		};
	}
}

// ---------------------------------------------------------------------------------------------------
vec4 normalize4(GLfloat x, GLfloat y, GLfloat z) {
	return vec4(normalize(vec3(x, y, z)), 0.0f);
}

vec4 normalize4(const vec4 &v) {
	return vec4(normalize(vec3(v)), 0.0f);
}

// ---------------------------------------------------------------------------------------------------
GLfloat randf() {
	srand(rand());
	return (rand() % 20 - 10.0) / (rand() % 5 + 1);
}