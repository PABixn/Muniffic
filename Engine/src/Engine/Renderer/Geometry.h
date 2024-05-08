#pragma once
#include "Buffer.h"
#include "Shader.h"
#include "UniformBuffer.h"
namespace eg {
	class Geometry {
	public:
		Geometry() = default;
		~Geometry() = default;

		static Ref<Geometry> Create(size_t vertexBufferSize, BufferLayout& layout, size_t indexBufferSize, std::string shaderPath);


		virtual void Bind() const = 0;
	private:
	};
}