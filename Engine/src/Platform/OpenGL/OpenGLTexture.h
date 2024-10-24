#pragma once
#include "Engine/Renderer/Texture.h"

#include <glad/glad.h>
namespace eg
{
	typedef void* ImTextureID;
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D() = default;
		OpenGLTexture2D(const TextureSpecification& specification);
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(const UUID& id);
		virtual ~OpenGLTexture2D();


		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(const UUID& id);
		static Ref<Texture2D> Create(const TextureSpecification& specification);

		bool Load(const std::string& path);

		virtual const TextureSpecification& GetSpecification() const override { return m_Specification; }
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual ImTextureID GetRendererID() const override { return m_RendererID; }

		virtual const UUID& GetID() const override { return m_Id; }

		virtual const std::string& GetPath() const override { return m_Path; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}
	private:
		
	private:TextureSpecification m_Specification;

		std::string m_Path;
		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height;	
		ImTextureID m_RendererID;
		GLenum m_InternalFormat = GL_RGBA8, m_DataFormat = GL_RGBA;
		UUID m_Id;
	};
}