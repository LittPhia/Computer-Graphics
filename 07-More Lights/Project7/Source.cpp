#include "class - camera/camera.h"
#include "class - shader/shader.h"

#include "data.h"
using namespace glm;

#include <ctime>
#include <iostream>
using namespace std;


void keyboardInput(GLFWwindow *window);
void framebufferSizeCallback(GLFWwindow *window, GLint width, GLint height);
void cursorPosCallback(GLFWwindow* window, GLdouble pos_x, GLdouble pos_y);

// ---------------------------------------------------------------------------------------------------
int main() {
	// Create window ////////////////////////////////////////////////////
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TURE);
#endif

	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
	if (window == nullptr) {
		std::cerr << "Fail to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Load glad pointer manager ------------------------------------- //
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Assemble shader program --------------------------------------- //
	Shader oshader, lshader;
	oshader.loadShaderFromFile(GL_VERTEX_SHADER, "object.vert");
	oshader.loadShaderFromFile(GL_FRAGMENT_SHADER, "object.frag");
	oshader.link();
	
	lshader.loadShaderFromFile(GL_VERTEX_SHADER, "light.vert");
	lshader.loadShaderFromFile(GL_FRAGMENT_SHADER, "light.frag");
	lshader.link();

	// Configure vertex attributes ----------------------------------- //
	GLuint object_VAO, light_VAO;
	GLuint VBO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &light_VAO);
	loadVertexData(light_VAO, VBO, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(0));

	glGenVertexArrays(1, &object_VAO);
	loadVertexData(object_VAO, VBO, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(0));
	loadVertexData(object_VAO, VBO, 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	loadVertexData(object_VAO, VBO, 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

	// Configure texture attributes ----------------------------------- //
	GLuint textures[3];

	glGenTextures(1, textures);
	bindTexture(object_VAO, GL_TEXTURE_2D, textures[0], GL_TEXTURE0);
	loadTextureData("pics/container2.png", GL_TEXTURE_2D, 0, 0, GL_UNSIGNED_BYTE);

	glGenTextures(1, textures + 1);
	bindTexture(object_VAO, GL_TEXTURE_2D, textures[1], GL_TEXTURE1);
	loadTextureData("pics/frame.png", GL_TEXTURE_2D, 0, 0, GL_UNSIGNED_BYTE);

	glGenTextures(1, textures + 2);
	bindTexture(object_VAO, GL_TEXTURE_2D, textures[2], GL_TEXTURE2);
	loadTextureData("pics/matrix.jpg", GL_TEXTURE_2D, 0, 0, GL_UNSIGNED_BYTE);

	// Initalization ------------------------------------------------- //

	const int n_attribs = 11;
	enum ShaderLightAttribes {
		LOCATION,
		POSITION,
		DIRECTION,
		COS_CUTOFF,
		COS_OUTER_CUTOFF,
		COLOR,
		ATTENUATION_0,
		ATTENUATION_1,
		ATTENUATION_2,
		AMBIENT,
		DIFFUSE,
		SPECULAR,
		N_ATTRIBES
	};
	const char *light_attrib_name_list[N_ATTRIBES] = {
		"lights[N].location",
		"lights[N].position",
		"lights[N].direction",
		"lights[N].cos_cutoff",
		"lights[N].cos_outer_cutoff",
		"lights[N].color",
		"lights[N].attenuation[0]",
		"lights[N].attenuation[1]",
		"lights[N].attenuation[2]",
		"lights[N].ambient",
		"lights[N].diffuse",
		"lights[N].specular"
	};
	const int PLACE_N = 7;

	char light_attrib_name[N_ATTRIBES][32];
	for (size_t i = 0; i < N_ATTRIBES; i++) {
		const char *p = light_attrib_name_list[i];
		char *q = light_attrib_name[i];
		while (*p != '\0') {
			*q = *p;
			p++, q++;
		}
		*q = '\0';
	}

	initLamps();
	initCubes();
	
	// Visualize ----------------------------------------------------- //
	glEnable(GL_DEPTH_TEST);

	mat4 light_model(1.0f);
	mat4 object_model(1.0f);
	mat4 view(1.0f);
	mat4 projection = perspective(field_of_view, (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, near_plane, far_plane);

	lshader.activate();
	lshader.setMat4fv("projection", projection);

	oshader.activate();
	oshader.setMat4fv("projection", projection);
	oshader.setVec4fv("material.color", cubes_material.bg_color);
	oshader.set1i("material.diffuse", 0);
	oshader.set1i("material.specular", 1);
	oshader.set1i("material.emission", 2);
	oshader.set1f("material.shininess", cubes_material.shininess);
	
	while (!glfwWindowShouldClose(window)) {
		keyboardInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = camera.getViewMatrix();

		// Draw lamps
		lshader.activate();
		lshader.setMat4fv("view", view);
		for (size_t i = 0; i < N_LAMPS; i++) {
			lshader.activate();
			lshader.setVec4fv("light_color", lamps[i].light.getColor());
			glBindVertexArray(light_VAO);

			light_model = translate(mat4(1.0f), vec3(lamps[i].position));
			light_model = rotate(light_model, (GLfloat)glfwGetTime(), vec3(1.0f));
			light_model = scale(light_model, vec3(lamps[i].sizezoom));
			lshader.setMat4fv("model", light_model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			light_model = translate(mat4(1.0f), vec3(lamps[i].position));
			light_model = rotate(light_model, (GLfloat)glfwGetTime(), vec3(1.0f, -1.0f, 1.0f));
			light_model = scale(light_model, vec3(lamps[i].sizezoom));
			lshader.setMat4fv("model", light_model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			light_model = translate(mat4(1.0f), vec3(lamps[i].position));
			light_model = rotate(light_model, (GLfloat)glfwGetTime(), vec3(-1.0f, -1.0f, 1.0f));
			light_model = scale(light_model, vec3(lamps[i].sizezoom));
			lshader.setMat4fv("model", light_model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			light_model = translate(mat4(1.0f), vec3(lamps[i].position));
			light_model = rotate(light_model, (GLfloat)glfwGetTime(), vec3(-1.0f, 1.0f, 1.0f));
			light_model = scale(light_model, vec3(lamps[i].sizezoom));
			lshader.setMat4fv("model", light_model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		// Draw Objects
		oshader.activate();
		oshader.setMat4fv("view", view);
		oshader.setVec4fv("viewer_pos", camera.getPosition());

		for (char i = 0; i < N_LAMPS; i++) {
			light_attrib_name[LOCATION][PLACE_N] = '0' + i;
			oshader.setVec4fv(light_attrib_name[LOCATION], lamps[i].position);

			light_attrib_name[POSITION][PLACE_N] = '0' + i;
			oshader.setVec4fv(light_attrib_name[POSITION], lamps[i].light.getPosition());

			light_attrib_name[DIRECTION][PLACE_N] = '0' + i;
			oshader.setVec4fv(light_attrib_name[DIRECTION], lamps[i].light.getDirection());
			
			light_attrib_name[COS_CUTOFF][PLACE_N] = '0' + i;
			oshader.set1f(light_attrib_name[COS_CUTOFF], (GLfloat)cos(lamps[i].light.getCutoff()));
			
			light_attrib_name[COS_OUTER_CUTOFF][PLACE_N] = '0' + i;
			oshader.set1f(light_attrib_name[COS_OUTER_CUTOFF], (GLfloat)cos(lamps[i].light.getOuterCutoff()));
			
			light_attrib_name[COLOR][PLACE_N] = '0' + i;
			oshader.setVec4fv(light_attrib_name[COLOR], lamps[i].light.getColor());

//			cout << light_attrib_name[DIRECTION] << ' '
//				<< lamps[i].light.getDirection().x << ' '
//				<< lamps[i].light.getDirection().y << ' '
//				<< lamps[i].light.getDirection().z << endl;

			light_attrib_name[ATTENUATION_0][PLACE_N] = '0' + i;
			oshader.set1f(light_attrib_name[ATTENUATION_0], lamps[i].light.getAttenuation3f()[0]);

			light_attrib_name[ATTENUATION_1][PLACE_N] = '0' + i;
			oshader.set1f(light_attrib_name[ATTENUATION_1], lamps[i].light.getAttenuation3f()[1]);

			light_attrib_name[ATTENUATION_2][PLACE_N] = '0' + i;
			oshader.set1f(light_attrib_name[ATTENUATION_2], lamps[i].light.getAttenuation3f()[2]);

			light_attrib_name[AMBIENT][PLACE_N] = '0' + i;
			oshader.setVec4fv(light_attrib_name[AMBIENT], lamps[i].light.getAmbient());

			light_attrib_name[DIFFUSE][PLACE_N] = '0' + i;
			oshader.setVec4fv(light_attrib_name[DIFFUSE], lamps[i].light.getDiffuse());

			light_attrib_name[SPECULAR][PLACE_N] = '0' + i;
			oshader.setVec4fv(light_attrib_name[SPECULAR], lamps[i].light.getSpecular());
		}
		
		Light spotlight = camera.spotlight();
		oshader.setVec4fv("camera_spotlight.location", camera.getPosition());
		oshader.setVec4fv("camera_spotlight.position", spotlight.getPosition());
		oshader.setVec4fv("camera_spotlight.direction", spotlight.getDirection());
		oshader.set1f("camera_spotlight.cos_cutoff", (GLfloat)cos(spotlight.getCutoff()));
		oshader.set1f("camera_spotlight.cos_outer_cutoff", (GLfloat)cos(spotlight.getOuterCutoff()));
		oshader.setVec4fv("camera_spotlight.color", spotlight.getColor());
		oshader.set1f("camera_spotlight.attenuation[0]", spotlight.getAttenuation3f()[0]);
		oshader.set1f("camera_spotlight.attenuation[1]", spotlight.getAttenuation3f()[1]);
		oshader.set1f("camera_spotlight.attenuation[2]", spotlight.getAttenuation3f()[2]);
		oshader.setVec4fv("camera_spotlight.ambient", spotlight.getAmbient());
		oshader.setVec4fv("camera_spotlight.diffuse", spotlight.getDiffuse());
		oshader.setVec4fv("camera_spotlight.specular", spotlight.getSpecular());

		for (size_t i = 1; i < 10; i++) {
			object_model = translate(mat4(1.0f), vec3(cubes[i].position));
			object_model = rotate(object_model, cubes[i].angle_per_second * (GLfloat)glfwGetTime(), vec3(cubes[i].rotation_axis));
			object_model = scale(object_model, vec3(cubes[i].sizezoom));
			oshader.setMat4fv("model", object_model);
			oshader.setMat4fv("norm_matrix", inverse(transpose(object_model)));

			oshader.set1f("material.emission_scale", 0.0f);
			glBindVertexArray(object_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glBindVertexArray(NULL);

	// Shut down ////////////////////////////////////////////////////////
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &object_VAO);
	glDeleteVertexArrays(1, &light_VAO);

	glfwTerminate();
	return 0;
}


// ---------------------------------------------------------------------------------------------------
void keyboardInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	static double proc_time = 0.0;
	static double last_time = 0.0;
	proc_time = glfwGetTime() - last_time;
	last_time = glfwGetTime();

	int num = glfwGetKey(window, GLFW_KEY_W)
		+ glfwGetKey(window, GLFW_KEY_S)
		+ glfwGetKey(window, GLFW_KEY_A)
		+ glfwGetKey(window, GLFW_KEY_D)
		+ glfwGetKey(window, GLFW_KEY_SPACE)
		+ glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
	proc_time /= sqrt(num);


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.keyboardEvent(Camera::FORWARD, Camera::PRESSED, (GLfloat)proc_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.keyboardEvent(Camera::BACKWARD, Camera::PRESSED, (GLfloat)proc_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.keyboardEvent(Camera::LEFT, Camera::PRESSED, (GLfloat)proc_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.keyboardEvent(Camera::RIGHT, Camera::PRESSED, (GLfloat)proc_time);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.keyboardEvent(Camera::UP, Camera::PRESSED, (GLfloat)proc_time);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.keyboardEvent(Camera::DOWN, Camera::PRESSED, (GLfloat)proc_time);

}

// ---------------------------------------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow *window, GLint width, GLint height) {
	if (width > height) glViewport((width - height) / 2, 0, height, height);
	else glViewport(0, (height - width) / 2, width, width);
}

// ---------------------------------------------------------------------------------------------------
void cursorPosCallback(GLFWwindow *window, GLdouble pos_x, GLdouble pos_y) {
	static GLdouble last_x = 0.0, last_y = 0.0;
	static bool is_first_call = true;

	if (is_first_call) {
		last_x = pos_x, last_y = pos_y;
		is_first_call = false;
	}
	GLdouble offset_X = last_x - pos_x,
		offset_Y = pos_y - last_y;
	last_x = pos_x;
	last_y = pos_y;

	camera.mouseMovementEvent((GLfloat)offset_X, (GLfloat)offset_Y);
}