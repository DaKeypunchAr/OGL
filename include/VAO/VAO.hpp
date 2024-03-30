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
		unsigned int count;
		unsigned int stride;
		std::vector<AttribInfo> attribs;
		BufferUsageHint usage;

		VBOConfig(unsigned int bindingIdx, unsigned int count, unsigned int stride, std::vector<AttribInfo> attribs, BufferUsageHint usage = BufferUsageHint::StaticDraw)
			: bindingIdx(bindingIdx), count(count), stride(stride), attribs(attribs), usage(usage) {}
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
		unsigned int count;
		EBDataType dataType;
		BufferUsageHint usage;
		EBOConfig(unsigned int count, EBDataType dataType = EBDataType::UBYTE, BufferUsageHint usage = BufferUsageHint::StaticDraw)
			: count(count), dataType(dataType), usage(usage) {}
	};

	class VAO
	{
	public:
		VAO() = default;
		VAO(const VAO& other) = delete;
		VAO(std::vector<VBOConfig> vboInfos, std::optional<EBOConfig>& eb = std::optional<EBOConfig>());
		~VAO();

		void bind() const;

		void updateVB(unsigned int bindingIdx, unsigned int off, std::vector<float> buffer) const;
		void updateEB(unsigned int off, std::vector<unsigned int> buffer) const;
		void updateEB(unsigned int off, std::vector<unsigned short> buffer) const;
		void updateEB(unsigned int off, std::vector<unsigned char> buffer) const;

		void recreateVB(VBOConfig vb) const;
		void recreateEB(EBOConfig eb) const;

		void initialize(std::vector<VBOConfig> vboInfos, std::optional<EBOConfig>& eb = std::optional<EBOConfig>());

		constexpr unsigned int getElementsCount() const { return m_ElementsCount; }
		constexpr unsigned int getVerticesCount() const { return m_VertexCount;}
		static constexpr unsigned int getBufferUsageHint(BufferUsageHint usage);
		static constexpr unsigned int getDataTypeSize(EBDataType dataType);
		constexpr bool isInitialized() const { return m_IsInitialized; }
		constexpr bool usesEBO() const { return m_UsesEBO; }
		constexpr EBDataType dataType() const { return m_EBDataType; }

	private:
		bool m_IsInitialized = false;
		bool m_UsesEBO = false;
		mutable EBDataType m_EBDataType = EBDataType::NONE;
		mutable unsigned int m_ElementsCount;
		mutable unsigned int m_VertexCount;

		mutable std::vector<unsigned char> m_VBAttribCount;

		unsigned int m_VAO, m_EBO;
		std::vector<unsigned int> m_VBOs;
	};
}

#endif