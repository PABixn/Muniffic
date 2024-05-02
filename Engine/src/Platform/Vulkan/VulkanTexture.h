#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Renderer/Texture.h"
#include "VulkanImage.h"
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanTexture :public Texture2D {
		VulkanTexture() = default;
		VulkanTexture(const TextureSpecification& specification);
		VulkanTexture(const std::string& path);
		VulkanTexture(const UUID& id);
		virtual ~VulkanTexture();

		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(const UUID& id);
		static Ref<Texture2D> Create(const TextureSpecification& specification);

		bool Load(const std::string& path);

		virtual const TextureSpecification& GetSpecification() const override { return m_Specification; }
		virtual uint32_t GetWidth() const override { return m_Specification.Width; }
		virtual uint32_t GetHeight() const override { return m_Specification.Height; }

		virtual const UUID& GetID() const override { return m_Id; }

		virtual const std::string& GetPath() const override { return m_Path; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }
	private:

	private:
		std::string m_Path;
		VulkanImage m_Image;
		TextureSpecification m_Specification;
		UUID m_Id;
		bool m_IsLoaded = false;
	};
}