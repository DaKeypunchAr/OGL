#ifndef _OGL_HPP
#define _OGL_HPP

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "GL/glew.h"

#ifdef _OGL_DEBUG
#include <iostream>
#endif

#include <string>

typedef GLFWwindow* Window;

namespace OGL
{
	void GLFWDebugCallback(int errCode, const char* desc);
	void __stdcall OGLDebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* msg, const void* userParam);

	void initializeContext(Window* window, glm::uvec2 windowDimensions, const std::string& title);
	void terminateContext(Window* window);
	bool shouldClose(Window* window);
	void swapBuffers(Window* window);
	void pollEvents();
}

#endif