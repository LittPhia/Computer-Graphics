#include "camera.h"
#include <GLFW/glfw3.h>

using namespace glm;

const vec4 InitPosition(0.0f, 0.0f, 1.0f, 1.0f);
const vec4 InitTarget(0.0f, 0.0f, 0.0f, 1.0f);
const GLfloat Camera::KeyboardSensitivity = 4.0f;
GLfloat Camera::MouseSensitivity = 0.0006f;

Camera::Camera() {
	mode_ = FRONT_VEC4;
	position_ = InitPosition;
	front_ = vec4(normalize(vec3(InitPosition - InitTarget)), 0.0f);
	up_ = vec4(0.0f, 1.0f, 0.0f, 0.0f);
}

Camera::Camera(vec4 camera_pos, vec4 target) {
	mode_ = FRONT_VEC4;
	position_ = camera_pos;
	front_ = vec4(normalize(vec3(camera_pos - target)), 0.0f);
	up_ = vec4(0.0f, 1.0f, 0.0f, 0.0f);
}

mat4 Camera::getViewMatrix() {
	switchMode(FRONT_VEC4);
	return lookAt(vec3(position_), vec3(position_ - front_), vec3(up_));
}

vec4 Camera::getPosition() {
	return vec4(position_);
}

void Camera::keyboardEvent(Movement direction, GLint status, GLfloat proc_time) {
	GLfloat stride = Camera::KeyboardSensitivity * (GLfloat)proc_time;
	cameraTranslate(direction, stride);
}

void Camera::mouseMovementEvent(GLfloat Offset_X, GLfloat Offset_Y, GLboolean pitch_constrain) {
	GLfloat stride_pitch = Offset_Y * Camera::MouseSensitivity;
	GLfloat stride_yaw = Offset_X * Camera::MouseSensitivity;
	cameraRotate(stride_pitch, stride_yaw);
}

/*
						+Y|
						  |
						  |
						  |          * Angle
						  |   /    * *
						  |  /   *   *
						  | /  *     *
						  |/ *       *
	----------------------*----------*-------------
						 /|  #*      * *         +X
						/#|#     *   **
					   /  | Yaw      *
				   +Z /   |
						  |
						  |
						  |
						  |
*/

void Camera::switchMode(Mode mode) {
	if (mode_ == mode) return;
	else if (mode_ == FRONT_VEC4 && mode == EULER_ANGLE) {
		GLfloat Y = front_.y,
			Y_on_XZ = front_.z / sqrt(1 - front_.y * front_.y);
		if (Y > 1.0f)				Y = 1.0f;
		else if (Y < -1.0f)			Y = -1.0f;

		if (Y_on_XZ > 1.0f)			Y_on_XZ = 1.0f;
		else if (Y_on_XZ < -1.0f)	Y_on_XZ = -1.0f;

		yaw_ = glm::sign(front_.x) * acos(Y_on_XZ);
		pitch_ = asin(front_.y);
	}
	else {
		front_ = vec4(normalize(vec3(cos(pitch_) * sin(yaw_), sin(pitch_), cos(pitch_) * cos(yaw_))), 0.0f);
	}
	mode_ = mode;
}

void Camera::cameraRotate(GLfloat stride_pitch, GLfloat stride_yaw) {
	switchMode(EULER_ANGLE);
	pitch_ += stride_pitch;
	yaw_ += stride_yaw;

	if (pitch_ > half_pi<GLfloat>() - 0.01f)
		pitch_ = half_pi<GLfloat>() - 0.01f;
	else if (pitch_ < -half_pi<GLfloat>() + 0.01f)
		pitch_ = -half_pi<GLfloat>() + 0.01f;
}

void Camera::cameraTranslate(Movement direction, GLfloat stride) {
	switchMode(FRONT_VEC4);

	vec4 right;
	right = vec4(normalize(cross(vec3(up_), vec3(front_))), 0.0f);
	switch (direction) {
	case Camera::FORWARD:
		position_ -= front_ * stride;
		break;
	case Camera::BACKWARD:
		position_ += front_ * stride;
		break;
	case Camera::LEFT:
		position_ -= right * stride;
		break;
	case Camera::RIGHT:
		position_ += right * stride;
	}
}