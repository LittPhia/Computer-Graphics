# Light
Light the world up :)

I have found an very interesting phenomenon today. To begin with, I sugguest you to view the code in [04-Camera](https://github.com/LittPhia/Computer-Graphics/blob/master/04-Camera/Project4/camera.cpp) first:
```cpp
void Camera::keyboardEvent(GLint key, GLint status) {
	static double delta_time = 0.0;
	static double last_time = 0.0;

	delta_time = glfwGetTime() - last_time;
	if (delta_time > 0.02) delta_time = 0.0;		// in case of 
	last_time = glfwGetTime();

	GLfloat stride = Camera::KeyboardSensitivity * (GLfloat)delta_time;
	switch (key) {
	case GLFW_KEY_UP:
		cameraTranslate(Camera::FORWARD, stride);
		break;
	case GLFW_KEY_DOWN:
		cameraTranslate(Camera::BACKWARD, stride);
		break;
	case GLFW_KEY_LEFT:
		cameraTranslate(Camera::LEFT, stride);
		break;
	case GLFW_KEY_RIGHT:
		cameraTranslate(Camera::RIGHT, stride);
	}
}
```
