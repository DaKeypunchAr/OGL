#include "VAO/VAO.hpp"

#ifdef _OGL_DEBUG
#include <iostream>
#endif

namespace OGL
{
	VAO::VAO(std::vector<unsigned int> vbCounts, std::vector<VBOConfig> vboInfos, std::optional<EBOConfig>& eb)
	{
		initialize(vbCounts, vboInfos, eb);
	}
	VAO::~VAO()
	{
		glDeleteVertexArrays(1, &m_VAO);
		if (m_UsesEBO) glDeleteBuffers(1, &m_EBO);
		for (unsigned int vb : m_VBOs)
		{
			glDeleteBuffers(1, &vb);
		}
	}

	void VAO::bind() const
	{
		glBindVertexArray(m_VAO);
	}

	void VAO::updateVB(unsigned int off, std::vector<float> buffer, unsigned int bindingIdx) const
	{
		glNamedBufferSubData(m_VBOs[bindingIdx], off * sizeof(float), buffer.size() * sizeof(float), buffer.data());
	}
	void VAO::updateEB(unsigned int off, std::vector<unsigned int> buffer) const
	{
		if (m_UsesEBO)
		{
			if (m_EBDataType != EBDataType::UINT)
			{
#ifdef _OGL_DEBUG
				std::cerr << "EBO is not set to use unsigned int!\n";
				__debugbreak();
#else
				exit(EXIT_FAILURE);
#endif
			}
			glNamedBufferSubData(m_EBO, off * sizeof(unsigned int), buffer.size() * sizeof(unsigned int), buffer.data());
		}
#ifdef _OGL_DEBUG
		else
		{
			std::cerr << "EBO is not present. So you cannot update it!\n";
			__debugbreak();
		}
#endif
	}
	void VAO::updateEB(unsigned int off, std::vector<unsigned short> buffer) const
	{
		if (m_UsesEBO)
		{
			if (m_EBDataType != EBDataType::USHORT)
			{
#ifdef _OGL_DEBUG
				std::cerr << "EBO is not set to use unsigned short!\n";
				__debugbreak();
#else
				exit(EXIT_FAILURE);
#endif
			}
			glNamedBufferSubData(m_EBO, off * sizeof(unsigned short), buffer.size() * sizeof(unsigned short), buffer.data());
		}
#ifdef _OGL_DEBUG
		else
		{
			std::cerr << "EBO is not present. So you cannot update it!\n";
			__debugbreak();
		}
#endif
	}
	void VAO::updateEB(unsigned int off, std::vector<unsigned char> buffer) const
	{
		if (m_UsesEBO)
		{
			if (m_EBDataType != EBDataType::UBYTE)
			{
#ifdef _OGL_DEBUG
				std::cerr << "EBO is not set to use unsigned byte!\n";
				__debugbreak();
#else
				exit(EXIT_FAILURE);
#endif
			}
			glNamedBufferSubData(m_EBO, off * sizeof(unsigned char), buffer.size() * sizeof(unsigned char), buffer.data());
		}
#ifdef _OGL_DEBUG
		else
		{
			std::cerr << "EBO is not present. So you cannot update it!\n";
			__debugbreak();
		}
#endif
	}

	void VAO::recreateVB(unsigned int vbCount, unsigned int usage, unsigned int bindingIdx) const
	{
		glNamedBufferData(m_VBOs[bindingIdx], vbCount * sizeof(float), nullptr, usage);
	}
	void VAO::recreateEB(EBOConfig eb) const
	{
		if (!m_UsesEBO)
		{
#ifdef _OGL_DEBUG
			std::cerr << "You are recreating a ebo without an ebo!\n";
			__debugbreak();
#endif
		}
		m_ElementsCount = eb.count;
		m_EBDataType = eb.dataType;
		glNamedBufferData(m_EBO, m_ElementsCount * getDataTypeSize(m_EBDataType), nullptr, getBufferUsageHint(eb.usage));
	}

	void VAO::initialize(std::vector<unsigned int> vbCounts, std::vector<VBOConfig> vboInfos, std::optional<EBOConfig>& eb)
	{
		if (!m_IsInitialized) glCreateVertexArrays(1, &m_VAO);
		else
		{
			glDeleteVertexArrays(1, &m_VAO);
			if (m_UsesEBO) glDeleteBuffers(1, &m_EBO);
			for (unsigned int vb : m_VBOs)
			{
				glDeleteBuffers(1, &vb);
			}
		}

		m_UsesEBO = eb.has_value();
		if (m_UsesEBO)
		{
			m_EBDataType = eb.value().dataType;
			m_ElementsCount = eb.value().count;
			glCreateBuffers(1, &m_EBO);
			glNamedBufferData(m_EBO, m_ElementsCount * getDataTypeSize(m_EBDataType), nullptr, getBufferUsageHint(eb.value().usage));
			glVertexArrayElementBuffer(m_VAO, m_EBO);
		}

		for (unsigned int i = 0; i < vbCounts.size(); i++)
		{
			unsigned int buffer;
			glCreateBuffers(1, &buffer);
			glNamedBufferData(buffer, vbCounts[i] * sizeof(float), nullptr, getBufferUsageHint(vboInfos[i].usage));
			glVertexArrayVertexBuffer(m_VAO, vboInfos[i].bindingIdx, buffer, 0, vboInfos[i].stride);

			for (const AttribInfo& info : vboInfos[i].attribs)
			{
				glVertexArrayAttribFormat(m_VAO, info.attribIdx, info.count, GL_FLOAT, GL_FALSE, info.offset);
				glVertexArrayAttribBinding(m_VAO, info.attribIdx, vboInfos[i].bindingIdx);
				glEnableVertexArrayAttrib(m_VAO, info.attribIdx);
			}
			m_VBOs.push_back(buffer);
		}
	}

	constexpr unsigned int VAO::getBufferUsageHint(BufferUsageHint usage)
	{
		switch (usage)
		{
		case BufferUsageHint::None:
		case BufferUsageHint::StaticDraw:
			return GL_STATIC_DRAW;
		case BufferUsageHint::StreamCopy:
			return GL_STREAM_COPY;
		case BufferUsageHint::StreamRead:
			return GL_STREAM_READ;
		case BufferUsageHint::StreamDraw:
			return GL_STREAM_DRAW;
		case BufferUsageHint::StaticRead:
			return GL_STATIC_READ;
		case BufferUsageHint::StaticCopy:
			return GL_STATIC_COPY;
		case BufferUsageHint::DynamicCopy:
			return GL_DYNAMIC_COPY;
		case BufferUsageHint::DynamicDraw:
			return GL_DYNAMIC_DRAW;
		case BufferUsageHint::DynamicRead:
			return GL_DYNAMIC_READ;
		}
		return 0; // Not happen though. Just for warning protection!
	}
	constexpr unsigned int VAO::getDataTypeSize(EBDataType dataType)
	{
		switch (dataType)
		{
		case EBDataType::NONE:
			return 0;
		case EBDataType::UBYTE:
			return sizeof(unsigned char);
		case EBDataType::USHORT:
			return sizeof(unsigned short);
		case EBDataType::UINT:
			return sizeof(unsigned int);
		}
		return 0; // only here to prevent warning.
	}
}