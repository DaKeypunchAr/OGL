#include <iostream>

#include "OGL.hpp"
#include "root.hpp"

int main()
{
	std::cout << "Using OGL Version: " << OGL::getVersion() << "\n      OGL Context Version: " << OGL::getContextVersion() << '\n';

	Window window;

	OGL::initializeContext(&window, glm::uvec2(900u), "Test Window");

	OGL::Program colorProgram(ROOT_DIR "/src/Resources/Shaders/Color");
	OGL::Program textureProgram(ROOT_DIR "/src/Resources/Shaders/Texture");

	OGL::VBOConfig posVB(0, 8, 2 * sizeof(float), { OGL::AttribInfo(0, 2, 0) }, OGL::BufferUsageHint::StaticDraw);
	OGL::VBOConfig texVB(1, 8, 2 * sizeof(float), { OGL::AttribInfo(1, 2, 0) }, OGL::BufferUsageHint::StaticDraw);
	OGL::EBOConfig ebInfo(6); 
	OGL::VAO vao({ posVB, texVB }, std::optional<OGL::EBOConfig>(ebInfo));
	vao.updateVB(0, 0, {
		+0.5f, +0.5f,
		+0.5f, -0.5f,
		-0.5f, -0.5f,
		-0.5f, +0.5f,
	});
	vao.updateVB(1, 0, {
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
	});
	vao.updateEB(0, std::vector<unsigned char>({ 0, 1, 2, 0, 2, 3 }));

	OGL::Texture2D monalisa(ROOT_DIR "/src/Resources/Textures/monalisa.jpg", OGL::TextureFilters(OGL::ScaleFilter::LINEAR, OGL::WrapFilter::MIRROR_REPEAT));
	monalisa.bind(0);
	textureProgram.use();
	textureProgram.uni1i("tex0", 0);

	while (!OGL::shouldClose(&window))
	{
		OGL::clearScreen();

		OGL::renderVAO(vao);

		OGL::swapBuffers(&window);
		OGL::pollEvents();
	}

	OGL::terminateContext(&window);
}