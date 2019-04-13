//	CAMERA VERSION - 1.0.0
//	work in Debug x64 mode

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
		FORWARD,	BACKWARD,
		UP,			DOWN,
		LEFT,		RIGHT
	};

	enum Mode { FRONT_VEC4, EULER_ANGLE };
	enum KeyStauts { PRESSED, RELEASED };

public:
	Camera();
	Camera(glm::vec4 camera_pos, glm::vec4 target);

	glm::mat4 getViewMatrix();
	glm::vec4 getPosition();
	void keyboardEvent(Camera::Movement direction, GLint status, GLfloat proc_time);
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
