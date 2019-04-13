# Light
Light the world up :)

-----------------------------------------------------------
-----------------------------------------------------------

Recently I have found an very interesting phenomenon today. To begin with, I sugguest you to view some codes in last project [04-Camera/camera.cpp](https://github.com/LittPhia/Computer-Graphics/blob/master/04-Camera/Project4/camera.cpp) and [04-Camera/Source.cpp](https://github.com/LittPhia/Computer-Graphics/blob/master/04-Camera/Project4/Source.cpp) first:
> camera.cpp
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
> Source.cpp
```cpp
void keyboard_input(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.keyboardEvent(GLFW_KEY_UP, GLFW_PRESS);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.keyboardEvent(GLFW_KEY_DOWN, GLFW_PRESS);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.keyboardEvent(GLFW_KEY_LEFT, GLFW_PRESS);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.keyboardEvent(GLFW_KEY_RIGHT, GLFW_PRESS);
}
```

If you successfully compile the project, then run the .exe and you may get a window like this:

![window first view](https://github.com/LittPhia/Computer-Graphics/blob/master/05-Light/Something%20wired/window%20first%20view.png)

You can move the camera by simply tap up-down-left-right key, or change the viewport angle by press mouse left button then drag. It could be exciting to the one who just write this and comfirm the correctness, but boring to the others, surely. However, as you did this, you may notice that if you tap two key simultaneously, you are not moving to the combined direction, but just one direction of them, and the other key looks disabled, or in other word, overwhelmed.

![window first view](https://github.com/LittPhia/Computer-Graphics/blob/master/05-Light/Something%20wired/why%20I%20am%20just%20moving%20forward.png)


After a period of endless testing, I eventualy found something. In this camera system, if you tap two key at the same time(for example, up and left key tapped), function 'camera::keyboardEvent' will be called twice:
`camera.keyboardEvent(GLFW_KEY_UP, GLFW_PRESS)`
and
`camera.keyboardEvent(GLFW_KEY_LEFT, GLFW_PRESS)`
in two `if` sentences, as we expected to combine directions. Now have a CLEAR look at how we calculate the stride in each movement:
```cpp
delta_time = glfwGetTime() - last_time;
if (delta_time > 0.02) delta_time = 0.0;
last_time = glfwGetTime();

GLfloat stride = Camera::KeyboardSensitivity * (GLfloat)delta_time;
```

You might not be able to find the problem in first shot. Yet by marking the time this function called, you will see
`CALLED_TIME(camera.keyboardEvent(GLFW_KEY_UP, GLFW_PRESS)) - CALLED_TIME(camera.keyboardEvent(GLFW_KEY_LEFT, GLFW_PRESS))`
is much smaller than
`CALLED_TIME(camera.keyboardEvent(GLFW_KEY_LEFT, GLFW_PRESS)) - CALLED_TIME(camera.keyboardEvent(GLFW_KEY_UP, GLFW_PRESS))`.

Soon you will realize it is because between `KEY_UP` and `KEY_LEFT` there are just some simple `if`s and a `camera.keyboardEvent`, while between `KEY_LEFT` and next `KEY_UP` there is a WHOLE mainloop. And then, `KEY_UP`'s `delta_time` will naturally much more greater than `KEY_LEFT`'s. And? As you see.

In this project I solved this. Thank me.



