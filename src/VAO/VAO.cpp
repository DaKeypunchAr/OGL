#include "VAO/VAO.hpp"

namespace OGL
{
	VAO::VAO(unsigned int ebCount, std::vector<unsigned int> vbCounts, std::vector<VBOConfig> vboInfos)
	{
		initialize(ebCount, vbCounts, vboInfos);
	}
	VAO::~VAO()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_EBO);
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
		glNamedBufferSubData(m_EBO, off * sizeof(unsigned int), buffer.size() * sizeof(unsigned int), buffer.data());
	}

	void VAO::recreateVB(unsigned int vbCount, unsigned int usage, unsigned int bindingIdx) const
	{
		glNamedBufferData(m_VBOs[bindingIdx], vbCount * sizeof(float), nullptr, usage);
	}
	void VAO::recreateEB(unsigned int ebCount, unsigned int usage) const
	{
		m_ElementsCount = ebCount;
		glNamedBufferData(m_EBO, ebCount * sizeof(unsigned int), nullptr, usage);
	}

	void VAO::initialize(unsigned int ebCount, std::vector<unsigned int> vbCounts, std::vector<VBOConfig> vboInfos)
	{
		if (!m_IsInitialized) glCreateVertexArrays(1, &m_VAO);
		else
		{
			glDeleteVertexArrays(1, &m_VAO);
			glDeleteBuffers(1, &m_EBO);
			for (unsigned int vb : m_VBOs)
			{
				glDeleteBuffers(1, &vb);
			}
		}

		glCreateBuffers(1, &m_EBO);
		glNamedBufferData(m_EBO, ebCount * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);
		glVertexArrayElementBuffer(m_VAO, m_EBO);

		for (unsigned int i = 0; i < vbCounts.size(); i++)
		{
			unsigned int buffer;
			glCreateBuffers(1, &buffer);
			glNamedBufferData(buffer, vbCounts[i] * sizeof(float), nullptr, getBufferHint(vboInfos[i].usage));
			glVertexArrayVertexBuffer(m_VAO, vboInfos[i].bindingIdx, buffer, 0, vboInfos[i].stride);

			for (const AttribInfo& info : vboInfos[i].attribs)
			{
				glVertexArrayAttribFormat(m_VAO, info.attribIdx, info.count, GL_FLOAT, GL_FALSE, info.offset);
				glVertexArrayAttribBinding(m_VAO, info.attribIdx, vboInfos[i].bindingIdx);
				glEnableVertexArrayAttrib(m_VAO, info.attribIdx);
			}
			m_VBOs.push_back(buffer);
		}
		m_ElementsCount = ebCount;
	}

	constexpr unsigned int VAO::getBufferHint(BufferUsageHint usage)
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
}