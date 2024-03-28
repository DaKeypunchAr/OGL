#include <iostream>

#include "OGL.hpp"
#include "root.hpp"

int main()
{
	std::cout << "Using OGL Version: " << OGL::getVersion() << "\n      OGL Context Version: " << OGL::getContextVersion() << '\n';

	Window window;

	OGL::initializeContext(&window, glm::uvec2(900u), "Test Window");

	OGL::Program colorProgram(ROOT_DIR "/src/Resources/Shaders/Color");

	OGL::VBOConfig vbInfo(0, OGL::BufferUsageHint::StaticDraw, 2 * sizeof(float), { OGL::AttribInfo(0, 2, 0) });
	OGL::VAO vao(3, { 6 }, { vbInfo });
	vao.updateEB(0, { 0, 1, 2 });
	vao.updateVB(0, {
		-0.5f, -0.5f,
		+0.5f, -0.5f,
		+0.0f, +0.5f
		}, 0);

	colorProgram.use();
	colorProgram.uni4f("uColor", glm::vec4(0.7f, 0.34f, 0.89f, 1.0f));
	while (!OGL::shouldClose(&window))
	{
		OGL::clearScreen();

		vao.bind();
		glDrawElements(GL_TRIANGLES, vao.getElementsCount(), GL_UNSIGNED_INT, nullptr);

		OGL::swapBuffers(&window);
		OGL::pollEvents();
	}

	OGL::terminateContext(&window);
}