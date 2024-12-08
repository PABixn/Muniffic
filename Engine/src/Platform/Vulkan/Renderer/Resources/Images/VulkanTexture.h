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
#include "Imgui/imgui.h"

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
		VulkanTexture(const char* path, bool sceneImage = false);
		VulkanTexture(const TextureSpecification& texSpec);
		~VulkanTexture() { cleanUp(); }
		void cleanUp();
		VkImage m_TextureImage;
		VkDeviceMemory m_TextureImageMemory;
		VkImageView m_TextureImageView;
		VkSampler m_Sampler;
	public:
		void LoadTexture(TextureManager* textureManager, const char* path, bool isSceneImage = false);
		uint32_t width;
		uint32_t height;
		ImTextureID rendererID;
		std::string path;
		bool loaded = false;
	private:
		virtual const TextureSpecification& GetSpecification() const override { return TextureSpecification(); };

		virtual uint32_t GetWidth() const override { return width; }
		virtual uint32_t GetHeight() const override { return height; }
		virtual ImTextureID GetRendererID() const override;

		virtual const std::string& GetPath() const override { return path; };

		virtual const UUID& GetID() const override { return 0; };

		virtual void SetData(void* data, uint32_t size) override {};

		virtual void Bind(uint32_t slot = 0) const override {};

		virtual bool IsLoaded() const override { return loaded; };

		virtual bool operator==(const Texture& other) const override { return this->GetID() == other.GetID(); };
	};
}