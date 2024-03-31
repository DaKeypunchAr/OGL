#ifndef _OGL_TEXTURE_HPP
#define _OGL_TEXTURE_HPP

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stbi_image.hpp"

#include <string>
#include <optional>

namespace OGL
{
	enum class ScaleFilter
	{
		NEAREST,
		LINEAR,
		NEAREST_ON_NEAREST_MIPMAP, // NEAREST_MIPMAP_NEAREST: returns nearest on the nearest mipmap
		LINEAR_ON_NEAREST_MIPMAP,  // LINEAR_MIPMAP_NEAREST: returns linearly on the nearest mipmap
		NEAREST_ON_LINEAR_MIPMAP,  // NEAREST_MIPMAP_LINEAR: returns nearest on two nearest mipmaps and then take a weighted average ( aka linear ).
		LINEAR_ON_LINEAR_MIPMAP,   // LINEAR_MIPMAP_LINEAR: returns linearly on two nearest mipmaps and then take a weighted average ( aka linear ).
	};

	enum class MinFilter
	{
		NEAREST,
		LINEAR,
		NEAREST_ON_NEAREST_MIPMAP, // NEAREST_MIPMAP_NEAREST: returns nearest on the nearest mipmap
		LINEAR_ON_NEAREST_MIPMAP,  // LINEAR_MIPMAP_NEAREST: returns linearly on the nearest mipmap
		NEAREST_ON_LINEAR_MIPMAP,  // NEAREST_MIPMAP_LINEAR: returns nearest on two nearest mipmaps and then take a weighted average ( aka linear ).
		LINEAR_ON_LINEAR_MIPMAP,   // LINEAR_MIPMAP_LINEAR: returns linearly on two nearest mipmaps and then take a weighted average ( aka linear ).
	};

	enum class MagFilter
	{
		NEAREST,
		LINEAR
	};

	enum class WrapFilter
	{
		REPEAT,
		MIRROR_REPEAT,
		MIRROR_CLAMP_TO_EDGE,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER
	};

	struct TextureFilters
	{
		MinFilter minFilter;
		MagFilter magFilter;
		WrapFilter wrapS, wrapT;
		std::optional<glm::vec4> borderColor;
		TextureFilters(MinFilter minFilter = MinFilter::LINEAR, MagFilter magFilter = MagFilter::LINEAR, WrapFilter wrapS = WrapFilter::REPEAT, WrapFilter wrapT = WrapFilter::REPEAT, std::optional<glm::vec4> borderColor = {})
			: minFilter(minFilter), magFilter(magFilter), wrapS(wrapS), wrapT(wrapT), borderColor(borderColor) {}
		TextureFilters(ScaleFilter minMagFilter = ScaleFilter::LINEAR, WrapFilter wrapST = WrapFilter::REPEAT, std::optional<glm::vec4> borderColor = {})
			: minFilter(getAsMinFilter(minMagFilter)), magFilter(getAsMagFilter(minMagFilter)), wrapS(wrapST), wrapT(wrapST), borderColor(borderColor) {}

		static MinFilter getAsMinFilter(ScaleFilter filter);
		static MagFilter getAsMagFilter(ScaleFilter filter);
		constexpr static unsigned int getMinFilter(MinFilter filter);
		constexpr static unsigned int getMagFilter(MagFilter filter);
		constexpr static unsigned int getWrapFilter(WrapFilter filter);
	};

	enum class Channels
	{
		R,
		RG,
		RGB,
		RGBA
	};

	class Texture2D
	{
	public:
		Texture2D() = default;
		Texture2D(const Texture2D& other) = delete;
		Texture2D(std::string imageLocation, TextureFilters filters, Channels imgChannels = Channels::RGBA);
		Texture2D(glm::uvec2 preservedDimension, TextureFilters filters, Channels imgChannels = Channels::RGBA);
		~Texture2D();

		void bind(unsigned int slot) const;

		void subImage(glm::ivec2 off, glm::ivec2 dim, unsigned char* img) const;

		void initialize(std::string imageLocation, TextureFilters filters, Channels imgChannels = Channels::RGBA);
		void initialize(glm::uvec2 preservedDimension, TextureFilters filters, Channels imgChannels = Channels::RGBA);

		glm::uvec2 getTextureDimensions() const { return m_TextureDimensions; }

	private:
		glm::uvec2 m_TextureDimensions;
		bool m_IsInitialized = false;
		unsigned int m_InternalFormat, m_Format;
		unsigned int m_ID;
	};
}

#endif