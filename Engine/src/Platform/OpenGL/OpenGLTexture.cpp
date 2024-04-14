#include "egpch.h"
#include "OpenGLTexture.h"
#include "Engine/Resources/Systems/ResourceSystem.h"
#include "Engine/Resources/ResourceUtils.h"
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
		Load(path);
	}

	OpenGLTexture2D::OpenGLTexture2D(const UUID& id)
	{
		std::filesystem::path path = ResourceDatabase::GetResourcePath(id);
		Load(path.string());
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		EG_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_RendererID);
	}

	Ref<Texture2D> OpenGLTexture2D::Create(const std::string& path)
	{
		Ref<OpenGLTexture2D> texture = CreateRef<OpenGLTexture2D>();
		if(texture->Load(path))
			return texture;
		return nullptr;
	}

	Ref<Texture2D> OpenGLTexture2D::Create(const UUID& id)
	{
		std::filesystem::path path = ResourceDatabase::GetResourcePath(id);
		EG_PROFILE_FUNCTION();
		return Create(path.string());
	}

	Ref<Texture2D> OpenGLTexture2D::Create(const TextureSpecification& specification)
	{
		return CreateRef<OpenGLTexture2D>(specification);
	}

	bool OpenGLTexture2D::Load(const std::string& path)
	{
		EG_PROFILE_FUNCTION();
		Resource imgResource;
		if (!resourceSystemLoad(path, ResourceType::Image, &imgResource)) {
			EG_ERROR("Failed to load image resource for texture {}", path.c_str());
			return false;
		}

		if (imgResource.Data)
		{
			m_IsLoaded = true;

			m_Width = ((ImageResourceData*)imgResource.Data)->width;
			m_Height = ((ImageResourceData*)imgResource.Data)->height;


			GLenum internalFormat = 0, dataFormat = 0;
			if (((ImageResourceData*)imgResource.Data)->channelCount == 4) {
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (((ImageResourceData*)imgResource.Data)->channelCount == 3) {
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			else if (((ImageResourceData*)imgResource.Data)->channelCount == 2) {
				internalFormat = GL_RG8;
				dataFormat = GL_RG;
			}
			else if (((ImageResourceData*)imgResource.Data)->channelCount == 1) {
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

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, (void*)(((ImageResourceData*)imgResource.Data)->pixels));

			stbi_image_free(((ImageResourceData*)imgResource.Data)->pixels);
			return true;
		}
		return false;
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