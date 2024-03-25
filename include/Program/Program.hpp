#ifndef _OGL_PROGRAM_H
#define _OGL_PROGRAM_H

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string>

namespace OGL
{
	class Program
	{
	public:
		Program();
		Program(const Program& other) = delete;
		Program(std::string folderPath);
		Program(std::string vsFilePath, std::string fsFilePath);
		~Program();

		void use() const;
		void uni1i(std::string uniName, int i) const;
		void uni1f(std::string uniName, float f) const;
		void uni2f(std::string uniName, glm::vec2 vec) const;
		void uni3f(std::string uniName, glm::vec3 vec) const;
		void uni4f(std::string uniName, glm::vec4 vec) const;
		void uniMat4(std::string uniName, glm::mat4 mat) const;

		unsigned int getUniLoc(std::string uniName) const;

		void initialize(std::string folderPath)
		{
			initialize(folderPath + "/vertex.glsl", folderPath + "/fragment.glsl");
		}
		void initialize(std::string vsFilePath, std::string fsFilePath);

	private:
		std::string readFile(std::string filePath) const;
		void compileShader(unsigned int shaderID) const;
		void linkProgram() const;

	private:
		unsigned int m_ID;
		bool m_IsInitialized = false;
	};
}

#endif