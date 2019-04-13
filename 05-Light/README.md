# Light
Light the world up :)

-----------------------------------------------------------
-----------------------------------------------------------

Recently I have found an very interesting phenomenon today. To begin with, I sugguest you to view the code in last project [04-Camera/camera.cpp](https://github.com/LittPhia/Computer-Graphics/blob/master/04-Camera/Project4/camera.cpp) and [04-Camera/Source.cpp](https://github.com/LittPhia/Computer-Graphics/blob/master/04-Camera/Project4/Source.cpp) first:
```cpp
void Camera::keyboardEvent(GLint key, GLint status) {
	static double delta_time = 0.0;
	static double last_time = 0.0;

	delta_time = glfwGetTime() - last_time;
	if (delta_time > 0.02) delta_time = 0.0;
	last_time = glfwGetTime();

	GLfloat stride = Camera::KeyboardSensitivity * (GLfloat)delta_time;
	switch (key) {
		
		blabla...
	
	}
}
```
```cpp
void Camera::keyboardEvent(GLint key, GLint status) {
	static double delta_time = 0.0;
	static double last_time = 0.0;

	delta_time = glfwGetTime() - last_time;
	if (delta_time > 0.02) delta_time = 0.0;
	last_time = glfwGetTime();

	GLfloat stride = Camera::KeyboardSensitivity * (GLfloat)delta_time;
	switch (key) {
		
		blabla...
	
	}
}
```
