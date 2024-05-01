#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eg {
	template <typename T>
	class UniformBuffer {
	public:
		
		UniformBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize bufferSize, T& uniformBufferObject);
		~UniformBuffer();

		VkBuffer GetBuffer() const;
		VkDeviceMemory GetBufferMemory() const;

		void UpdateBuffer(T& ubo);
	private:
		VkBuffer buffer;
		VkDeviceMemory bufferMemory;
		void* mappedMemory;
	};
}