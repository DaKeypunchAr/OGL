#include <iostream>

#include "OGL.hpp"
#include "root.hpp"

int main()
{
	std::cout << "Using OGL Version: " << OGL::getVersion() << "\n      OGL Context Version: " << OGL::getContextVersion() << '\n';

	Window window;

	OGL::initializeContext(&window, glm::uvec2(900u), "Test Window");

	OGL::Program colorProgram(ROOT_DIR "/src/Resources/Shaders/Color");

	while (!OGL::shouldClose(&window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		OGL::swapBuffers(&window);
		OGL::pollEvents();
	}

	OGL::terminateContext(&window);
}