#ifndef _OGL_HPP
#define _OGL_HPP

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "GL/glew.h"

#include <string>

typedef GLFWwindow* Window;

#include "version.hpp"

namespace OGL
{
	constexpr static const char* getVersion() { return _OGL_VERSION; }
	constexpr static const char* getContextVersion() { return "4.5"; }
}

#include "Program/Program.hpp"
#include "VAO/VAO.hpp"
#include "Texture/Texture.hpp"

namespace OGL
{
#pragma region OGL/Debug Callbacks
	void GLFWDebugCallback(int errCode, const char* desc);
	void __stdcall OGLDebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* msg, const void* userParam);
#pragma endregion
#pragma region Init/Terminate
	void initializeContext(Window* window, glm::uvec2 windowDimensions, const std::string& title);
	void terminateContext(Window* window);
#pragma endregion
#pragma region OGL/GLFW Wrappers
	bool shouldClose(Window* window);
	void swapBuffers(Window* window);
	void pollEvents();
	void clearColor(glm::vec4 clearColor);
	void clearScreen();
#pragma endregion
#pragma region Renders
	void renderVAO(const VAO& vao);
#pragma endregion
}

#endif