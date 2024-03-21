#include "OGL.hpp"

namespace OGL
{
	void initializeContext(Window* window, glm::uvec2 windowDimensions, const std::string& title)
	{
		if (!glfwInit())
		{
#ifdef _OGL_DEBUG
			std::cout << "GLFW Failed to Initialize!\n";
			exit(EXIT_FAILURE);
#endif
#ifdef _OGL_RELEASE
			exit(EXIT_FAILURE);
#endif
		}

		glfwSetErrorCallback(GLFWDebugCallback);

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

		(*window) = glfwCreateWindow(windowDimensions.x, windowDimensions.y, title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(*window);

		if (glewInit())
		{
#ifdef _OGL_DEBUG
			std::cout << "GLEW Failed to Initialize!\n";
			exit(EXIT_FAILURE);
#endif
#ifdef _OGL_RELEASE
			exit(EXIT_FAILURE);
#endif
		}

		glEnable(GL_DEBUG_CALLBACK_FUNCTION);
		glDebugMessageCallback(OGLDebugCallback, nullptr);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}
	void terminateContext(Window* window)
	{
		glfwDestroyWindow(*window);
		glfwTerminate();
	}
	bool shouldClose(Window* window)
	{
		return glfwWindowShouldClose(*window);
	}
	void swapBuffers(Window* window)
	{
		glfwSwapBuffers(*window);
	}
	void pollEvents()
	{
		glfwPollEvents();
	}

	void GLFWDebugCallback(int errCode, const char* desc)
	{
#ifdef _OGL_DEBUG
		auto glfwErrorCodeToString = [](int error_code) {
			switch (error_code) {
			case GLFW_NOT_INITIALIZED:
				return "GLFW_NOT_INITIALIZED";
			case GLFW_NO_CURRENT_CONTEXT:
				return "GLFW_NO_CURRENT_CONTEXT";
			case GLFW_INVALID_ENUM:
				return "GLFW_INVALID_ENUM";
			case GLFW_INVALID_VALUE:
				return "GLFW_INVALID_VALUE";
			case GLFW_OUT_OF_MEMORY:
				return "GLFW_OUT_OF_MEMORY";
			case GLFW_API_UNAVAILABLE:
				return "GLFW_API_UNAVAILABLE";
			case GLFW_VERSION_UNAVAILABLE:
				return "GLFW_VERSION_UNAVAILABLE";
			case GLFW_PLATFORM_ERROR:
				return "GLFW_PLATFORM_ERROR";
			case GLFW_FORMAT_UNAVAILABLE:
				return "GLFW_FORMAT_UNAVAILABLE";
			case GLFW_NO_WINDOW_CONTEXT:
				return "GLFW_NO_WINDOW_CONTEXT";
			case GLFW_FOCUS_ON_SHOW:
				return "GLFW_FOCUS_ON_SHOW";
			default:
				return "Unknown GLFW error code.";
			}
			};
		std::cerr << "GLFW Error Callback: " << glfwErrorCodeToString(errCode) << "\nDescription: " << desc << "\n";
		__debugbreak();
#endif
	}
	void __stdcall OGLDebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* msg, const void* userParam)
	{
#ifdef _OGL_DEBUG
		std::string sourceStr;
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:
			sourceStr = "Source: API"; break;
		case GL_DEBUG_SOURCE_APPLICATION:
			sourceStr = "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:
			sourceStr = "Source: Other"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			sourceStr = "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			sourceStr = "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			sourceStr = "Source: Window System"; break;
		default:
			sourceStr = "Source: Unknown";
		}

		std::string typeStr;
		switch (type)
		{
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			typeStr = "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_ERROR:
			typeStr = "Type: Error"; break;
		case GL_DEBUG_TYPE_MARKER:
			typeStr = "Type: Marker"; break;
		case GL_DEBUG_TYPE_OTHER:
			typeStr = "Type: Other"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			typeStr = "Type: Performance"; break;
		case GL_DEBUG_TYPE_POP_GROUP:
			typeStr = "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_PORTABILITY:
			typeStr = "Type: Portability"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			typeStr = "Type: Push Group"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			typeStr = "Type: Undefined Behaviour"; break;
		default:
			typeStr = "Type: Unknown";
		}

		std::string severityStr;
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			severityStr = "Severity: High"; break;
		case GL_DEBUG_SEVERITY_LOW:
			severityStr = "Severity: Low"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			severityStr = "Severity: Medium"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			severityStr = "Severity: Notification"; break;
		default:
			severityStr = "Severity: Unknown";
		}

		std::cerr << "Debug Callback:\n\t" << sourceStr << "\n\t" << typeStr << "\n\tID: " << id << "\n\t" << severityStr << "\n\tLength: " << length << "\nMessage:\n" << msg << "\n";
		__debugbreak();
#endif

#ifdef _OGL_RELEASE
		if (type == GL_DEBUG_TYPE_ERROR || severity == GL_DEBUG_SEVERITY_HIGH)
		{
			exit(EXIT_FAILURE);
		}
#endif
	}
}