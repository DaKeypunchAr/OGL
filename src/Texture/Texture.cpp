#include "Texture/Texture.hpp"

#ifdef _OGL_DEBUG
#include <iostream>
#endif

namespace OGL
{
	MinFilter TextureFilters::getAsMinFilter(ScaleFilter filter)
	{
		switch (filter)
		{
		case ScaleFilter::NEAREST:
			return MinFilter::NEAREST;
		case ScaleFilter::LINEAR:
			return MinFilter::LINEAR;
		case ScaleFilter::NEAREST_ON_NEAREST_MIPMAP:
			return MinFilter::NEAREST_ON_NEAREST_MIPMAP;
		case ScaleFilter::LINEAR_ON_NEAREST_MIPMAP:
			return MinFilter::LINEAR_ON_NEAREST_MIPMAP;
		case ScaleFilter::NEAREST_ON_LINEAR_MIPMAP:
			return MinFilter::NEAREST_ON_LINEAR_MIPMAP;
		case ScaleFilter::LINEAR_ON_LINEAR_MIPMAP:
			return MinFilter::LINEAR_ON_LINEAR_MIPMAP;
		}
		return MinFilter::NEAREST; // warning protection :)
	}
	MagFilter TextureFilters::getAsMagFilter(ScaleFilter filter)
	{
		switch (filter)
		{
		case ScaleFilter::NEAREST:
		case ScaleFilter::NEAREST_ON_NEAREST_MIPMAP:
		case ScaleFilter::NEAREST_ON_LINEAR_MIPMAP:
			return MagFilter::NEAREST;
		case ScaleFilter::LINEAR:
		case ScaleFilter::LINEAR_ON_NEAREST_MIPMAP:
		case ScaleFilter::LINEAR_ON_LINEAR_MIPMAP:
			return MagFilter::LINEAR;
		}
		return MagFilter::NEAREST; // warning protection
	}
	constexpr unsigned int TextureFilters::getMinFilter(MinFilter filter)
	{
		switch (filter)
		{
		case MinFilter::NEAREST:
			return GL_NEAREST;
		case MinFilter::LINEAR:
			return GL_LINEAR;
		case MinFilter::NEAREST_ON_NEAREST_MIPMAP:
			return GL_NEAREST_MIPMAP_NEAREST;
		case MinFilter::NEAREST_ON_LINEAR_MIPMAP:
			return GL_NEAREST_MIPMAP_LINEAR;
		case MinFilter::LINEAR_ON_NEAREST_MIPMAP:
			return GL_LINEAR_MIPMAP_NEAREST;
		case MinFilter::LINEAR_ON_LINEAR_MIPMAP:
			return GL_LINEAR_MIPMAP_LINEAR;
		}
		return 0; // warning protection :)
	}
	constexpr unsigned int TextureFilters::getMagFilter(MagFilter filter)
	{
		switch (filter)
		{
		case MagFilter::NEAREST:
			return GL_NEAREST;
		case MagFilter::LINEAR:
			return GL_LINEAR;
		}
		return 0; // warning protection :)
	}
	constexpr unsigned int TextureFilters::getWrapFilter(WrapFilter filter)
	{
		switch (filter)
		{
		case WrapFilter::REPEAT:
			return GL_REPEAT;
		case WrapFilter::MIRROR_REPEAT:
			return GL_MIRRORED_REPEAT;
		case WrapFilter::MIRROR_CLAMP_TO_EDGE:
			return GL_MIRROR_CLAMP_TO_EDGE;
		case WrapFilter::CLAMP_TO_BORDER:
			return GL_CLAMP_TO_BORDER;
		case WrapFilter::CLAMP_TO_EDGE:
			return GL_CLAMP_TO_EDGE;
		}
		return 0; // warning protection :)
	}

	Texture2D::Texture2D(std::string imageLocation, TextureFilters filters, Channels channels)
	{
		initialize(imageLocation, filters, channels);
	}
	Texture2D::Texture2D(glm::uvec2 preservedDimension, TextureFilters filters, Channels channels)
	{
		initialize(preservedDimension, filters, channels);
	}
	Texture2D::~Texture2D()
	{
		if (m_IsInitialized) glDeleteTextures(1, &m_ID);
	}

	void Texture2D::bind(unsigned int slot) const
	{
		if (!m_IsInitialized)
		{
#ifdef _OGL_DEBUG
			std::cerr << "You cannot use a uninitialized texture!\n";
			__debugbreak();
#endif
			return;
		}
		glBindTextureUnit(slot, m_ID);
	}
	void Texture2D::subImage(glm::ivec2 off, glm::ivec2 dim, unsigned char* img) const
	{
		switch (m_Format)
		{
		case GL_RED:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			break;
		case GL_RG:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
			break;
		case GL_RGB:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
			break;
		case GL_RGBA:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			break;
		}

		glTextureSubImage2D(m_ID, 0, off.x, off.y, dim.x, dim.y, m_Format, GL_UNSIGNED_BYTE, img);
	}

	void Texture2D::initialize(std::string imageLocation, TextureFilters filters, Channels channels)
	{
		if (m_IsInitialized)
		{
			glDeleteTextures(1, &m_ID);
		}
		unsigned int numOfChannel = 0;
		switch (channels)
		{
		case Channels::R:
			m_InternalFormat = GL_R8;
			m_Format = GL_RED;
			numOfChannel = 1;
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			break;
		case Channels::RG:
			m_InternalFormat = GL_RG8;
			m_Format = GL_RG;
			numOfChannel = 2;
			glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
			break;
		case Channels::RGB:
			m_InternalFormat = GL_RGB8;
			m_Format = GL_RGB;
			numOfChannel = 3;
			glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
			break;
		case Channels::RGBA:
			m_InternalFormat = GL_RGBA8;
			m_Format = GL_RGBA;
			numOfChannel = 4;
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			break;
		}

		stbi_set_flip_vertically_on_load(true);
		int w, h;
		unsigned char* img = stbi_load(imageLocation.c_str(), &w, &h, nullptr, numOfChannel);
		m_TextureDimensions = { w, h };

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glTextureStorage2D(m_ID, 1, m_InternalFormat, m_TextureDimensions.x, m_TextureDimensions.y);
		subImage({ 0, 0 }, m_TextureDimensions, img);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, TextureFilters::getMinFilter(filters.minFilter));
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, TextureFilters::getMagFilter(filters.magFilter));
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, TextureFilters::getWrapFilter(filters.wrapS));
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, TextureFilters::getWrapFilter(filters.wrapT));

		if (filters.wrapS == WrapFilter::CLAMP_TO_BORDER || filters.wrapT == WrapFilter::CLAMP_TO_BORDER)
		{
			if (filters.borderColor.has_value()) glTextureParameterfv(m_ID, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(filters.borderColor.value()));
		}

		switch (filters.minFilter)
		{
		case MinFilter::NEAREST_ON_NEAREST_MIPMAP:
		case MinFilter::LINEAR_ON_NEAREST_MIPMAP:
		case MinFilter::NEAREST_ON_LINEAR_MIPMAP:
		case MinFilter::LINEAR_ON_LINEAR_MIPMAP:
			glGenerateTextureMipmap(m_ID);
		}

		stbi_image_free(img);
		m_IsInitialized = true;
	}
	void Texture2D::initialize(glm::uvec2 preservedDimension, TextureFilters filters, Channels channels)
	{
		if (m_IsInitialized)
		{
			glDeleteTextures(1, &m_ID);
		}
		switch (channels)
		{
		case Channels::R:
			m_InternalFormat = GL_R8;
			m_Format = GL_RED;
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			break;
		case Channels::RG:
			m_InternalFormat = GL_RG8;
			m_Format = GL_RG;
			glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
			break;
		case Channels::RGB:
			m_InternalFormat = GL_RGB8;
			m_Format = GL_RGB;
			glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
			break;
		case Channels::RGBA:
			m_InternalFormat = GL_RGBA8;
			m_Format = GL_RGBA;
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			break;
		}

		m_TextureDimensions = preservedDimension;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glTextureStorage2D(m_ID, 1, m_InternalFormat, m_TextureDimensions.x, m_TextureDimensions.y);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, TextureFilters::getMinFilter(filters.minFilter));
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, TextureFilters::getMagFilter(filters.magFilter));
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, TextureFilters::getWrapFilter(filters.wrapS));
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, TextureFilters::getWrapFilter(filters.wrapT));

		if (filters.wrapS == WrapFilter::CLAMP_TO_BORDER || filters.wrapT == WrapFilter::CLAMP_TO_BORDER)
		{
			if (filters.borderColor.has_value()) glTextureParameterfv(m_ID, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(filters.borderColor.value()));
		}

		switch (filters.minFilter)
		{
		case MinFilter::NEAREST_ON_NEAREST_MIPMAP:
		case MinFilter::LINEAR_ON_NEAREST_MIPMAP:
		case MinFilter::NEAREST_ON_LINEAR_MIPMAP:
		case MinFilter::LINEAR_ON_LINEAR_MIPMAP:
			glGenerateTextureMipmap(m_ID);
		}

		m_IsInitialized = true;
	}
}