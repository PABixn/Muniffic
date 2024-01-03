#include "egpch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"

namespace eg {

	namespace Utils
	{
		static GLenum MunifficFormatToGLDataFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::R8: return GL_RED;
			case ImageFormat::RG8: return GL_RG;
			case ImageFormat::RGB8: return GL_RGB;
			case ImageFormat::RGBA8: return GL_RGBA;

			}

			EG_CORE_ASSERT(false);
			return 0;
		}

		static GLenum MunifficFormatToGLInternalFormat(ImageFormat format)
		{
			switch (format)
			{
			case eg::ImageFormat::R8: return GL_R8;
			case eg::ImageFormat::RG8: return GL_RG8;
			case eg::ImageFormat::RGB8: return GL_RGB8;
			case eg::ImageFormat::RGBA8: return GL_RGBA8;
			}

			EG_CORE_ASSERT(false);
			return 0;
		}
	}


	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
		: m_Specification(specification), m_Width(specification.Width), m_Height(specification.Height)
	{
		EG_PROFILE_FUNCTION();

		switch (m_Specification.Format)

			m_InternalFormat = Utils::MunifficFormatToGLInternalFormat(m_Specification.Format);
		m_DataFormat = Utils::MunifficFormatToGLDataFormat(m_Specification.Format);
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		EG_PROFILE_FUNCTION();
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;

		{
			EG_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		if (data)
		{
			m_IsLoaded = true;

			m_Width = width;
			m_Height = height;


			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4) {
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3) {
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			else if (channels == 2) {
				internalFormat = GL_RG8;
				dataFormat = GL_RG;
			}
			else if (channels == 1) {
				internalFormat = GL_R8;
				dataFormat = GL_RED;
			}

			m_InternalFormat = internalFormat; m_DataFormat = dataFormat;

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}

	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		EG_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_RendererID);
	}

	static uint32_t TextureFormatTobpp(GLenum format) {
		switch (format) {
		case GL_RGBA: return 4;
		case GL_RGB: return 3;
		case GL_RG: return 2;
		case GL_RED: return 1;
		}
		EG_CORE_ASSERT(false, "Unknown Texture Format!");
		return 0;
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		EG_PROFILE_FUNCTION();
		uint32_t bpp = TextureFormatTobpp(m_DataFormat);
		EG_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		EG_PROFILE_FUNCTION();
		glBindTextureUnit(slot, m_RendererID);
	}

	
}