#pragma once
#include <filesystem>
#include "Renderer/Resources/Buffer.h"
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif // !RENDERER_INCLUDE_GLFW_VULKAN

#include "Engine/Renderer/Texture.h"

namespace eg {
	class TextureManager;
	class VulkanTexture : public Texture2D
	{
	public:
		static Ref<Texture2D> Create(const TextureSpecification& specification);
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(const UUID& id);
	public:
		VulkanTexture() = default;
		VulkanTexture(const char* path);
		~VulkanTexture() { cleanUp(); }
		void cleanUp();
		VkImage m_TextureImage;
		VkDeviceMemory m_TextureImageMemory;
	public:
		void LoadTexture(TextureManager* textureManager, const char* path);
		uint32_t width;
		uint32_t height;
		uint32_t rendererID = 1;
		std::string path;
	private:
		virtual const TextureSpecification& GetSpecification() const override { return TextureSpecification(); };

		virtual uint32_t GetWidth() const override { return width; }
		virtual uint32_t GetHeight() const override { return height; }
		virtual uint32_t GetRendererID() const override { return rendererID; };

		virtual const std::string& GetPath() const override { return path; };

		virtual const UUID& GetID() const override { return 0; };

		virtual void SetData(void* data, uint32_t size) override {};

		virtual void Bind(uint32_t slot = 0) const override {};

		virtual bool IsLoaded() const override { return false; };

		virtual bool operator==(const Texture& other) const override { return this->GetID() == other.GetID(); };
	};
}