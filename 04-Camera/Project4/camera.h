#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

extern const glm::vec4 InitPosition;
extern const glm::vec4 InitTarget;

class Camera {
public:
	static const GLfloat KeyboardSensitivity;
	static GLfloat MouseSensitivity;

	enum Movement {
		FORWARD,
		BACKWARD,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	enum Mode {
		FRONT_VEC4,
		EULER_ANGLE
	};

public:
	Camera();

	glm::mat4 getViewMatrix();
	void keyboardEvent(GLint key, GLint status);
	void mouseMovementEvent(GLfloat Offset_X, GLfloat Offset_Y, GLboolean pitch_constrain = GL_TRUE);

private:
	void switchMode(Mode mode);
	void cameraRotate(GLfloat stride_pitch, GLfloat stride_yaw);
	void cameraTranslate(Movement direction, GLfloat stride);

private:
	Mode mode_;
	glm::vec4 position_;

	glm::vec4 front_;
	glm::vec4 up_;

	GLfloat pitch_;
	GLfloat yaw_;
	GLfloat roll_;
};
