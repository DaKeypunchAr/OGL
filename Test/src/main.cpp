#include <iostream>

#include "OGL.hpp"

int main()
{
	Window window;

	OGL::initializeContext(&window, glm::uvec2(900u), "Test Window");

	while (!OGL::shouldClose(&window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		OGL::swapBuffers(&window);
		OGL::pollEvents();
	}

	OGL::terminateContext(&window);
}