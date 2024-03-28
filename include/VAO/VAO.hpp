#ifndef _OGL_VAO_HPP
#define _OGL_VAO_HPP

#include "GL/glew.h"
#include <vector>
#include <optional>

namespace OGL
{
	struct AttribInfo
	{
		unsigned int attribIdx;
		unsigned int count;
		unsigned int offset;

		AttribInfo(unsigned int attribIdx, unsigned int count, unsigned int offset)
			: attribIdx(attribIdx), count(count), offset(offset) {}
	};

	enum class BufferUsageHint
	{
		None,
		StreamDraw,
		StreamRead,
		StreamCopy,
		StaticDraw,
		StaticRead,
		StaticCopy,
		DynamicDraw,
		DynamicRead,
		DynamicCopy,
	};

	struct VBOConfig
	{
		unsigned int bindingIdx;
		BufferUsageHint usage;
		unsigned int stride;
		std::vector<AttribInfo> attribs;

		VBOConfig(unsigned int bindingIdx, BufferUsageHint usage, unsigned int stride, std::vector<AttribInfo> attribs)
			: bindingIdx(bindingIdx), usage(usage), stride(stride), attribs(attribs) {}
	};

	enum class EBDataType
	{
		NONE,
		UINT,
		USHORT,
		UBYTE,
	};

	struct EBOConfig
	{
		EBDataType dataType;
		BufferUsageHint usage;
		unsigned int count;
	};

	class VAO
	{
	public:
		VAO() = default;
		VAO(const VAO& other) = delete;
		VAO(unsigned int ebCount, std::vector<unsigned int> vbCounts, std::vector<VBOConfig> vboInfos);
		~VAO();

		void bind() const;

		void updateVB(unsigned int off, std::vector<float> buffer, unsigned int bindingIdx) const;
		void updateEB(unsigned int off, std::vector<unsigned int> buffer) const;

		void recreateVB(unsigned int vbCount, unsigned int usage, unsigned int bindingIdx) const;
		void recreateEB(unsigned int ebCount, unsigned int usage) const;

		void initialize(unsigned int ebCount, std::vector<unsigned int> vbCounts, std::vector<VBOConfig> vboInfos);

		unsigned int getElementsCount() const { return m_ElementsCount; }
		static constexpr unsigned int getBufferHint(BufferUsageHint usage);

	private:
		bool m_IsInitialized = false;
		bool m_UsesEBO = false;
		EBDataType m_EBDataType = EBDataType::NONE;
		mutable unsigned int m_ElementsCount;

		unsigned int m_VAO, m_EBO;
		std::vector<unsigned int> m_VBOs;
	};
}

#endif